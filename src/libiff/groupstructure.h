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

#ifndef __IFF_GROUPSTRUCTURE_H
#define __IFF_GROUPSTRUCTURE_H

typedef struct IFF_GroupMember IFF_GroupMember;
typedef struct IFF_GroupStructure IFF_GroupStructure;

#include <stdio.h>
#include "group.h"
#include "chunk.h"
#include "attributepath.h"
#include "error.h"
#include "chunkregistry.h"

typedef void (*IFF_initGroupContentsFunction) (IFF_Group *group);

typedef IFF_Bool (*IFF_attachChunkToGroupFunction) (IFF_Group *group, IFF_Chunk *chunk);

typedef IFF_Chunk *(*IFF_getChunkFromGroupFunction) (const IFF_Group *group, const unsigned int index);

typedef IFF_Chunk **(*IFF_getChunksFromGroupFunction) (const IFF_Group *group, const unsigned int index, unsigned int *chunksLength);

typedef enum
{
    IFF_GROUP_MEMBER_SINGLE,
    IFF_GROUP_MEMBER_MULTIPLE
}
IFF_GroupMemberCardinality;

struct IFF_GroupMember
{
    IFF_ID chunkId;
    char *attributeName;
    IFF_GroupMemberCardinality cardinality;
};

struct IFF_GroupStructure
{
    unsigned int groupSize;
    unsigned int groupMembersLength;
    IFF_GroupMember *groupMembers;
    IFF_initGroupContentsFunction initGroupContents;
    IFF_attachChunkToGroupFunction attachChunkToGroup;
    IFF_getChunkFromGroupFunction getChunkFromGroup;
    IFF_getChunksFromGroupFunction getChunksFromGroup;
};

#ifdef __cplusplus
extern "C" {
#endif

IFF_Bool IFF_writeGroupStructure(FILE *file, const IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_Long IFF_addActualGroupStructureSize(const IFF_Group *group, const IFF_GroupStructure *groupStructure, IFF_Long chunkSize);

IFF_QualityLevel IFF_checkGroupStructure(const IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

void IFF_clearGroupStructure(IFF_Group *group, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry);

void IFF_printGroupStructure(FILE *file, const IFF_Group *group, const IFF_GroupStructure *groupStructure, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

IFF_Bool IFF_compareGroupStructure(const IFF_Group *group1, const IFF_Group *group2, const IFF_GroupStructure *groupStructure, const IFF_ChunkRegistry *chunkRegistry);

IFF_Bool IFF_traverseGroupStructureHierarchy(const IFF_Group *group, const IFF_GroupStructure *groupStructure, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
