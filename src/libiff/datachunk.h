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

#ifndef __IFF_DATACHUNK_H
#define __IFF_DATACHUNK_H

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new data chunk instance.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk data
 * @param formType Form type describing the purpose of the sub chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return A data chunk instance or NULL, if the memory for the struct can't be allocated
 */
IFF_Chunk *IFF_createDataChunk(const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Reads a data chunk from a given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. 0 is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the data chunk has been successfully read, or FALSE if an error has occured
 */
IFF_Bool IFF_readDataChunk(FILE *file, IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Writes a data chunk to a given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. 0 is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeDataChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether a data chunk conforms to the IFF specification.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. 0 is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the IFF file conforms to the IFF specification, else FALSE
 */
IFF_Bool IFF_checkDataChunk(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Frees a data chunk hierarchy from memory.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. 0 is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeDataChunk(IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of a data chunk hierarchy on the standard output.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param formType Form type id describing in which FORM the sub chunk is located. 0 is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printDataChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether two given data chunks are equal.
 *
 * @param chunk1 A chunk hierarchy representing an IFF file
 * @param chunk2 A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. 0 is used for sub chunks in other group chunks.
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given chunk hierarchies are equal, else FALSE
 */
IFF_Bool IFF_compareDataChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
