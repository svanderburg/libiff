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
#include "form.h"
#include "cat.h"
#include "list.h"
#include "prop.h"
#include "field.h"
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
    IFF_Group *group = (IFF_Group*)IFF_createChunk(chunkId, chunkSize, sizeof(IFF_Group));

    if(group != NULL)
        IFF_initGroup(group, groupType);

    return group;
}

IFF_Group *IFF_createEmptyGroup(const IFF_ID chunkId, const IFF_ID groupType)
{
    return IFF_createGroup(chunkId, IFF_ID_SIZE /* We have a group ID so the minimum size is bigger than 0 */, groupType);
}

IFF_Chunk *IFF_createUnparsedGroup(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    return (IFF_Chunk*)IFF_createGroup(chunkId, chunkSize, 0);
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

static IFF_Bool readGroupSubChunks(FILE *file, IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    while(*bytesProcessed < group->chunkSize)
    {
        /* Read sub chunk */
        IFF_Chunk *chunk = IFF_readChunk(file, group->groupType, chunkRegistry);

        if(chunk == NULL)
            return FALSE;

        /* Attach chunk to the group */
        IFF_attachToGroup(group, chunk);

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, chunk);
    }

    if(*bytesProcessed > group->chunkSize)
        IFF_error("WARNING: truncated group chunk! The size specifies: %d but the total amount of its sub chunks is: %d bytes. The parser may get confused!\n", group->chunkSize, bytesProcessed);

    return TRUE;
}

IFF_Bool IFF_readGroup(FILE *file, IFF_Chunk *chunk, const char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    IFF_Group *group = (IFF_Group*)chunk;
    IFF_FieldStatus status;

    /* Read group type */
    if((status = IFF_readIdField(file, &group->groupType, chunk, groupTypeName, bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    /* Keep parsing sub chunks until we have read all bytes */
    if(!readGroupSubChunks(file, group, chunkRegistry, bytesProcessed))
        return FALSE;

    return TRUE;
}

IFF_Bool IFF_writeGroupSubChunks(FILE *file, const IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    unsigned int i;

    for(i = 0; i < group->chunkLength; i++)
    {
        if(!IFF_writeChunk(file, group->chunk[i], group->groupType, chunkRegistry))
        {
            IFF_error("Error writing chunk!\n");
            return FALSE;
        }

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, group->chunk[i]);
    }

    return TRUE;
}

IFF_Bool IFF_writeGroup(FILE *file, const IFF_Chunk *chunk, const char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const IFF_Group *group = (const IFF_Group*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeIdField(file, group->groupType, chunk, groupTypeName, bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if(!IFF_writeGroupSubChunks(file, group, chunkRegistry, bytesProcessed))
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

IFF_Long IFF_checkGroupSubChunks(const IFF_Group *group, IFF_Bool (*subChunkCheck) (const IFF_Group *group, const IFF_Chunk *subChunk), const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;
    IFF_Long chunkSize = 0;

    for(i = 0; i < group->chunkLength; i++)
    {
        IFF_Chunk *subChunk = group->chunk[i];

        if(!subChunkCheck(group, subChunk))
            return -1;

        /* Check validity of the sub chunk */
        if(!IFF_checkChunk(subChunk, group->groupType, chunkRegistry))
            return -1;

        chunkSize = IFF_incrementChunkSize(chunkSize, subChunk);
    }

    return chunkSize;
}

IFF_Bool IFF_checkGroup(const IFF_Group *group, IFF_Bool (*groupTypeCheck) (const IFF_ID groupType), IFF_Bool (*subChunkCheck) (const IFF_Group *group, const IFF_Chunk *subChunk), const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_Long chunkSize;

    if(!groupTypeCheck(group->groupType))
        return FALSE;

    if((chunkSize = IFF_checkGroupSubChunks(group, subChunkCheck, chunkRegistry)) == -1)
        return FALSE;

    chunkSize += IFF_ID_SIZE;

    if(!IFF_checkGroupChunkSize(group, chunkSize))
        return FALSE;

    return TRUE;
}

void IFF_freeGroup(IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    for(i = 0; i < group->chunkLength; i++)
        IFF_freeChunk(group->chunk[i], group->groupType, chunkRegistry);

    free(group->chunk);
}

void IFF_printGroupType(const char *groupTypeName, const IFF_ID groupType, const unsigned int indentLevel)
{
    IFF_printIndent(stdout, indentLevel, "%s = '", groupTypeName);
    IFF_printId(groupType);
    printf("';\n");
}

void IFF_printGroupSubChunks(const IFF_Group *group, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    IFF_printIndent(stdout, indentLevel, "[\n");

    for(i = 0; i < group->chunkLength; i++)
        IFF_printChunk((IFF_Chunk*)group->chunk[i], indentLevel + 1, group->groupType, chunkRegistry);

    IFF_printIndent(stdout, indentLevel, "];\n");
}

void IFF_printGroup(const IFF_Group *group, const unsigned int indentLevel, const char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroupType(groupTypeName, group->groupType, indentLevel);
    IFF_printGroupSubChunks(group, indentLevel, chunkRegistry);
}

IFF_Bool IFF_compareGroup(const IFF_Group *group1, const IFF_Group *group2, const IFF_ChunkRegistry *chunkRegistry)
{
    if(group1->groupType == group2->groupType && group1->chunkLength == group2->chunkLength)
    {
        unsigned int i;

        for(i = 0; i < group1->chunkLength; i++)
        {
            if(!IFF_compareChunk(group1->chunk[i], group2->chunk[i], group1->groupType, chunkRegistry))
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

IFF_Form **IFF_searchFormsFromArray(IFF_Chunk *chunk, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    switch(chunk->chunkId)
    {
        case IFF_ID_FORM:
            return IFF_searchFormsInForm((IFF_Form*)chunk, formTypes, formTypesLength, formsLength);
        case IFF_ID_CAT:
            return IFF_searchFormsInCAT((IFF_CAT*)chunk, formTypes, formTypesLength, formsLength);
        case IFF_ID_LIST:
            return IFF_searchFormsInList((IFF_List*)chunk, formTypes, formTypesLength, formsLength);
        default:
            *formsLength = 0;
            return NULL;
    }
}

IFF_Form **IFF_searchForms(IFF_Chunk *chunk, const IFF_ID formType, unsigned int *formsLength)
{
    IFF_ID formTypes[1];
    formTypes[0] = formType;
    return IFF_searchFormsFromArray(chunk, formTypes, 1, formsLength);
}

void IFF_updateChunkSizes(IFF_Chunk *chunk)
{
    /* Check whether the given chunk is a group chunk and update the sizes */
    switch(chunk->chunkId)
    {
        case IFF_ID_FORM:
            IFF_updateFormChunkSizes((IFF_Form*)chunk);
            break;
        case IFF_ID_PROP:
            IFF_updatePropChunkSizes((IFF_Prop*)chunk);
            break;
        case IFF_ID_CAT:
            IFF_updateCATChunkSizes((IFF_CAT*)chunk);
            break;
        case IFF_ID_LIST:
            IFF_updateListChunkSizes((IFF_List*)chunk);
            break;
    }

    /* If the given type has a parent, recursively update these as well */
    if(chunk->parent != NULL)
        IFF_updateChunkSizes((IFF_Chunk*)chunk->parent);
}
