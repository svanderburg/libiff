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
#include <array.h>
#include <util.h>

IFF_Bool IFF_attachChunkToGroupStructure(IFF_Group *group, IFF_Chunk *chunk)
{
    IFF_GroupMember *groupMember;

    if(group->groupStructure == NULL || (groupMember = group->groupStructure->getGroupMemberByChunkId(group->groupStructure, chunk->chunkId)) == NULL)
        return FALSE;
    else
    {
        if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
        {
            IFF_Chunk **chunkPointer = group->groupStructure->getFieldPointerByChunkId(group, chunk->chunkId);

            /* If there was already a single member assigned, then put it in the chunks array so that it gets overridden */
            if(*chunkPointer != NULL)
                group->chunks = (IFF_Chunk**)IFF_addElementToPointerArray((void**)group->chunks, (void*)*chunkPointer, &group->chunksLength);

            *chunkPointer = chunk;
        }
        else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
        {
            unsigned int *chunksLength;
            IFF_Chunk ***chunksPointer = group->groupStructure->getArrayFieldPointerByChunkId(group, chunk->chunkId, &chunksLength);
            *chunksPointer = (IFF_Chunk**)IFF_addElementToPointerArray((void**)*chunksPointer, (void*)chunk, chunksLength);
        }

        return TRUE;
    }
}

IFF_Chunk *IFF_updateChunkInGroupStructure(IFF_Group *group, IFF_Chunk *chunk)
{
    IFF_Chunk **obsoleteChunkPtr = group->groupStructure->getFieldPointerByChunkId(group, chunk->chunkId);

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

IFF_Chunk *IFF_removeChunkFromGroupStructure(IFF_Group *group, const IFF_ID chunkId)
{
    IFF_Chunk **obsoleteChunkPtr = group->groupStructure->getFieldPointerByChunkId(group, chunkId);

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
            group->chunks = (IFF_Chunk**)IFF_removeElementFromPointerArrayByIndex((void**)group->chunks, index, &group->chunksLength, (void**)&previousChunk);
            *obsoleteChunkPtr = previousChunk;
        }
        else
            *obsoleteChunkPtr = NULL;

        return obsoleteChunk;
    }
}

IFF_Chunk *IFF_updateChunkInGroupStructureByIndex(IFF_Group *group, const unsigned int index, IFF_Chunk *chunk)
{
    unsigned int *chunksLength;
    IFF_Chunk ***obsoleteChunkArrayPtr = group->groupStructure->getArrayFieldPointerByChunkId(group, chunk->chunkId, &chunksLength);

    if(obsoleteChunkArrayPtr == NULL)
        return NULL;
    else
    {
        IFF_Chunk *obsoleteChunk = (IFF_Chunk*)IFF_replaceElementInPointerArrayByIndex((void**)*obsoleteChunkArrayPtr, *chunksLength, index, (void*)chunk);

        if(obsoleteChunk != NULL)
        {
            obsoleteChunk->parent = NULL;
            IFF_decreaseChunkSize((IFF_Chunk*)group, obsoleteChunk);
        }

        IFF_increaseChunkSize((IFF_Chunk*)group, chunk);

        return obsoleteChunk;
    }
}

IFF_Chunk *IFF_removeChunkFromGroupStructureByIndex(IFF_Group *group, const IFF_ID chunkId, const unsigned int index)
{
    unsigned int *chunksLength;
    IFF_Chunk ***obsoleteChunkArrayPtr = group->groupStructure->getArrayFieldPointerByChunkId(group, chunkId, &chunksLength);

    if(obsoleteChunkArrayPtr == NULL)
        return NULL;
    else
    {
        IFF_Chunk *obsoleteChunk;
        *obsoleteChunkArrayPtr = (IFF_Chunk**)IFF_removeElementFromPointerArrayByIndex((void**)*obsoleteChunkArrayPtr, index, chunksLength, (void**)&obsoleteChunk);

        if(obsoleteChunk != NULL)
        {
            obsoleteChunk->parent = NULL;
            IFF_decreaseChunkSize((IFF_Chunk*)group, obsoleteChunk);
        }

        return obsoleteChunk;
    }
}

IFF_Chunk *IFF_getPropertyFromGroupStructure(const IFF_Group *group, const unsigned int index)
{
    if(group->groupStructure == NULL)
        return NULL;
    else
    {
        IFF_Chunk *chunk = group->groupStructure->getChunkFromGroup(group, index);

        if(chunk == NULL)
        {
            IFF_Group *parentGroup = (IFF_Group*)IFF_searchEnclosingProp((IFF_Chunk*)group, group->groupType);

            if(parentGroup == NULL)
                return NULL;
            else
                return IFF_getPropertyFromGroupStructure(parentGroup, index);
        }
        else
            return chunk;
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
        unsigned int appendPropertiesLength;
        IFF_Chunk **appendProperties = group->groupStructure->getChunksFromGroup(group, index, &appendPropertiesLength);

        return (IFF_Chunk**)IFF_appendPointerArrayToPointerArray((void**)properties, *propertiesLength, (void**)appendProperties, appendPropertiesLength, propertiesLength);
    }
}

