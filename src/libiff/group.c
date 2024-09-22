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
#include "pointerarray.h"
#include "chunksarray.h"

IFF_Group *IFF_createGroup(const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID groupType, IFF_GroupStructure *groupStructure, IFF_ChunkInterface *chunkInterface)
{
    IFF_Group *group;

    if(groupStructure == NULL)
        group = (IFF_Group*)IFF_createChunk(chunkId, chunkSize, sizeof(IFF_Group), chunkInterface);
    else
        group = (IFF_Group*)IFF_createChunk(chunkId, chunkSize, groupStructure->groupSize, chunkInterface);

    if(group != NULL)
    {
        group->groupType = groupType;
        group->chunksLength = 0;
        group->chunks = NULL;
        group->groupStructure = groupStructure;

        IFF_initGroupStructure(group);
    }

    return group;
}

IFF_Group *IFF_createEmptyGroup(const IFF_ID chunkId, const IFF_ID groupType, IFF_GroupStructure *groupStructure, IFF_ChunkInterface *chunkInterface)
{
    return IFF_createGroup(chunkId, IFF_ID_SIZE /* We have a group ID so the minimum size is bigger than 0 */, groupType, groupStructure, chunkInterface);
}

void IFF_attachChunkToGroup(IFF_Group *group, IFF_Chunk *chunk)
{
    chunk->parent = (IFF_Chunk*)group;

    if(!IFF_attachChunkToGroupStructure(group, chunk))
        group->chunks = IFF_addChunkToChunksArray(group->chunks, &group->chunksLength, chunk);
}

void IFF_addChunkToGroup(IFF_Group *group, IFF_Chunk *chunk)
{
    IFF_attachChunkToGroup(group, chunk);
    IFF_increaseChunkSize((IFF_Chunk*)group, chunk);
}

