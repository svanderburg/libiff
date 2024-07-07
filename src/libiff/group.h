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

typedef IFF_QualityLevel (*IFF_subChunkCheckFunction) (const IFF_Group *group, const IFF_Chunk *subChunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);
typedef IFF_QualityLevel (*IFF_groupTypeCheckFunction) (const IFF_ID groupType, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief An abstract group chunk, which contains all common properties of the compound chunk types. This chunk type should never be used directly.
 */
struct IFF_Group
{
    /** Pointer to the parent chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Chunk *parent;

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
};

typedef struct
{
    IFF_ID groupType;
    unsigned int chunksLength;
    IFF_Chunk **chunks;
    const IFF_ChunkRegistry *chunkRegistry;
}
IFF_PrintChunksArrayParameter;

/**
 * Initializes the members of the group chunk instances with default values.
 *
 * @param group A group chunk instance
 * @param groupType A group type ID
 */
void IFF_initGroup(IFF_Group *group, const IFF_ID groupType);

/**
 * Creates a new group chunk instance with the chunk id, chunk size and group type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character chunk id.
 * @param chunkSize Size of the chunk data
 * @param groupType Type describing the purpose of the sub chunks.
 * @return Group chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Group *IFF_createGroup(const IFF_ID chunkId, const IFF_Long chunkSize, IFF_ID groupType);

/**
 * Creates a new empty group chunk instance with the chunk id and group type.
 * Sub chunks can be added with the IFF_addToGroup() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character chunk id.
 * @param groupType Type describing the purpose of the sub chunks.
 * @return Group chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Group *IFF_createEmptyGroup(const IFF_ID chunkId, const IFF_ID groupType);

/**
 * Creates an empty group with no group type specified. This function should not
 * be directly used to create a group.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character chunk id
 * @param chunkSize Size of the chunk data
 * @return Group chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Chunk *IFF_createUnparsedGroup(const IFF_ID chunkId, const IFF_Long chunkSize);

/**
 * Attaches a chunk to the body of the given group.
 *
 * @param group An instance of a group chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_attachToGroup(IFF_Group *group, IFF_Chunk *chunk);

/**
 * Adds a chunk to the body of the given group. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param group An instance of a group chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_addToGroup(IFF_Group *group, IFF_Chunk *chunk);

/**
 * Reads a group chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a group chunk
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the group has been successfully read, or FALSE if an error has occured
 */
IFF_Bool IFF_readGroupContents(FILE *file, IFF_Chunk *chunk, char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Writes all sub chunks inside a group to a file.
 *
 * @param file File descriptor of the file
 * @param group An instance of a group chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the sub chunks have been successfully written, else FALSE
 */
IFF_Bool IFF_writeGroupSubChunks(FILE *file, const IFF_Group *group, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Writes a group chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a group chunk
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the group has been successfully written, else FALSE
 */
IFF_Bool IFF_writeGroupContents(FILE *file, const IFF_Chunk *chunk, char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Computes the actual chunk size of the group.
 *
 * @param group An instance of a group chunk
 * @return The size of the group chunk in bytes
 */
IFF_Long IFF_computeActualGroupChunkSize(const IFF_Group *group);

/**
 * Checks whether the given chunk size matches the chunk size of the group
 *
 * @param group An instance of a group chunk
 * @param actualChunkSize The actual chunk size
 * @return TRUE if the chunk sizes are equal, else FALSE
 */
IFF_QualityLevel IFF_checkGroupChunkSize(const IFF_Group *group, const IFF_Long actualChunkSize, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Checks whether the group sub chunks are valid
 *
 * @param group An instance of a group chunk
 * @param subChunkCheck Pointer to a function, which checks an individual sub chunk for its validity
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return The size of the sub chunks together, or -1 if a failure has occured
 */
IFF_QualityLevel IFF_checkGroupSubChunks(const IFF_Group *group, IFF_subChunkCheckFunction subChunkCheck, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Checks whether the group chunk and its sub chunks conform to the IFF specification.
 *
 * @param group An instance of a group chunk
 * @param groupTypeCheck Pointer to a function, which checks the groupType for its validity
 * @param subChunkCheck Pointer to a function, which checks an individual sub chunk for its validity
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the form is valid, else FALSE.
 */
IFF_QualityLevel IFF_checkGroupContents(const IFF_Group *group, char *groupTypeName, IFF_groupTypeCheckFunction groupTypeCheck, IFF_subChunkCheckFunction subChunkCheck, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Recursively frees the memory of the sub chunks of the given group chunk.
 *
 * @param chunk An instance of a group chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_clearGroupContents(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays the group type on the standard output.
 *
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param groupType A group type ID
 * @param indentLevel Indent level of the textual representation
 */
void IFF_printGroupType(FILE *file, const char *groupTypeName, const IFF_ID groupType, const unsigned int indentLevel);

void IFF_printChunksArray(FILE *file, const void *value, const unsigned int indentLevel);

/**
 * Displays a textual representation of the sub chunks on the standard output.
 *
 * @param group An instance of a group chunk
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printGroupSubChunks(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of the group chunk and its sub chunks on the standard output.
 *
 * @param group An instance of a group chunk
 * @param indentLevel Indent level of the textual representation
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printGroupContents(FILE *file, const IFF_Group *group, const unsigned int indentLevel, const char *groupTypeName, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether the given group chunks' contents is equal to each other.
 *
 * @param chunk1 Group to compare
 * @param chunk2 Group to compare
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given groups are equal, else FALSE
 */
IFF_Bool IFF_compareGroupContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

IFF_Bool IFF_traverseGroupChunkHierarchy(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Increments the given chunk size by the size of the given chunk.
 * Additionally, it takes the padding byte into account if the chunk size is odd.
 *
 * @param chunkSize Chunk size of a group chunk
 * @param chunk A sub chunk
 * @return The incremented chunk size with an optional padding byte
 */
IFF_Long IFF_incrementChunkSize(const IFF_Long chunkSize, const IFF_Chunk *chunk);

/**
 * Recalculates the chunk size of the given group chunk.
 *
 * @param group An instance of a group chunk
 */
void IFF_updateGroupChunkSizes(IFF_Group *group);

/**
 * Recalculates the chunk size of the given chunk and recursively updates the chunk sizes of the parent group chunks.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 */
void IFF_updateChunkSizes(IFF_Chunk *chunk);

#ifdef __cplusplus
}
#endif

#endif
