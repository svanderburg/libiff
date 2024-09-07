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
#include <util.h>

IFF_Bool IFF_writeGroupStructure(FILE *file, const IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    if(groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &groupStructure->groupMembers[i];

            IFF_visitAttributeByName(attributePath, groupMember->attributeName);

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = groupStructure->getChunkFromGroup(group, i);

                if(!IFF_writeChunk(file, chunk, group->groupType, chunkRegistry, attributePath, error))
                    return FALSE;

                /* Increase the bytes processed counter */
                *bytesProcessed = IFF_addChunkSize(*bytesProcessed, chunk);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                {
                    IFF_visitAttributeByIndex(attributePath, j);

                    if(!IFF_writeChunk(file, chunks[j], group->groupType, chunkRegistry, attributePath, error))
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

IFF_Long IFF_addActualGroupStructureSize(const IFF_Group *group, const IFF_GroupStructure *groupStructure, IFF_Long chunkSize)
{
    if(groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = groupStructure->getChunkFromGroup(group, i);
                chunkSize = IFF_addChunkSize(chunkSize, chunk);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                    chunkSize = IFF_addChunkSize(chunkSize, chunks[j]);
            }
        }
    }

    return chunkSize;
}

IFF_QualityLevel IFF_checkGroupStructure(const IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;

    if(groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &groupStructure->groupMembers[i];

            IFF_visitAttributeByName(attributePath, groupMember->attributeName);

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = groupStructure->getChunkFromGroup(group, i);
                qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(chunk, group->groupType, chunkRegistry, attributePath, printCheckMessage, data));
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                {
                    IFF_visitAttributeByIndex(attributePath, j);
                    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(chunks[j], group->groupType, chunkRegistry, attributePath, printCheckMessage, data));
                    IFF_unvisitAttribute(attributePath);
                }
            }

            IFF_unvisitAttribute(attributePath);
        }
    }

    return qualityLevel;
}

void IFF_clearGroupStructure(IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry)
{
    if(groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = groupStructure->getChunkFromGroup(group, i);
                IFF_freeChunk(chunk, group->groupType, chunkRegistry);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                    IFF_freeChunk(chunks[j], group->groupType, chunkRegistry);

                free(chunks);
            }
        }
    }
}

void IFF_printGroupStructure(FILE *file, const IFF_Group *group, const IFF_GroupStructure *groupStructure, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    if(groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &groupStructure->groupMembers[i];

            if(i > 0)
                fprintf(file, ",");

            fprintf(file, "\n");
            IFF_printIndent(file, indentLevel, ".%s = ", groupMember->attributeName);

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = groupStructure->getChunkFromGroup(group, i);
                IFF_printChunk(file, chunk, indentLevel, group->groupType, chunkRegistry);
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                fprintf(file, "{\n");

                for(j = 0; j < chunksLength; j++)
                    IFF_printChunk(file, chunks[j], indentLevel + 1, group->groupType, chunkRegistry);

                fprintf(file, "\n");
                IFF_printIndent(file, indentLevel, "}");
            }
        }
    }
}

IFF_Bool IFF_compareGroupStructure(const IFF_Group *group1, const IFF_Group *group2, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry)
{
    if(groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk1 = groupStructure->getChunkFromGroup(group1, i);
                IFF_Chunk *chunk2 = groupStructure->getChunkFromGroup(group2, i);

                if(!IFF_compareChunk(chunk1, chunk2, group1->groupType, chunkRegistry))
                    return FALSE;
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunks1Length, chunks2Length;
                IFF_Chunk **chunks1 = groupStructure->getChunksFromGroup(group1, i, &chunks1Length);
                IFF_Chunk **chunks2 = groupStructure->getChunksFromGroup(group2, i, &chunks2Length);

                if(chunks1Length == chunks2Length)
                {
                    unsigned int j;

                    for(j = 0; j < chunks1Length; j++)
                    {
                        if(!IFF_compareChunk(chunks1[i], chunks2[i], group1->groupType, chunkRegistry))
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

IFF_Bool IFF_traverseGroupStructureHierarchy(const IFF_Group *group, const IFF_GroupStructure *groupStructure, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry)
{
    if(groupStructure != NULL)
    {
        unsigned int i;

        for(i = 0; i < groupStructure->groupMembersLength; i++)
        {
            IFF_GroupMember *groupMember = &groupStructure->groupMembers[i];

            if(groupMember->cardinality == IFF_GROUP_MEMBER_SINGLE)
            {
                IFF_Chunk *chunk = groupStructure->getChunkFromGroup(group, i);

                if(!IFF_traverseChunkHierarchy(chunk, group->groupType, data, visitChunk, chunkRegistry))
                    return FALSE;
            }
            else if(groupMember->cardinality == IFF_GROUP_MEMBER_MULTIPLE)
            {
                unsigned int chunksLength;
                IFF_Chunk **chunks = groupStructure->getChunksFromGroup(group, i, &chunksLength);
                unsigned int j;

                for(j = 0; j < chunksLength; j++)
                {
                    if(!IFF_traverseChunkHierarchy(chunks[j], group->groupType, data, visitChunk, chunkRegistry))
                        return FALSE;
                }
            }
        }
    }

    return TRUE;
}
