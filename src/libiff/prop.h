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

#ifndef __IFF_PROP_H
#define __IFF_PROP_H

#define IFF_ID_PROP IFF_MAKEID('P', 'R', 'O', 'P')

typedef struct IFF_Form IFF_Prop;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "form.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new PROP chunk instance with a given chunk size and form type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkSize Size of the chunk data
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Prop *IFF_createProp(const IFF_Long chunkSize, const IFF_ID formType);

/**
 * Creates a new empty PROP chunk instance with a given form type.
 * The resulting chunk must be freed by using IFF_free().
 * Sub chunks can be added with the IFF_addToProp() function.
 *
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Prop *IFF_createEmptyProp(const IFF_ID formType);

/**
 * Creates an empty PROP with no form type specified. This function should not
 * be directly used to create a PROP.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk data
 * @return PROP chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Chunk *IFF_createUnparsedProp(const IFF_ID chunkId, const IFF_Long chunkSize);

/**
 * Adds a chunk to the body of the given PROP. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param prop An instance of a PROP chunk
 * @param chunk A data chunk
 */
void IFF_addToProp(IFF_Prop *prop, IFF_Chunk *chunk);

/**
 * Reads a PROP chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a PROP chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the PROP has been successfully read, or FALSE if an error has occured
 */
IFF_Bool IFF_readProp(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Writes a PROP chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a PROP chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the PROP has been successfully written, else FALSE
 */
IFF_Bool IFF_writeProp(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Checks whether the PROP chunk and its sub chunks conform to the IFF specification.
 *
 * @param chunk An instance of a PROP chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the PROP is valid, else FALSE.
 */
IFF_Bool IFF_checkProp(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Recursively frees the memory of the sub chunks of the given PROP chunk.
 *
 * @param chunk An instance of a PROP chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeProp(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of the PROP chunk and its sub chunks on the standard output.
 *
 * @param chunk An instance of a PROP chunk
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printProp(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether the given PROP chunks' contents is equal to each other.
 *
 * @param chunk1 PROP chunk to compare
 * @param chunk2 PROP chunk to compare
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given forms are equal, else FALSE
 */
IFF_Bool IFF_compareProp(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Recalculates the chunk size of the given PROP chunk.
 *
 * @param prop An instance of a PROP chunk
 */
void IFF_updatePropChunkSizes(IFF_Prop *prop);

/**
 * Retrieves the chunk with the given chunk ID from the given PROP chunk. 
 *
 * @param prop An instance of a PROP chunk
 * @param chunkId An arbitrary chunk ID
 * @return The chunk with the given chunk ID, or NULL if the chunk can't be found
 */
IFF_Chunk *IFF_getChunkFromProp(const IFF_Prop *prop, const IFF_ID chunkId);

#ifdef __cplusplus
}
#endif

#endif
