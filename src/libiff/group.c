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
#include "io.h"
#include "form.h"
#include "cat.h"
#include "list.h"
#include "prop.h"
#include "field.h"
#include "util.h"

void IFF_initGroup(IFF_Group *group, const IFF_ID groupType)
{
    group->groupType = groupType;
    group->chunksLength = 0;
    group->chunks = NULL;
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
    group->chunks = (IFF_Chunk**)realloc(group->chunks, (group->chunksLength + 1) * sizeof(IFF_Chunk*));
    group->chunks[group->chunksLength] = chunk;
    group->chunksLength++;
    chunk->parent = group;
}

void IFF_addToGroup(IFF_Group *group, IFF_Chunk *chunk)
{
    IFF_attachToGroup(group, chunk);
    group->chunkSize = IFF_incrementChunkSize(group->chunkSize, chunk);
}

static IFF_Bool readGroupSubChunks(FILE *file, IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int index = 0;

    IFF_visitAttributeByName(attributePath, "chunks");

    while(*bytesProcessed < group->chunkSize)
    {
        IFF_Chunk *chunk;

        IFF_visitAttributeByIndex(attributePath, index);

        /* Read sub chunk */
        chunk = IFF_readChunk(file, group->groupType, chunkRegistry, attributePath, error);

        if(chunk == NULL)
            return FALSE;

        /* Attach chunk to the group */
        IFF_attachToGroup(group, chunk);

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, chunk);
        index++;

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return TRUE;
}

IFF_Bool IFF_readGroupContents(FILE *file, IFF_Chunk *chunk, char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_Group *group = (IFF_Group*)chunk;
    IFF_FieldStatus status;

    /* Read group type */
    if((status = IFF_readIdField(file, &group->groupType, chunk, attributePath, groupTypeName, bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    /* Keep parsing sub chunks until we have read all bytes */
    if(!readGroupSubChunks(file, group, chunkRegistry, attributePath, bytesProcessed, error))
        return FALSE;

    return TRUE;
}

IFF_Bool IFF_writeGroupSubChunks(FILE *file, const IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int i;

    IFF_visitAttributeByName(attributePath, "chunks");

    for(i = 0; i < group->chunksLength; i++)
    {
        IFF_visitAttributeByIndex(attributePath, i);

        if(!IFF_writeChunk(file, group->chunks[i], group->groupType, chunkRegistry, attributePath, error))
            return FALSE;

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, group->chunks[i]);

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return TRUE;
}

IFF_Bool IFF_writeGroupContents(FILE *file, const IFF_Chunk *chunk, char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    const IFF_Group *group = (const IFF_Group*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeIdField(file, group->groupType, chunk, attributePath, groupTypeName, bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if(!IFF_writeGroupSubChunks(file, group, chunkRegistry, attributePath, bytesProcessed, error))
        return FALSE;

    return TRUE;
}

IFF_Long IFF_computeActualGroupChunkSize(const IFF_Group *group)
{
    unsigned int i;
    IFF_Long chunkSize = IFF_ID_SIZE; /* Initially, the group chunk already contains a group type */

    for(i = 0; i < group->chunksLength; i++)
    {
        IFF_Chunk *subChunk = group->chunks[i];
        chunkSize = IFF_incrementChunkSize(chunkSize, subChunk);
    }

    return chunkSize;
}

IFF_QualityLevel IFF_checkGroupChunkSize(const IFF_Group *group, const IFF_Long actualChunkSize, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    if(actualChunkSize == group->chunkSize)
        return IFF_QUALITY_PERFECT;
    else
    {
        printCheckMessage(attributePath, "chunkSize", group->chunkId, data, "does not match the sum of its sub chunks: %d, value is: %d", actualChunkSize, group->chunkSize);
        return IFF_QUALITY_RECOVERED;
    }
}

IFF_QualityLevel IFF_checkGroupSubChunks(const IFF_Group *group, IFF_subChunkCheckFunction subChunkCheck, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    unsigned int i;

    IFF_visitAttributeByName(attributePath, "chunks");

    for(i = 0; i < group->chunksLength; i++)
    {
        IFF_Chunk *subChunk = group->chunks[i];

        IFF_visitAttributeByIndex(attributePath, i);

        qualityLevel = IFF_adjustQualityLevel(qualityLevel, subChunkCheck(group, subChunk, attributePath, printCheckMessage, data));
        qualityLevel = IFF_adjustQualityLevel(qualityLevel, IFF_checkChunk(subChunk, group->groupType, chunkRegistry, attributePath, printCheckMessage, data));

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return qualityLevel;
}

IFF_QualityLevel IFF_checkGroupContents(const IFF_Group *group, char *groupTypeName, IFF_groupTypeCheckFunction groupTypeCheck, IFF_subChunkCheckFunction subChunkCheck, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    IFF_Long actualChunkSize;

    qualityLevel = IFF_adjustQualityLevel(qualityLevel, groupTypeCheck(group->groupType, attributePath, groupTypeName, printCheckMessage, data, group->chunkId));
    qualityLevel = IFF_adjustQualityLevel(qualityLevel, IFF_checkGroupSubChunks(group, subChunkCheck, chunkRegistry, attributePath, printCheckMessage, data));

    actualChunkSize = IFF_computeActualGroupChunkSize(group);

    qualityLevel = IFF_adjustQualityLevel(qualityLevel, IFF_checkGroupChunkSize(group, actualChunkSize, attributePath, printCheckMessage, data));

    return qualityLevel;
}

void IFF_clearGroupContents(IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    for(i = 0; i < group->chunksLength; i++)
        IFF_freeChunk(group->chunks[i], group->groupType, chunkRegistry);

    free(group->chunks);
}

void IFF_printGroupType(FILE *file, const char *groupTypeName, const IFF_ID groupType, const unsigned int indentLevel)
{
    IFF_printIdField(file, indentLevel, groupTypeName, groupType);
}

void IFF_printGroupSubChunks(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    IFF_printIndent(file, indentLevel, ".chunks = {\n");

    for(i = 0; i < group->chunksLength; i++)
    {
        if(i > 0)
            fprintf(file, ",\n");

        IFF_printChunk(file, (IFF_Chunk*)group->chunks[i], indentLevel + 1, group->groupType, chunkRegistry);
    }

    fprintf(file, "\n");

    IFF_printIndent(file, indentLevel, "},\n");
}

void IFF_printGroupContents(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroupType(file, groupTypeName, group->groupType, indentLevel);
    IFF_printGroupSubChunks(file, group, indentLevel, chunkRegistry);
}

IFF_Bool IFF_compareGroupContents(const IFF_Group *group1, const IFF_Group *group2, const IFF_ChunkRegistry *chunkRegistry)
{
    if(group1->groupType == group2->groupType && group1->chunksLength == group2->chunksLength)
    {
        unsigned int i;

        for(i = 0; i < group1->chunksLength; i++)
        {
            if(!IFF_compareChunk(group1->chunks[i], group2->chunks[i], group1->groupType, chunkRegistry))
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

    for(i = 0; i < group->chunksLength; i++)
    {
        unsigned int resultLength;
        IFF_Form **result = IFF_searchFormsFromArray(group->chunks[i], formTypes, formTypesLength, &resultLength);

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

    for(i = 0; i < group->chunksLength; i++)
        group->chunkSize = IFF_incrementChunkSize(group->chunkSize, group->chunks[i]);
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
