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

IFF_Group *IFF_createGroup(const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID groupType, const IFF_GroupStructure *groupStructure)
{
    IFF_Group *group;

    if(groupStructure == NULL)
        group = (IFF_Group*)IFF_createChunk(chunkId, chunkSize, sizeof(IFF_Group));
    else
        group = (IFF_Group*)IFF_createChunk(chunkId, chunkSize, groupStructure->groupSize);

    if(group != NULL)
    {
        group->groupType = groupType;
        group->chunksLength = 0;
        group->chunks = NULL;

        if(groupStructure != NULL)
            groupStructure->initGroupContents(group);
    }

    return group;
}

IFF_Group *IFF_createEmptyGroup(const IFF_ID chunkId, const IFF_ID groupType, const IFF_GroupStructure *groupStructure)
{
    return IFF_createGroup(chunkId, IFF_ID_SIZE /* We have a group ID so the minimum size is bigger than 0 */, groupType, groupStructure);
}

void IFF_attachChunkToGroup(IFF_Group *group, const IFF_GroupStructure *groupStructure, IFF_Chunk *chunk)
{
    chunk->parent = (IFF_Chunk*)group;

    if(groupStructure == NULL || !groupStructure->attachChunkToGroup(group, chunk))
        group->chunks = (IFF_Chunk**)IFF_addElementToPointerArray((void**)group->chunks, (void*)chunk, &group->chunksLength);
}

