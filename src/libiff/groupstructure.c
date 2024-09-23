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

#include "groupstructure.h"
#include <stdlib.h>
#include "list.h"
#include "pointerarray.h"
#include "chunksarray.h"
#include "field.h"
#include "util.h"

static IFF_Chunk *getFieldValue(const IFF_Group *group, const unsigned int index)
{
    IFF_Chunk **chunkPtr = group->groupStructure->getFieldPointer(group, index);
    return *chunkPtr;
}

static IFF_Chunk **getArrayFieldValue(const IFF_Group *group, const unsigned int index, unsigned int *chunksLength)
{
    unsigned int *chunksLengthPtr;
    IFF_Chunk ***chunksPtr = group->groupStructure->getArrayFieldPointer(group, index, &chunksLengthPtr);

    *chunksLength = *chunksLengthPtr;
    return *chunksPtr;
}

IFF_Bool IFF_attachChunkToGroupStructure(IFF_Group *group, IFF_Chunk *chunk)
{
    if(group->groupStructure == NULL)
        return FALSE;
    else
    {
        unsigned int index;

        if(group->groupStructure->mapChunkIdToFieldIndex(chunk->chunkId, &index))
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[index];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk **chunkPointer = group->groupStructure->getFieldPointer(group, index);

                /* If there was already a single member assigned, then put it in the chunks array so that it gets overridden */
                if(*chunkPointer != NULL)
                    group->chunks = IFF_addChunkToChunksArray(group->chunks, &group->chunksLength, *chunkPointer);

                *chunkPointer = chunk;
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int *chunksLength;
                IFF_Chunk ***chunksPointer = group->groupStructure->getArrayFieldPointer(group, index, &chunksLength);
                *chunksPointer = IFF_addChunkToChunksArray(*chunksPointer, chunksLength, chunk);
            }

            return TRUE;
        }
        else
            return FALSE;
    }
}

IFF_Chunk *IFF_updateChunkInGroupStructure(IFF_Group *group, IFF_Chunk *chunk)
{
    unsigned int index;

    if(group->groupStructure->mapChunkIdToFieldIndex(chunk->chunkId, &index))
    {
        IFF_Chunk **obsoleteChunkPtr = group->groupStructure->getFieldPointer(group, index);

        if(obsoleteChunkPtr == NULL)
            return NULL;
        else
        {
            IFF_Chunk *obsoleteChunk = *obsoleteChunkPtr;

            if(obsoleteChunk != NULL)
            {
                obsoleteChunk->parent = NULL;
                IFF_decreaseChunkSize((IFF_Chunk*)group, obsoleteChunk);
            }

            *obsoleteChunkPtr = chunk;
            chunk->parent = (IFF_Chunk*)group;
            IFF_increaseChunkSize((IFF_Chunk*)group, chunk);

            return obsoleteChunk;
        }
    }
    else
        return NULL;
}

IFF_Chunk *IFF_removeChunkFromGroupStructure(IFF_Group *group, const IFF_ID chunkId)
{
    unsigned int index;

    if(group->groupStructure->mapChunkIdToFieldIndex(chunkId, &index))
    {
        IFF_Chunk **obsoleteChunkPtr = group->groupStructure->getFieldPointer(group, index);

        if(obsoleteChunkPtr == NULL)
            return NULL;
        else
        {
            unsigned int index;
            IFF_Chunk *obsoleteChunk = *obsoleteChunkPtr;

            if(obsoleteChunk != NULL)
            {
                obsoleteChunk->parent = NULL;
                IFF_decreaseChunkSize((IFF_Chunk*)group, obsoleteChunk);
            }

            if(IFF_searchLastChunkIndexInGroup((IFF_Group*)group, chunkId, &index))
            {
                IFF_Chunk *previousChunk;
                group->chunks = IFF_removeChunkFromChunksArrayByIndex(group->chunks, &group->chunksLength, index, &previousChunk);
                *obsoleteChunkPtr = previousChunk;
            }
            else
                *obsoleteChunkPtr = NULL;

            return obsoleteChunk;
        }
    }
    else
        return NULL;
}

IFF_Chunk *IFF_updateChunkInGroupStructureByIndex(IFF_Group *group, const unsigned int index, IFF_Chunk *chunk)
{
    unsigned int groupMemberIndex;

    if(group->groupStructure->mapChunkIdToFieldIndex(chunk->chunkId, &groupMemberIndex))
    {
        unsigned int *chunksLength;
        IFF_Chunk ***obsoleteChunkArrayPtr = group->groupStructure->getArrayFieldPointer(group, groupMemberIndex, &chunksLength);

        if(obsoleteChunkArrayPtr == NULL)
            return NULL;
        else
        {
            IFF_Chunk *obsoleteChunk = IFF_replaceChunkInChunksArrayByIndex(*obsoleteChunkArrayPtr, *chunksLength, index, chunk);

            if(obsoleteChunk != NULL)
            {
                obsoleteChunk->parent = NULL;
                IFF_decreaseChunkSize((IFF_Chunk*)group, obsoleteChunk);
            }

            IFF_increaseChunkSize((IFF_Chunk*)group, chunk);

            return obsoleteChunk;
        }
    }
    else
        return NULL;
}

