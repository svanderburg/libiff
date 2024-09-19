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

#ifndef __IFF_GROUP_H
#define __IFF_GROUP_H

typedef struct IFF_Group IFF_Group;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "form.h"
#include "attributepath.h"
#include "error.h"
#include "groupstructure.h"

typedef IFF_QualityLevel (*IFF_subChunkCheckFunction) (const IFF_Group *group, const IFF_Chunk *subChunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);
typedef IFF_QualityLevel (*IFF_groupTypeCheckFunction) (const IFF_ID groupType, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId);
typedef IFF_GroupStructure *(*IFF_lookupGroupStructureFunction) (const IFF_Registry *registry, const IFF_ID groupType);

/**
 * @brief An abstract group chunk, which contains all common properties of the compound chunk types. This chunk type should never be used directly.
 */
struct IFF_Group
{
    /** Pointer to the parent chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Chunk *parent;

    /** Pointer to the chunk interface exposing operations to manage the chunk */
    IFF_ChunkInterface *chunkInterface;

    /** Contains a 4 character ID of this chunk */
    IFF_ID chunkId;

    /** Contains the size of the chunk data in bytes */
    IFF_Long chunkSize;

    /** Could be either a formType or a contentsType */
    IFF_ID groupType;

    /** Contains the number of sub chunks stored in this group chunk */
    unsigned int chunksLength;

    /** An array of chunk pointers referring to the sub chunks */
    IFF_Chunk **chunks;

    /** Defines the structure of the functional fields inside this struct */
    IFF_GroupStructure *groupStructure;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new group chunk instance with the chunk id, chunk size and group type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character chunk id.
 * @param chunkSize Size of the chunk data
 * @param groupType Type describing the purpose of the sub chunks.
 * @return Group chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Group *IFF_createGroup(const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID groupType, IFF_GroupStructure *groupStructure, IFF_ChunkInterface *chunkInterface);

/**
 * Creates a new empty group chunk instance with the chunk id and group type.
 * Sub chunks can be added with the IFF_addChunkToGroup() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character chunk id.
 * @param groupType Type describing the purpose of the sub chunks.
 * @return Group chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Group *IFF_createEmptyGroup(const IFF_ID chunkId, const IFF_ID groupType, IFF_GroupStructure *groupStructure, IFF_ChunkInterface *chunkInterface);

/**
 * Attaches a chunk to the body of the given group.
 *
 * @param group An instance of a group chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_attachChunkToGroup(IFF_Group *group, IFF_Chunk *chunk);

/**
 * Adds a chunk to the body of the given group. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param group An instance of a group chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_addChunkToGroup(IFF_Group *group, IFF_Chunk *chunk);

/**
 * Detaches a chunk from the body of the given group.
 *
 * @param group An instance of a group chunk
 * @param index Index of a chunk
 * @return The chunk that was detached or NULL if it cannot be detached
 */
IFF_Chunk *IFF_detachChunkFromGroup(IFF_Group *group, const unsigned int index);

/**
 * Removes a chunk at the given index from the body of the given group. This function also decrements
 * the chunk size and chunk length counter.
 *
 * @param group An instance of a group chunk
 * @param index Index of a chunk
 * @return The chunk that was removed or NULL if it cannot be removed
 */
IFF_Chunk *IFF_removeChunkFromGroupByIndex(IFF_Group *group, const unsigned int index);

/**
 * Updates a chunk in the body of the given group at the given index and updates the chunk size.
 *
 * @param group An instance of a group chunk
 * @param index Index of a chunk
 * @param chunk A chunk that needs to be put in the position
 * @return The chunk that was previously at the specified index or NULL if it does not exists
 */
IFF_Chunk *IFF_updateChunkInGroupByIndex(IFF_Group *group, const unsigned int index, IFF_Chunk *chunk);

IFF_Chunk **IFF_mergeChunksArrayIntoGroup(IFF_Group *group, IFF_Chunk **baseChunks, const unsigned int baseChunksLength, IFF_Chunk **appendChunks, const unsigned int appendChunksLength, unsigned int *resultChunksLength);

IFF_Group *IFF_evaluateGroup(IFF_Group *group);

void IFF_freeEvaluatedGroup(IFF_Group *evaluatedGroup);

/**
 * Parses a group chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk in bytes
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param registry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the group has been successfully read, or FALSE if an error has occured
 */
IFF_Group *IFF_parseGroupContents(FILE *file, IFF_lookupGroupStructureFunction lookupGroupStructure, const IFF_ID chunkId, const IFF_Long chunkSize, char *groupTypeName, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Writes a group chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a group chunk
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the group has been successfully written, else FALSE
 */
IFF_Bool IFF_writeGroupContents(FILE *file, const IFF_Group *group, char *groupTypeName, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Checks whether the group chunk and its sub chunks conform to the IFF specification.
 *
 * @param group An instance of a group chunk
 * @param groupTypeCheck Pointer to a function, which checks the groupType for its validity
 * @param subChunkCheck Pointer to a function, which checks an individual sub chunk for its validity
 * @return TRUE if the form is valid, else FALSE.
 */
IFF_QualityLevel IFF_checkGroupContents(const IFF_Group *group, char *groupTypeName, IFF_groupTypeCheckFunction groupTypeCheck, IFF_subChunkCheckFunction subChunkCheck, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Recursively frees the memory of the sub chunks of the given group chunk.
 *
 * @param chunk An instance of a group chunk
 */
void IFF_clearGroupContents(IFF_Group *group);

void IFF_printChunksArrayField(FILE *file, const void *value, const unsigned int indentLevel);

/**
 * Displays a textual representation of the group chunk and its sub chunks on the standard output.
 *
 * @param group An instance of a group chunk
 * @param indentLevel Indent level of the textual representation
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 */
void IFF_printGroupContents(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const char *groupTypeName);

/**
 * Checks whether the given group chunks' contents is equal to each other.
 *
 * @param chunk1 Group to compare
 * @param chunk2 Group to compare
 * @return TRUE if the given groups are equal, else FALSE
 */
IFF_Bool IFF_compareGroupContents(const IFF_Group *group1, const IFF_Group *group2);

IFF_Bool IFF_traverseGroupChunkHierarchy(IFF_Group *group, void *data, IFF_visitChunkFunction visitChunk);

/**
 * Recalculates the chunk size of the given group chunk.
 *
 * @param chunk An instance of a group chunk
 */
void IFF_recalculateGroupChunkSize(IFF_Group *group);

IFF_Bool IFF_searchLastChunkIndexInGroup(const IFF_Group *group, const IFF_ID chunkId, unsigned int *index);

#ifdef __cplusplus
}
#endif

#endif