IFF_Chunk **IFF_getPropertiesFromGroupStructure(const IFF_Group *group, const unsigned int index, unsigned int *propertiesLength)
{
    *propertiesLength = 0;
    return appendPropertiesToResult(group, index, NULL, propertiesLength);
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
                IFF_Chunk *chunk = group->groupStructure->getChunkFromGroup(group, i);

                if(chunk != NULL)
                {
                    if(!IFF_writeChunk(file, chunk, group->groupType, attributePath, error))
                        return FALSE;

                    /* Increase the bytes processed counter */
                    *bytesProcessed = IFF_addChunkSize(*bytesProcessed, chunk);
                }
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = group->groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                {
                    IFF_visitAttributeByIndex(attributePath, j);

                    if(!IFF_writeChunk(file, chunks[j], group->groupType, attributePath, error))
                        return FALSE;

                    /* Increase the bytes processed counter */
                    *bytesProcessed = IFF_addChunkSize(*bytesProcessed, chunks[j]);

                    IFF_unvisitAttribute(attributePath);
                }
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
                IFF_Chunk *chunk = group->groupStructure->getChunkFromGroup(group, i);

                if(chunk != NULL)
                    chunkSize = IFF_addChunkSize(chunkSize, chunk);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = group->groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                    chunkSize = IFF_addChunkSize(chunkSize, chunks[j]);
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
                IFF_Chunk *chunk = group->groupStructure->getChunkFromGroup(group, i);

                if(chunk != NULL)
                    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(chunk, group->groupType, attributePath, printCheckMessage, data));
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = group->groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                {
                    IFF_visitAttributeByIndex(attributePath, j);
                    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(chunks[j], group->groupType, attributePath, printCheckMessage, data));
                    IFF_unvisitAttribute(attributePath);
                }
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
                IFF_Chunk *chunk = group->groupStructure->getChunkFromGroup(group, i);

                if(chunk != NULL)
                    IFF_freeChunk(chunk, group->groupType);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = group->groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                    IFF_freeChunk(chunks[j], group->groupType);

                free(chunks);
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
            IFF_printIndent(file, indentLevel, ".%s = ", groupMember->attributeName);

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = group->groupStructure->getChunkFromGroup(group, i);

                if(chunk == NULL)
                    fprintf(file, "NULL");
                else
                    IFF_printChunk(file, chunk, indentLevel, group->groupType);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = group->groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                fprintf(file, "{\n");

                for(j = 0; j < chunksLength; j++)
                {
                    if(j > 0)
                        fputs(",\n", file);

                    IFF_printIndent(file, indentLevel + 1, "");
                    IFF_printChunk(file, chunks[j], indentLevel + 1, group->groupType);
                }

                fprintf(file, "\n");
                IFF_printIndent(file, indentLevel, "}");
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
                IFF_Chunk *chunk1 = group1->groupStructure->getChunkFromGroup(group1, i);
                IFF_Chunk *chunk2 = group2->groupStructure->getChunkFromGroup(group2, i);

                if(chunk1 != NULL && chunk2 != NULL)
                {
                    if(!IFF_compareChunk(chunk1, chunk2, group1->groupType))
                        return FALSE;
                }
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunks1Length, chunks2Length;
                IFF_Chunk **chunks1 = group1->groupStructure->getChunksFromGroup(group1, i, &chunks1Length);
                IFF_Chunk **chunks2 = group2->groupStructure->getChunksFromGroup(group2, i, &chunks2Length);

                if(chunks1Length == chunks2Length)
                {
                    unsigned int j;

                    for(j = 0; j < chunks1Length; j++)
                    {
                        if(!IFF_compareChunk(chunks1[j], chunks2[j], group1->groupType))
                            return FALSE;
                    }
                }
                else
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
                IFF_Chunk *chunk = group->groupStructure->getChunkFromGroup(group, i);

                if(chunk != NULL)
                {
                    if(!IFF_traverseChunkHierarchy(chunk, group->groupType, data, visitChunk))
                        return FALSE;
                }
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = group->groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                {
                    if(!IFF_traverseChunkHierarchy(chunks[j], group->groupType, data, visitChunk))
                        return FALSE;
                }
            }
        }
    }

    return TRUE;
}
