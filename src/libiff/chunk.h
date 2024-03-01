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

typedef struct IFF_Chunk IFF_Chunk;

#include <stdio.h>
#include "ifftypes.h"
#include "chunkregistry.h"
#include "group.h"

/**
 * @brief An abstract chunk containing the common properties of all chunk types
 */
struct IFF_Chunk
{
    /** Pointer to the parent group chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Group *parent;

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
 * @param formType Form type id describing in which FORM the sub chunk is located. 0 is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_readChunk(FILE *file, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Writes a chunk hierarchy to a given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether a chunk hierarchy conforms to the IFF specification.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the IFF file conforms to the IFF specification, else FALSE
 */
IFF_Bool IFF_checkChunk(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Frees an IFF chunk hierarchy from memory.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeChunk(IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of an IFF chunk hierarchy on the standard output.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether two given chunk hierarchies are equal.
 *
 * @param chunk1 Chunk hierarchy to compare
 * @param chunk2 Chunk hierarchy to compare
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given chunk hierarchies are equal, else FALSE
 */
IFF_Bool IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