IFF_Chunk *IFF_removeChunkFromGroupStructureByIndex(IFF_Group *group, const IFF_ID chunkId, const unsigned int index)
{
    unsigned int groupMemberIndex;

    if(group->groupStructure->mapChunkIdToFieldIndex(chunkId, &groupMemberIndex))
    {
        unsigned int *chunksLength;
        IFF_Chunk ***obsoleteChunkArrayPtr = group->groupStructure->getArrayFieldPointer(group, groupMemberIndex, &chunksLength);

        if(obsoleteChunkArrayPtr == NULL)
            return NULL;
        else
        {
            IFF_Chunk *obsoleteChunk;
            *obsoleteChunkArrayPtr = IFF_removeChunkFromChunksArrayByIndex(*obsoleteChunkArrayPtr, chunksLength, index, &obsoleteChunk);

            if(obsoleteChunk != NULL)
            {
                obsoleteChunk->parent = NULL;
                IFF_decreaseChunkSize((IFF_Chunk*)group, obsoleteChunk);
            }

            return obsoleteChunk;
        }
    }
    else
        return NULL;
}

IFF_Chunk *IFF_getPropertyFromGroupStructure(const IFF_Group *group, const unsigned int index)
{
    if(group->groupStructure == NULL)
        return NULL;
    else
    {
        IFF_Chunk **chunkPtr = group->groupStructure->getFieldPointer(group, index);

        if(chunkPtr == NULL)
            return NULL;
        else
        {
            if(*chunkPtr == NULL)
            {
                IFF_Group *parentGroup = (IFF_Group*)IFF_searchEnclosingProp((IFF_Chunk*)group, group->groupType);

                if(parentGroup == NULL)
                    return NULL;
                else
                    return IFF_getPropertyFromGroupStructure(parentGroup, index);
            }
            else
                return *chunkPtr;
        }
    }
}

static IFF_Chunk **appendPropertiesToResult(const IFF_Group *group, const unsigned int index, IFF_Chunk **properties, unsigned int *propertiesLength)
{
    IFF_Group *parentGroup = (IFF_Group*)IFF_searchEnclosingProp((const IFF_Chunk*)group, group->groupType);

    if(parentGroup != NULL)
        properties = appendPropertiesToResult(parentGroup, index, properties, propertiesLength);

    if(group->groupStructure == NULL)
        return properties;
    else
    {
        unsigned int *appendPropertiesLengthPtr;
        IFF_Chunk ***appendPropertiesPtr = group->groupStructure->getArrayFieldPointer(group, index, &appendPropertiesLengthPtr);

        if(**appendPropertiesPtr == NULL)
            return properties;
        else
            return IFF_appendChunksArrayToChunksArray(properties, *propertiesLength, *appendPropertiesPtr, *appendPropertiesLengthPtr, propertiesLength);
    }
}

IFF_Chunk **IFF_getPropertiesFromGroupStructure(const IFF_Group *group, const unsigned int index, unsigned int *propertiesLength)
{
    *propertiesLength = 0;
    return appendPropertiesToResult(group, index, NULL, propertiesLength);
}

void IFF_evaluateGroupStructure(const IFF_Group *group, IFF_Group *evaluatedGroup)
{
    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = getFieldValue(group, i);
                IFF_Chunk **targetChunk = evaluatedGroup->groupStructure->getFieldPointer(evaluatedGroup, i);

                *targetChunk = chunk;
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int appendChunksLength;
                IFF_Chunk **appendChunks = getArrayFieldValue(group, i, &appendChunksLength);
                unsigned int *baseChunksLength;
                IFF_Chunk ***baseChunks = evaluatedGroup->groupStructure->getArrayFieldPointer(evaluatedGroup, i, &baseChunksLength);

                *baseChunks = IFF_mergeChunksArrayIntoGroup(evaluatedGroup, *baseChunks, *baseChunksLength, appendChunks, appendChunksLength, baseChunksLength);
            }
        }
    }
}

void IFF_freeEvaluatedGroupStructure(IFF_Group *evaluatedGroup)
{
    if(evaluatedGroup->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < evaluatedGroup->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &evaluatedGroup->groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = getArrayFieldValue(evaluatedGroup, i, &chunksLength);
                free(chunks);
            }
        }
    }
}