IFF_Chunk *IFF_detachChunkFromGroup(IFF_Group *group, const unsigned int index)
{
    IFF_Chunk *obsoleteChunk = NULL;
    group->chunks = IFF_removeChunkFromChunksArrayByIndex(group->chunks, &group->chunksLength, index, &obsoleteChunk);

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

IFF_Chunk *IFF_replaceChunkInGroup(IFF_Group *group, const unsigned int index, IFF_Chunk *chunk)
{
    IFF_Chunk *obsoleteChunk = IFF_replaceChunkInChunksArrayByIndex(group->chunks, group->chunksLength, index, chunk);

    if(obsoleteChunk != NULL)
        obsoleteChunk->parent = NULL;

    return obsoleteChunk;
}

IFF_Chunk *IFF_updateChunkInGroupByIndex(IFF_Group *group, const unsigned int index, IFF_Chunk *chunk)
{
    IFF_Chunk *obsoleteChunk = IFF_replaceChunkInGroup(group, index, chunk);

    if(obsoleteChunk != NULL)
        IFF_updateChunkSize((IFF_Chunk*)group, obsoleteChunk, chunk);

    return obsoleteChunk;
}

IFF_Chunk **IFF_mergeChunksArrayIntoGroup(IFF_Group *group, IFF_Chunk **baseChunks, const unsigned int baseChunksLength, IFF_Chunk **appendChunks, const unsigned int appendChunksLength, unsigned int *resultChunksLength)
{
    unsigned int i;

    baseChunks = (IFF_Chunk**)realloc(baseChunks, ((baseChunksLength + appendChunksLength) * sizeof(IFF_Chunk*)));

    for(i = 0; i < appendChunksLength; i++)
    {
        IFF_Chunk *appendChunk = appendChunks[i];
        baseChunks[baseChunksLength + i] = appendChunk;
        IFF_increaseChunkSize((IFF_Chunk*)group, appendChunk);
    }

    *resultChunksLength = baseChunksLength + appendChunksLength;

    return baseChunks;
}

static void evaluateGroupSubChunks(IFF_Group *group, IFF_Group *evaluatedGroup)
{
    IFF_Group *parentGroup = (IFF_Group*)IFF_searchEnclosingProp((const IFF_Chunk*)group, group->groupType);

    if(parentGroup != NULL)
        evaluateGroupSubChunks(parentGroup, evaluatedGroup);

    evaluatedGroup->chunks = IFF_mergeChunksArrayIntoGroup(evaluatedGroup, evaluatedGroup->chunks, evaluatedGroup->chunksLength, group->chunks, group->chunksLength, &evaluatedGroup->chunksLength);
    IFF_evaluateGroupStructure(group, evaluatedGroup);
}

IFF_Group *IFF_evaluateGroup(IFF_Group *group)
{
    IFF_Group *evaluatedGroup = IFF_createEmptyGroup(group->chunkId, group->groupType, group->groupStructure, group->chunkInterface);
    evaluateGroupSubChunks(group, evaluatedGroup);
    return evaluatedGroup;
}

void IFF_freeEvaluatedGroup(IFF_Group *evaluatedGroup)
{
    free(evaluatedGroup->chunks);
    IFF_freeEvaluatedGroupStructure(evaluatedGroup);
    free(evaluatedGroup);
}

static IFF_Bool readGroupSubChunks(FILE *file, IFF_Group *group, const IFF_Registry *registry, const IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
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
        chunk = IFF_parseChunk(file, group->groupType, registry, attributePath, error);

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

IFF_Group *IFF_parseGroupContents(FILE *file, IFF_lookupGroupStructureFunction lookupGroupStructure, const IFF_ID chunkId, const IFF_Long chunkSize, char *groupTypeName, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_ID groupType;

    /* Read group type */
    if(IFF_readId(file, &groupType, attributePath, groupTypeName, chunkId, error))
    {
        IFF_GroupStructure *groupStructure = lookupGroupStructure(registry, groupType);
        IFF_Group *group = IFF_createGroup(chunkId, chunkSize, groupType, groupStructure, chunkInterface);

        *bytesProcessed = *bytesProcessed + IFF_ID_SIZE; /* Initially, we have already read the group ID */

        if(group != NULL)
            readGroupSubChunks(file, group, registry, chunkInterface, attributePath, bytesProcessed, error);

        return group;
    }
    else
        return IFF_createGroup(chunkId, chunkSize, 0, NULL, chunkInterface);
}

static IFF_Bool writeGroupSubChunks(FILE *file, const IFF_Group *group, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_visitAttributeByName(attributePath, "chunks");

    if(!IFF_writeChunksArray(file, group->chunks, group->chunksLength, attributePath, bytesProcessed, error))
        return FALSE;

    IFF_unvisitAttribute(attributePath);

    return TRUE;
}

IFF_Bool IFF_writeGroupContents(FILE *file, const IFF_Group *group, char *groupTypeName, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_FieldStatus status;

    if((status = IFF_writeIdField(file, group->groupType, (const IFF_Chunk*)group, attributePath, groupTypeName, bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if(!IFF_writeGroupStructure(file, group, attributePath, bytesProcessed, error))
        return FALSE;

    if(!writeGroupSubChunks(file, group, attributePath, bytesProcessed, error))
        return FALSE;

    return TRUE;
}

static IFF_Long computeActualGroupChunkSize(const IFF_Group *group)
{
    IFF_Long chunkSize = IFF_ID_SIZE; /* Initially, the group chunk already contains a group type */
    chunkSize = IFF_addActualGroupStructureSize(group, chunkSize);
    chunkSize = IFF_addChunksArraySize(group->chunks, group->chunksLength, chunkSize);
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

static IFF_QualityLevel checkGroupSubChunks(const IFF_Group *group, IFF_subChunkCheckFunction subChunkCheck, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    unsigned int i;

    IFF_visitAttributeByName(attributePath, "chunks");

    for(i = 0; i < group->chunksLength; i++)
    {
        IFF_Chunk *subChunk = group->chunks[i];

        IFF_visitAttributeByIndex(attributePath, i);

        qualityLevel = IFF_degradeQualityLevel(qualityLevel, subChunkCheck(group, subChunk, attributePath, printCheckMessage, data));
        qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(subChunk, attributePath, printCheckMessage, data));

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return qualityLevel;
}

IFF_QualityLevel IFF_checkGroupContents(const IFF_Group *group, char *groupTypeName, IFF_groupTypeCheckFunction groupTypeCheck, IFF_subChunkCheckFunction subChunkCheck, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    IFF_Long actualChunkSize;

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, groupTypeCheck(group->groupType, attributePath, groupTypeName, printCheckMessage, data, group->chunkId));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkGroupStructure(group, attributePath, printCheckMessage, data));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, checkGroupSubChunks(group, subChunkCheck, attributePath, printCheckMessage, data));

    actualChunkSize = computeActualGroupChunkSize(group);

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, checkGroupChunkSize(group, actualChunkSize, attributePath, printCheckMessage, data));

    return qualityLevel;
}

void IFF_clearGroupContents(IFF_Group *group)
{
    IFF_freeChunksArray(group->chunks, group->chunksLength);
    IFF_clearGroupStructure(group);
}

static void printGroupType(FILE *file, const char *groupTypeName, const IFF_ID groupType, const unsigned int indentLevel)
{
    IFF_printIdField(file, indentLevel, groupTypeName, groupType);
}

void IFF_printGroupContents(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const char *groupTypeName)
{
    printGroupType(file, groupTypeName, group->groupType, indentLevel);
    IFF_printGroupStructure(file, group, indentLevel);
    fputs(",\n", file);
    IFF_printChunksArrayField(file, indentLevel, "chunks", group->chunks, group->chunksLength);
}

IFF_Bool IFF_compareGroupContents(const IFF_Group *group1, const IFF_Group *group2)
{
    return (group1->groupType == group2->groupType &&
        IFF_compareChunksArray(group1->chunks, group1->chunksLength, group2->chunks, group2->chunksLength) &&
        IFF_compareGroupStructure(group1, group2));
}

IFF_Bool IFF_traverseGroupChunkHierarchy(IFF_Group *group, void *data, IFF_visitChunkFunction visitChunk)
{
    visitChunk((IFF_Chunk*)group, data);

    return IFF_traverseGroupStructureHierarchy(group, data, visitChunk) &&
        IFF_traverseChunksArray(group->chunks, group->chunksLength, data, visitChunk);
}

void IFF_recalculateGroupChunkSize(IFF_Group *group)
{
    group->chunkSize = computeActualGroupChunkSize(group);
}

IFF_Bool IFF_searchLastChunkIndexInGroup(const IFF_Group *group, const IFF_ID chunkId, unsigned int *index)
{
    unsigned int i;

    for(i = group->chunksLength; i-- > 0; )
    {
        if(group->chunks[i]->chunkId == chunkId)
        {
            *index = i;
            return TRUE;
        }
    }

    return FALSE;
}
