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
#include "array.h"

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

void IFF_attachChunkToGroup(IFF_Group *group, IFF_Chunk *chunk)
{
    chunk->parent = (IFF_Chunk*)group;
    group->chunks = (IFF_Chunk**)IFF_addElementToPointerArray((void**)group->chunks, (void*)chunk, &group->chunksLength);
}

void IFF_addChunkToGroup(IFF_Group *group, IFF_Chunk *chunk)
{
    IFF_attachChunkToGroup(group, chunk);
    IFF_increaseChunkSize((IFF_Chunk*)group, chunk);
}

IFF_Chunk *IFF_detachChunkFromGroup(IFF_Group *group, const unsigned int index)
{
    IFF_Chunk *obsoleteChunk = NULL;
    group->chunks = (IFF_Chunk**)IFF_removeElementFromPointerArrayByIndex((void**)group->chunks, index, &group->chunksLength, (void**)&obsoleteChunk);

    if(obsoleteChunk != NULL)
        obsoleteChunk->parent = NULL;

    return obsoleteChunk;
}

IFF_Chunk *IFF_removeChunkFromGroupByIndex(IFF_Group *group, const unsigned int index)
{
    IFF_Chunk *obsoleteChunk = IFF_detachChunkFromGroup(group, index);

    if(obsoleteChunk != NULL)
        IFF_decreaseChunkSize((IFF_Chunk*)group, obsoleteChunk);

    return obsoleteChunk;
}

IFF_Chunk *IFF_replaceInGroup(IFF_Group *group, const unsigned int index, IFF_Chunk *chunk)
{
    IFF_Chunk *obsoleteChunk = (IFF_Chunk*)IFF_replaceElementInPointerArrayByIndex((void**)group->chunks, group->chunksLength, index, (void*)chunk);

    if(obsoleteChunk != NULL)
        obsoleteChunk->parent = NULL;

    return obsoleteChunk;
}

IFF_Chunk *IFF_updateChunkInGroupByIndex(IFF_Group *group, const unsigned int index, IFF_Chunk *chunk)
{
    IFF_Chunk *obsoleteChunk = IFF_replaceInGroup(group, index, chunk);

    if(obsoleteChunk != NULL)
        IFF_updateChunkSize((IFF_Chunk*)group, obsoleteChunk, chunk);

    return obsoleteChunk;
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
        IFF_attachChunkToGroup(group, chunk);

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_addChunkSize(*bytesProcessed, chunk);
        index++;

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return TRUE;
}

IFF_Chunk *IFF_parseGroupContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_Group *group = IFF_createGroup(chunkId, chunkSize, 0);

    /* Read group type */
    if(IFF_readIdField(file, &group->groupType, (IFF_Chunk*)group, attributePath, groupTypeName, bytesProcessed, error) == IFF_FIELD_MORE &&
        readGroupSubChunks(file, group, chunkRegistry, attributePath, bytesProcessed, error))
        ;

    return (IFF_Chunk*)group;
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
        *bytesProcessed = IFF_addChunkSize(*bytesProcessed, group->chunks[i]);

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
        chunkSize = IFF_addChunkSize(chunkSize, subChunk);
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

        qualityLevel = IFF_degradeQualityLevel(qualityLevel, subChunkCheck(group, subChunk, attributePath, printCheckMessage, data));
        qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(subChunk, group->groupType, chunkRegistry, attributePath, printCheckMessage, data));

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return qualityLevel;
}

IFF_QualityLevel IFF_checkGroupContents(const IFF_Group *group, char *groupTypeName, IFF_groupTypeCheckFunction groupTypeCheck, IFF_subChunkCheckFunction subChunkCheck, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    IFF_Long actualChunkSize;

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, groupTypeCheck(group->groupType, attributePath, groupTypeName, printCheckMessage, data, group->chunkId));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkGroupSubChunks(group, subChunkCheck, chunkRegistry, attributePath, printCheckMessage, data));

    actualChunkSize = IFF_computeActualGroupChunkSize(group);

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkGroupChunkSize(group, actualChunkSize, attributePath, printCheckMessage, data));

    return qualityLevel;
}

void IFF_clearGroupContents(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_Group *group = (IFF_Group*)chunk;
    unsigned int i;

    for(i = 0; i < group->chunksLength; i++)
        IFF_freeChunk(group->chunks[i], group->groupType, chunkRegistry);

    free(group->chunks);
}

void IFF_printGroupType(FILE *file, const char *groupTypeName, const IFF_ID groupType, const unsigned int indentLevel)
{
    IFF_printIdField(file, indentLevel, groupTypeName, groupType);
}

void IFF_printChunksArray(FILE *file, const void *value, const unsigned int indentLevel)
{
    const IFF_PrintChunksArrayParameter *param = (const IFF_PrintChunksArrayParameter*)value;
    unsigned int i;

    fputs("{\n", file);

    for(i = 0; i < param->chunksLength; i++)
    {
        if(i > 0)
            fputs(",\n", file);

        IFF_printChunk(file, (const IFF_Chunk*)param->chunks[i], indentLevel + 1, param->groupType, param->chunkRegistry);
    }

    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

void IFF_printGroupSubChunks(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_PrintChunksArrayParameter param;
    param.groupType = group->groupType;
    param.chunksLength = group->chunksLength;
    param.chunks = group->chunks;
    param.chunkRegistry = chunkRegistry;

    IFF_printField(file, indentLevel, "chunks", &param, IFF_printChunksArray);
}

void IFF_printGroupContents(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroupType(file, groupTypeName, group->groupType, indentLevel);
    IFF_printGroupSubChunks(file, group, indentLevel, chunkRegistry);
}

IFF_Bool IFF_compareGroupContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_Group *group1 = (const IFF_Group*)chunk1;
    const IFF_Group *group2 = (const IFF_Group*)chunk2;

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

IFF_Bool IFF_traverseGroupChunkHierarchy(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_Group *group = (IFF_Group*)chunk;
    unsigned int i;

    visitChunk(chunk, data);

    /* Visit sub chunks */
    for(i = 0; i < group->chunksLength; i++)
    {
        if(!IFF_traverseChunkHierarchy(group->chunks[i], group->groupType, data, visitChunk, chunkRegistry))
            return FALSE;
    }

    return TRUE;
}

void IFF_recalculateGroupChunkSize(IFF_Chunk *chunk)
{
    IFF_Group *group = (IFF_Group*)chunk;
    unsigned int i;

    group->chunkSize = IFF_ID_SIZE;

    for(i = 0; i < group->chunksLength; i++)
        group->chunkSize = IFF_addChunkSize(group->chunkSize, group->chunks[i]);
}
