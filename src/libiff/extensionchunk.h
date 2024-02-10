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

#ifndef __IFF_EXTENSIONCHUNK_H
#define __IFF_EXTENSIONCHUNK_H

#include <stdio.h>
#include "chunk.h"
#include "extension.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads a data chunk by invoking extension functions.
 *
 * @param file File descriptor of the file
 * @param chunkSize Size of the data chunk in bytes
 * @param formExtension Extension that refers to the function executing data chunk specific operations
 * @return The data chunk struct derived from the file, or NULL if an error has occured
 */
IFF_Chunk *IFF_readExtensionChunk(FILE *file, const IFF_Long chunkSize, const IFF_FormExtension *formExtension);

/**
 * Writes a given data chunk to a file descriptor by invoking extension functions.
 *
 * @param file File descriptor of the file
 * @param chunk A data chunk instance
 * @param formExtension Extension that refers to the function executing data chunk specific operations
 * @return TRUE if the chunk has been successfully written, else FALSE
 */
IFF_Bool IFF_writeExtensionChunk(FILE *file, const IFF_Chunk *chunk, const IFF_FormExtension *formExtension);

/**
 * Frees the data of the given data chunk by invoking extension functions.
 *
 * @param chunk A data chunk instance
 * @param formExtension Extension that refers to the function executing data chunk specific operations
 */
void IFF_freeExtensionChunk(IFF_Chunk *chunk, const IFF_FormExtension *formExtension);

/**
 * Checks the fields of a given data chunk by invoking extension functions.
 *
 * @param chunk A data chunk instance
 * @param formExtension Extension that refers to the function executing data chunk specific operations
 * @return TRUE if the fields are valid, else FALSE
 */
IFF_Bool IFF_checkExtensionChunk(const IFF_Chunk *chunk, const IFF_FormExtension *formExtension);

/**
 * Prints the fields of a given data chunk by invoking extension functions.
 *
 * @param chunk A data chunk instance
 * @param indentLevel Indent level of the textual representation
 * @param formExtension Extension that refers to the function executing data chunk specific operations
 */
void IFF_printExtensionChunk(const IFF_Chunk *chunk, unsigned int indentLevel, const IFF_FormExtension *formExtension);

/**
 * Checks whether two given data chunks are equal.
 *
 * @param chunk1 Data chunk to compare
 * @param chunk2 Data chunk to compare
 * @param formExtension Extension that refers to the function executing data chunk specific operations
 * @return TRUE if the data chunks are equal, else FALSE
 */
IFF_Bool IFF_compareExtensionChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_FormExtension *formExtension);

#ifdef __cplusplus
}
#endif

#endif