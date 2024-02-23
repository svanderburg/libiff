/*
 * Copyright (c) 2012 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "group.h"
#include <stdlib.h>
#include "id.h"
#include "error.h"
#include "util.h"

void IFF_initGroup(IFF_Group *group, const IFF_ID groupType)
{
    group->groupType = groupType;
    group->chunkLength = 0;
    group->chunk = NULL;
}

IFF_Group *IFF_createGroup(const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID groupType)
{
    IFF_Group *group = (IFF_Group*)IFF_allocateChunk(chunkId, chunkSize, sizeof(IFF_Group));

    if(group != NULL)
        IFF_initGroup(group, groupType);

    return group;
}

IFF_Group *IFF_createEmptyGroup(const IFF_ID chunkId, const IFF_ID groupType)
{
    return IFF_createGroup(chunkId, IFF_ID_SIZE /* We have a group ID so the minimum size is bigger than 0 */, groupType);
}

void IFF_attachToGroup(IFF_Group *group, IFF_Chunk *chunk)
{
    group->chunk = (IFF_Chunk**)realloc(group->chunk, (group->chunkLength + 1) * sizeof(IFF_Chunk*));
    group->chunk[group->chunkLength] = chunk;
    group->chunkLength++;
    chunk->parent = group;
}

void IFF_addToGroup(IFF_Group *group, IFF_Chunk *chunk)
{
    IFF_attachToGroup(group, chunk);
    group->chunkSize = IFF_incrementChunkSize(group->chunkSize, chunk);
}

static IFF_Bool readGroupSubChunks(FILE *file, IFF_Group *group, const IFF_ID groupType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_Long bytesProcessed = IFF_ID_SIZE; /* The groupType field was already processed, so the size is bigger than 0 */

    while(bytesProcessed < group->chunkSize)
    {
        /* Read sub chunk */
        IFF_Chunk *chunk = IFF_readChunk(file, groupType, extension, extensionLength);

        if(chunk == NULL)
            return FALSE;

        /* Attach chunk to the group */
        IFF_attachToGroup(group, chunk);

        /* Increase the bytes processed counter */
        bytesProcessed = IFF_incrementChunkSize(bytesProcessed, chunk);
    }

    if(bytesProcessed > group->chunkSize)
        IFF_error("WARNING: truncated group chunk! The size specifies: %d but the total amount of its sub chunks is: %d bytes. The parser may get confused!\n", group->chunkSize, bytesProcessed);

    return TRUE;
}

IFF_Group *IFF_readGroup(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const char *groupTypeName, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_ID groupType;
    IFF_Group *group;

    /* Read group type */
    if(!IFF_readId(file, &groupType, chunkId, groupTypeName))
        return NULL;

    /* Create new group */
    group = IFF_createGroup(chunkId, chunkSize, groupType);

    if(group == NULL)
        return NULL;

    /* Keep parsing sub chunks until we have read all bytes */
    if(!readGroupSubChunks(file, group, groupType, extension, extensionLength))
    {
        IFF_error("Error while reading chunk!\n");
        IFF_freeChunk((IFF_Chunk*)group, groupType, extension, extensionLength);
        return NULL;
    }

    /* Return the resulting group */
    return group;
}

IFF_Bool IFF_writeGroupSubChunks(FILE *file, const IFF_Group *group, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;

    for(i = 0; i < group->chunkLength; i++)
    {
        if(!IFF_writeChunk(file, group->chunk[i], formType, extension, extensionLength))
        {
            IFF_error("Error writing chunk!\n");
            return FALSE;
        }
    }

    return TRUE;
}

IFF_Bool IFF_writeGroup(FILE *file, const IFF_Group *group, const IFF_ID formType, const char *groupTypeName, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(!IFF_writeId(file, group->groupType, group->chunkId, groupTypeName))
        return FALSE;

    if(!IFF_writeGroupSubChunks(file, group, formType, extension, extensionLength))
        return FALSE;

    return TRUE;
}

IFF_Bool IFF_checkGroupChunkSize(const IFF_Group *group, const IFF_Long chunkSize)
{
    if(chunkSize == group->chunkSize)
        return TRUE;
    else
    {
        IFF_error("Chunk size mismatch! ");
        IFF_errorId(group->chunkId);
        IFF_error(" size: %d, while body has: %d\n", group->chunkSize, chunkSize);
        return FALSE;
    }
}

