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

#ifndef __IFF_CHUNK_H
#define __IFF_CHUNK_H
#include "ifftypes.h"

typedef struct IFF_Chunk IFF_Chunk;
typedef IFF_Bool (*IFF_visitChunkFunction) (IFF_Chunk *chunk, void *data);

#include <stdio.h>
#include "chunkregistry.h"
#include "attributepath.h"
#include "error.h"

/**
 * @brief An abstract chunk containing the common properties of all chunk types
 */
struct IFF_Chunk
{
    /** Pointer to the parent chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Chunk *parent;

    /** Contains a 4 character ID of this chunk */
    IFF_ID chunkId;

    /** Contains the size of the chunk data in bytes */
    IFF_Long chunkSize;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates memory for a chunk with the given chunk ID and chunk size.
 * The resulting chunk must be freed using IFF_free()
 *
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk in bytes
 * @param structSize The size of the struct that provides the data in bytes
 * @return A generic chunk with the given chunk Id and size, or NULL if the memory can't be allocated.
 */
IFF_Chunk *IFF_createChunk(const IFF_ID chunkId, IFF_Long chunkSize, size_t structSize);

/**
 * Reads a chunk hierarchy from a given file descriptor. The resulting chunk must be freed using IFF_free()
 *
 * @param file File descriptor of the file
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_readChunk(FILE *file, const IFF_ID scopeId, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_IOError **error);

/**
 * Writes a chunk hierarchy to a given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID scopeId, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_IOError **error);

/**
 * Checks whether a chunk hierarchy conforms to the IFF specification.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @return TRUE if the IFF file conforms to the IFF specification, else FALSE
 */
IFF_QualityLevel IFF_checkChunk(const IFF_Chunk *chunk, const IFF_ID scopeId, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Frees an IFF chunk hierarchy from memory.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 */
void IFF_freeChunk(IFF_Chunk *chunk, const IFF_ID scopeId, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of an IFF chunk hierarchy on the standard output.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 */
void IFF_printChunk(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID scopeId, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether two given chunk hierarchies are equal.
 *
 * @param chunk1 Chunk hierarchy to compare
 * @param chunk2 Chunk hierarchy to compare
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @return TRUE if the given chunk hierarchies are equal, else FALSE
 */
IFF_Bool IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID scopeId, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Traverses over the chunk and its sub chunks, invoking a visitor function for each chunk that it encounters
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param data An arbitrary data structure propagated to the visitor function
 * @param visitChunk Function that gets invoked for each chunk that is encountered
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @return TRUE if the entire chunk hierarchy was traversed, else FALSE
 */
IFF_Bool IFF_traverseChunkHierarchy(IFF_Chunk *chunk, const IFF_ID scopeId, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Recalculates the chunk size of the given chunk and recursively updates the chunk sizes of the parent group chunks.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param scopeId Specifies the ID of the scope of the chunk. 0 is used to force the global scope.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type within a specified scope
 */
void IFF_recalculateChunkHierarchySizes(IFF_Chunk *chunk, const IFF_ID scopeId, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Computes how much memory a chunk really needs. The chunkSize field of a chunk only indicates how many bytes its contents is.
 * This function also takes the chunk header and padding into account.
 *
 * @param chunk A chunk
 * @return The amount of bytes a chunk consumes
 */
IFF_Long IFF_computeActualChunkSize(const IFF_Chunk *chunk);

/**
 * Adds to the given chunk size the actual size of the given chunk.
 *
 * @param chunkSize Chunk size of a group chunk
 * @param chunk A sub chunk
 * @return The incremented chunk size
 */
IFF_Long IFF_addChunkSize(const IFF_Long chunkSize, const IFF_Chunk *chunk);

/**
 * Increases the chunk size by the actual size of another chunk that is to be attached.
 *
 * @param chunk A chunk
 * @param attachedChunk Chunk to be attached
 */
void IFF_increaseChunkSize(IFF_Chunk *chunk, const IFF_Chunk *attachedChunk);

/**
 * Decreases the chunk size by the actual size of another chunk that is to be attached.
 *
 * @param chunk A chunk
 * @param attachedChunk Chunk to be attached
 */
void IFF_decreaseChunkSize(IFF_Chunk *chunk, const IFF_Chunk *attachedChunk);

/**
 * Updates the chunk size by differences of the actual size of the detached chunk and attached chunk.
 *
 * @param chunk A chunk to be updated
 * @param detachedChunk Chunk to be detached
 * @param attachedChunk Chunk to be attached
 */
void IFF_updateChunkSize(IFF_Chunk *chunk, const IFF_Chunk *detachedChunk, const IFF_Chunk *attachedChunk);

#ifdef __cplusplus
}
#endif

#endif
