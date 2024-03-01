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

#ifndef __IFF_RAWCHUNK_H
#define __IFF_RAWCHUNK_H

#define IFF_ID_TEXT IFF_MAKEID('T', 'E', 'X', 'T')

typedef struct IFF_RawChunk IFF_RawChunk;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A raw chunk, which contains an arbitrary number of bytes.
 */
struct IFF_RawChunk
{
    /** Pointer to the parent group chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Group *parent;

    /** Contains a 4 character ID of this chunk */
    IFF_ID chunkId;

    /** Contains the size of the chunk data in bytes */
    IFF_Long chunkSize;

    /** An array of bytes representing raw chunk data */
    IFF_UByte *chunkData;
};

/**
 * Creates a raw chunk with the given chunk ID and size. The resulting chunk must be freed using IFF_free().
 *
 * @param chunkId A 4 character id
 * @param chunkSize Length of the bytes array.
 * @return A raw chunk with the given chunk Id, or NULL if the memory can't be allocated
 */
IFF_Chunk *IFF_createRawChunk(const IFF_ID chunkId, const IFF_Long chunkSize);

/**
 * Copies the given data array to the chunk data
 *
 * @param rawChunk A raw chunk
 * @param data Data to copy. Its size should be at least as big as the chunk size
 */
void IFF_copyDataToRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *data);

/**
 * Attaches chunk data to a given chunk. It also increments the chunk size.
 *
 * @param rawChunk A raw chunk
 * @param chunkData An array of bytes
 * @param chunkSize Length of the bytes array.
 */
void IFF_setRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *chunkData, IFF_Long chunkSize);

/**
 * Copies the given string into the data of the chunk. Additionally, it makes
 * the chunk size equal to the given string.
 *
 * @param rawChunk A raw chunk
 * @param text Text to store in the body
 */
void IFF_setTextData(IFF_RawChunk *rawChunk, const char *text);

/**
 * Reads a raw chunk with the given chunk id and chunk size from a file.
 *
 * @param file File descriptor of the file
 * @param chunk A raw chunk instance
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the chunk has been successfully read, else FALSE
 */
IFF_Bool IFF_readRawChunk(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Writes the given raw chunk to a file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A raw chunk instance
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the chunk has been successfully written, else FALSE
 */
IFF_Bool IFF_writeRawChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Checks the given raw chunk
 *
 * @param chunk A raw chunk instance
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the chunk is valid, else FALSE
 */
IFF_Bool IFF_checkRawChunk(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Frees the raw chunk data of the given raw chunk.
 *
 * @param chunk A raw chunk instance
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeRawChunk(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Prints the data of the raw chunk as text
 *
 * @param rawChunk A raw chunk instance
 * @param indentLevel Indent level of the textual representation
 */
void IFF_printText(const IFF_RawChunk *rawChunk, const unsigned int indentLevel);

/**
 * Prints the data of the raw chunk as numeric values
 *
 * @param rawChunk A raw chunk instance
 * @param indentLevel Indent level of the textual representation
 */
void IFF_printRaw(const IFF_RawChunk *rawChunk, const unsigned int indentLevel);

/**
 * Displays a textual representation of the raw chunk data on the standard output.
 *
 * @param chunk A raw chunk instance
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printRawChunk(const IFF_Chunk *chunk, unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether two given raw chunks are equal.
 *
 * @param chunk1 Raw chunk to compare
 * @param chunk2 Raw chunk to compare
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the raw chunks are equal, else FALSE
 */
IFF_Bool IFF_compareRawChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