IFF_Long IFF_checkGroupSubChunks(const IFF_Group *group, IFF_Bool (*subChunkCheck) (const IFF_Group *group, const IFF_Chunk *subChunk), const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;
    IFF_Long chunkSize = 0;

    for(i = 0; i < group->chunkLength; i++)
    {
        IFF_Chunk *subChunk = group->chunk[i];

        if(!subChunkCheck(group, subChunk))
            return -1;

        /* Check validity of the sub chunk */
        if(!IFF_checkChunk(subChunk, formType, extension, extensionLength))
            return -1;

        chunkSize = IFF_incrementChunkSize(chunkSize, subChunk);
    }

    return chunkSize;
}

IFF_Bool IFF_checkGroup(const IFF_Group *group, IFF_Bool (*groupTypeCheck) (const IFF_ID groupType), IFF_Bool (*subChunkCheck) (const IFF_Group *group, const IFF_Chunk *subChunk), const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_Long chunkSize;

    if(!groupTypeCheck(group->groupType))
        return FALSE;

    if((chunkSize = IFF_checkGroupSubChunks(group, subChunkCheck, formType, extension, extensionLength)) == -1)
        return FALSE;

    chunkSize += IFF_ID_SIZE;

    if(!IFF_checkGroupChunkSize(group, chunkSize))
        return FALSE;

    return TRUE;
}

void IFF_freeGroup(IFF_Group *group, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;

    for(i = 0; i < group->chunkLength; i++)
        IFF_freeChunk(group->chunk[i], formType, extension, extensionLength);

    free(group->chunk);
}

void IFF_printGroupType(const char *groupTypeName, const IFF_ID groupType, const unsigned int indentLevel)
{
    IFF_printIndent(stdout, indentLevel, "%s = '", groupTypeName);
    IFF_printId(groupType);
    printf("';\n");
}

void IFF_printGroupSubChunks(const IFF_Group *group, const unsigned int indentLevel, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;

    IFF_printIndent(stdout, indentLevel, "[\n");

    for(i = 0; i < group->chunkLength; i++)
        IFF_printChunk((IFF_Chunk*)group->chunk[i], indentLevel + 1, formType, extension, extensionLength);

    IFF_printIndent(stdout, indentLevel, "];\n");
}

void IFF_printGroup(const IFF_Group *group, const unsigned int indentLevel, const IFF_ID formType, const char *groupTypeName, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printGroupType(groupTypeName, group->groupType, indentLevel);
    IFF_printGroupSubChunks(group, indentLevel, formType, extension, extensionLength);
}

IFF_Bool IFF_compareGroup(const IFF_Group *group1, const IFF_Group *group2, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if((group1->groupType == group2->groupType) && (group1->chunkLength == group2->chunkLength))
    {
        unsigned int i;

        for(i = 0; i < group1->chunkLength; i++)
        {
            if(!IFF_compareChunk(group1->chunk[i], group2->chunk[i], formType, extension, extensionLength))
                return FALSE;
        }

        return TRUE;
    }
    else
        return FALSE;
}

IFF_Form **IFF_searchFormsInGroup(IFF_Group *group, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    IFF_Form **forms = NULL;
    unsigned int i;

    *formsLength = 0;

    for(i = 0; i < group->chunkLength; i++)
    {
        unsigned int resultLength;
        IFF_Form **result = IFF_searchFormsFromArray(group->chunk[i], formTypes, formTypesLength, &resultLength);

        forms = IFF_mergeFormArray(forms, formsLength, result, resultLength);
    }

    return forms;
}

IFF_Long IFF_incrementChunkSize(const IFF_Long chunkSize, const IFF_Chunk *chunk)
{
    IFF_Long returnValue = chunkSize + IFF_ID_SIZE + sizeof(IFF_Long) + chunk->chunkSize;

    /* If the size of the nested chunk size is odd, we have to count the padding byte as well */
    if(chunk->chunkSize % 2 != 0)
        returnValue++;

    return returnValue;
}

void IFF_updateGroupChunkSizes(IFF_Group *group)
{
    unsigned int i;

    group->chunkSize = IFF_ID_SIZE;

    for(i = 0; i < group->chunkLength; i++)
        group->chunkSize = IFF_incrementChunkSize(group->chunkSize, group->chunk[i]);
}