void IFF_addChunkToGroup(IFF_Group *group, const IFF_GroupStructure *groupStructure, IFF_Chunk *chunk)
{
    IFF_attachChunkToGroup(group, groupStructure, chunk);
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

static IFF_Bool readGroupSubChunks(FILE *file, IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int index = 0;

    /*
     * We use a generic attribute names. We only know the chunks' exact scope
     * after it has been successfully read. If it failed, then we can not determine it.
     */

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
        IFF_attachChunkToGroup(group, groupStructure, chunk);

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_addChunkSize(*bytesProcessed, chunk);
        index++;

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return TRUE;
}

IFF_Group *IFF_parseGroupContents(FILE *file, IFF_lookupGroupStructureFunction lookupGroupStructure, const IFF_ID chunkId, const IFF_Long chunkSize, char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_ID groupType;

    /* Read group type */
    if(IFF_readId(file, &groupType, attributePath, groupTypeName, chunkId, error))
    {
        IFF_GroupStructure *groupStructure = lookupGroupStructure(chunkRegistry, groupType);
        IFF_Group *group = IFF_createGroup(chunkId, chunkSize, groupType, groupStructure);

        *bytesProcessed = *bytesProcessed + IFF_ID_SIZE; /* Initially, we have already read the group ID */

        if(group != NULL)
            readGroupSubChunks(file, group, groupStructure, chunkRegistry, attributePath, bytesProcessed, error);

        return group;
    }
    else
        return IFF_createGroup(chunkId, chunkSize, 0, NULL);
}

IFF_Bool IFF_writeGroupSubChunks(FILE *file, const IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
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

IFF_Bool IFF_writeGroupContents(FILE *file, const IFF_Group *group, const IFF_GroupStructure *groupStructure, char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_FieldStatus status;

    if((status = IFF_writeIdField(file, group->groupType, (const IFF_Chunk*)group, attributePath, groupTypeName, bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if(!IFF_writeGroupStructure(file, group, groupStructure, chunkRegistry, attributePath, bytesProcessed, error))
        return FALSE;

    if(!IFF_writeGroupSubChunks(file, group, groupStructure, chunkRegistry, attributePath, bytesProcessed, error))
        return FALSE;

    return TRUE;
}

static IFF_Long computeActualGroupChunkSize(const IFF_Group *group, const IFF_GroupStructure *groupStructure)
{
    unsigned int i;
    IFF_Long chunkSize = IFF_ID_SIZE; /* Initially, the group chunk already contains a group type */

    /* Check group structure */
    chunkSize = IFF_addActualGroupStructureSize(group, groupStructure, chunkSize);

    /* Check arbitrary chunks */

    for(i = 0; i < group->chunksLength; i++)
    {
        IFF_Chunk *subChunk = group->chunks[i];
        chunkSize = IFF_addChunkSize(chunkSize, subChunk);
    }

    return chunkSize;
}

static IFF_QualityLevel checkGroupChunkSize(const IFF_Group *group, const IFF_Long actualChunkSize, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    if(actualChunkSize == group->chunkSize)
        return IFF_QUALITY_PERFECT;
    else
    {
        printCheckMessage(attributePath, "chunkSize", group->chunkId, data, "does not match the sum of its sub chunks: %d, value is: %d", actualChunkSize, group->chunkSize);
        return IFF_QUALITY_RECOVERED;
    }
}

static IFF_QualityLevel checkGroupSubChunks(const IFF_Group *group, IFF_subChunkCheckFunction subChunkCheck, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
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

IFF_QualityLevel IFF_checkGroupContents(const IFF_Group *group, const IFF_GroupStructure *groupStructure, char *groupTypeName, IFF_groupTypeCheckFunction groupTypeCheck, IFF_subChunkCheckFunction subChunkCheck, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    IFF_Long actualChunkSize;

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, groupTypeCheck(group->groupType, attributePath, groupTypeName, printCheckMessage, data, group->chunkId));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkGroupStructure(group, groupStructure, chunkRegistry, attributePath, printCheckMessage, data));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, checkGroupSubChunks(group, subChunkCheck, chunkRegistry, attributePath, printCheckMessage, data));

    actualChunkSize = computeActualGroupChunkSize(group, groupStructure);

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, checkGroupChunkSize(group, actualChunkSize, attributePath, printCheckMessage, data));

    return qualityLevel;
}

static void clearGroupSubChunks(IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    for(i = 0; i < group->chunksLength; i++)
        IFF_freeChunk(group->chunks[i], group->groupType, chunkRegistry);

    free(group->chunks);
}

void IFF_clearGroupContents(IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry)
{
    clearGroupSubChunks(group, chunkRegistry);
    IFF_clearGroupStructure(group, groupStructure, chunkRegistry);
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

        IFF_printIndent(file, indentLevel + 1, "");
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

void IFF_printGroupContents(FILE *file, const IFF_Group *group, const IFF_GroupStructure *groupStructure, const unsigned int indentLevel, const char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroupType(file, groupTypeName, group->groupType, indentLevel);
    IFF_printGroupStructure(file, group, groupStructure, indentLevel, chunkRegistry);
    IFF_printGroupSubChunks(file, group, indentLevel, chunkRegistry);
}

static IFF_Bool compareGroupSubChunks(const IFF_Group *group1, const IFF_Group *group2, const IFF_ChunkRegistry *chunkRegistry)
{
    if(group1->chunksLength == group2->chunksLength)
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

IFF_Bool IFF_compareGroupContents(const IFF_Group *group1, const IFF_Group *group2, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry)
{
    return (group1->groupType == group2->groupType &&
        compareGroupSubChunks(group1, group2, chunkRegistry) &&
        IFF_compareGroupStructure(group1, group2, groupStructure, chunkRegistry));
}

static IFF_Bool traverseGroupSubChunkHierarchy(IFF_Group *group, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    for(i = 0; i < group->chunksLength; i++)
    {
        if(!IFF_traverseChunkHierarchy(group->chunks[i], group->groupType, data, visitChunk, chunkRegistry))
            return FALSE;
    }

    return TRUE;
}

IFF_Bool IFF_traverseGroupChunkHierarchy(IFF_Group *group, const IFF_GroupStructure *groupStructure, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry)
{
    visitChunk((IFF_Chunk*)group, data);

    return (IFF_traverseGroupStructureHierarchy(group, groupStructure, data, visitChunk, chunkRegistry) &&
        traverseGroupSubChunkHierarchy(group, data, visitChunk, chunkRegistry));
}

static IFF_Long addGroupSubChunksSize(IFF_Group *group, IFF_Long chunkSize)
{
    unsigned int i;

    for(i = 0; i < group->chunksLength; i++)
        chunkSize = IFF_addChunkSize(chunkSize, group->chunks[i]);

    return chunkSize;
}

void IFF_recalculateGroupChunkSize(IFF_Group *group, const IFF_GroupStructure *groupStructure)
{
    group->chunkSize = IFF_ID_SIZE;
    group->chunkSize = IFF_addActualGroupStructureSize(group, groupStructure, group->chunkSize);
    group->chunkSize = addGroupSubChunksSize(group, group->chunkSize);
}