void IFF_initGroupStructure(IFF_Group *group)
{
    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk **chunkPointer = group->groupStructure->getFieldPointer(group, i);
                *chunkPointer = NULL;
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int *chunksLength;
                IFF_Chunk ***chunksPointer = group->groupStructure->getArrayFieldPointer(group, i, &chunksLength);

                *chunksPointer = NULL;
                *chunksLength = 0;
            }
        }
    }
}

IFF_Bool IFF_writeGroupStructure(FILE *file, const IFF_Group *group, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            IFF_visitAttributeByName(attributePath, groupMember->attributeName);

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = getFieldValue(group, i);

                if(chunk != NULL)
                {
                    if(!IFF_writeChunk(file, chunk, attributePath, error))
                        return FALSE;

                    /* Increase the bytes processed counter */
                    *bytesProcessed = IFF_addChunkSize(*bytesProcessed, chunk);
                }
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = getArrayFieldValue(group, i, &chunksLength);

                if(!IFF_writeChunksArray(file, chunks, chunksLength, attributePath, bytesProcessed, error))
                    return FALSE;
            }

            IFF_unvisitAttribute(attributePath);
        }
    }

    return TRUE;
}

IFF_Long IFF_addActualGroupStructureSize(const IFF_Group *group, IFF_Long chunkSize)
{
    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = getFieldValue(group, i);

                if(chunk != NULL)
                    chunkSize = IFF_addChunkSize(chunkSize, chunk);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = getArrayFieldValue(group, i, &chunksLength);

                chunkSize = IFF_addChunksArraySize(chunks, chunksLength, chunkSize);
            }
        }
    }

    return chunkSize;
}

IFF_QualityLevel IFF_checkGroupStructure(const IFF_Group *group, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;

    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            IFF_visitAttributeByName(attributePath, groupMember->attributeName);

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = getFieldValue(group, i);

                if(chunk != NULL)
                    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(chunk, attributePath, printCheckMessage, data));
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = getArrayFieldValue(group, i, &chunksLength);

                qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunksArray(chunks, chunksLength, attributePath, printCheckMessage, data));
            }

            IFF_unvisitAttribute(attributePath);
        }
    }

    return qualityLevel;
}

void IFF_clearGroupStructure(IFF_Group *group)
{
    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = getFieldValue(group, i);

                if(chunk != NULL)
                    IFF_freeChunk(chunk);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = getArrayFieldValue(group, i, &chunksLength);

                IFF_freeChunksArray(chunks, chunksLength);
            }
        }
    }
}

void IFF_printGroupStructure(FILE *file, const IFF_Group *group, const unsigned int indentLevel)
{
    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            if(i > 0)
                fprintf(file, ",");

            fprintf(file, "\n");

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = getFieldValue(group, i);
                IFF_printChunkField(file, indentLevel, groupMember->attributeName, chunk);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = getArrayFieldValue(group, i, &chunksLength);

                IFF_printChunksArrayField(file, indentLevel, groupMember->attributeName, chunks, chunksLength);
            }
        }
    }
}

IFF_Bool IFF_compareGroupStructure(const IFF_Group *group1, const IFF_Group *group2)
{
    if(group1->groupStructure != group2->groupStructure)
        return FALSE;
    else if(group1->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group1->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group1->groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk1 = getFieldValue(group1, i);
                IFF_Chunk *chunk2 = getFieldValue(group2, i);

                if(chunk1 != NULL && chunk2 != NULL)
                {
                    if(!IFF_compareChunk(chunk1, chunk2))
                        return FALSE;
                }
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunks1Length, chunks2Length;
                IFF_Chunk **chunks1 = getArrayFieldValue(group1, i, &chunks1Length);
                IFF_Chunk **chunks2 = getArrayFieldValue(group2, i, &chunks2Length);

                if(!IFF_compareChunksArray(chunks1, chunks1Length, chunks2, chunks2Length))
                    return FALSE;
            }
        }
    }

    return TRUE;
}

IFF_Bool IFF_traverseGroupStructureHierarchy(const IFF_Group *group, void *data, IFF_visitChunkFunction visitChunk)
{
    if(group->groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < group->groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &group->groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = getFieldValue(group, i);

                if(chunk != NULL)
                {
                    if(!IFF_traverseChunkHierarchy(chunk, data, visitChunk))
                        return FALSE;
                }
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = getArrayFieldValue(group, i, &chunksLength);

                if(!IFF_traverseChunksArray(chunks, chunksLength, data, visitChunk))
                    return FALSE;
            }
        }
    }

    return TRUE;
}
