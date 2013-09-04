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
#include "extension.h"
#include "group.h"
#include "form.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * Allocates memory for a chunk with the given chunk ID and chunk size.
 * The resulting chunk must be freed using IFF_free()
 *
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk in bytes
 * @return A generic chunk with the given chunk Id and size, or NULL if the memory can't be allocated.
 */
IFF_Chunk *IFF_allocateChunk(const char *chunkId, const size_t chunkSize);

/**
 * Reads a chunk hierarchy from a given file descriptor. The resulting chunk must be freed using IFF_free()
 *
 * @param file File descriptor of the file
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_readChunk(FILE *file, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes a chunk hierarchy to a given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the file has been successfully written, else FALSE
 */
int IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether a chunk hierarchy conforms to the IFF specification.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the IFF file conforms to the IFF specification, else FALSE
 */
int IFF_checkChunk(const IFF_Chunk *chunk, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Frees an IFF chunk hierarchy from memory.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 */
void IFF_freeChunk(IFF_Chunk *chunk, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays a textual representation of an IFF chunk hierarchy on the standard output.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 */
void IFF_printChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether two given chunk hierarchies are equal.
 *
 * @param chunk1 Chunk hierarchy to compare
 * @param chunk2 Chunk hierarchy to compare
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the given chunk hierarchies are equal, else FALSE
 */
int IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Recursively searches for all FORMs with the given form type in a chunk hierarchy.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType A 4 character form identifier
 * @param formsLength An integer in which the length of the resulting array is stored
 * @return An array of forms having the given form type
 */
IFF_Form **IFF_searchForms(IFF_Chunk *chunk, const char *formType, unsigned int *formsLength);

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
 * Recalculates the chunk size of the given chunk and recursively updates the chunk sizes of the parent group chunks.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 */
void IFF_updateChunkSizes(IFF_Chunk *chunk);

#ifdef __cplusplus
}
#endif

#endif
