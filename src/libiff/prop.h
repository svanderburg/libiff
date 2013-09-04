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

typedef struct IFF_Form IFF_Prop;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "form.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new PROP chunk instance with the given form type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Prop *IFF_createProp(const char *formType);

/**
 * Adds a chunk to the body of the given PROP. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param prop An instance of a PROP chunk
 * @param chunk A data chunk
 */
void IFF_addToProp(IFF_Prop *prop, IFF_Chunk *chunk);

/**
 * Reads a PROP chunk and its sub chunks from a file. The resulting chunk must be
 * freed by using IFF_free().
 *
 * @param file File descriptor of the file
 * @param chunkSize Size of the chunk data
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return The PROP struct derived from the file, or NULL if an error has occured
 */
IFF_Prop *IFF_readProp(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes a PROP chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param prop An instance of a PROP chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the PROP has been successfully written, else FALSE
 */
int IFF_writeProp(FILE *file, const IFF_Prop *prop, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the PROP chunk and its sub chunks conform to the IFF specification.
 *
 * @param prop An instance of a PROP chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the PROP is valid, else FALSE.
 */
int IFF_checkProp(const IFF_Prop *prop, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Recursively frees the memory of the sub chunks of the given PROP chunk.
 *
 * @param prop An instance of a PROP chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_freeProp(IFF_Prop *prop, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays a textual representation of the PROP chunk and its sub chunks on the standard output.
 *
 * @param prop An instance of a PROP chunk
 * @param indentLevel Indent level of the textual representation
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_printProp(const IFF_Prop *prop, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the given PROP chunks' contents is equal to each other.
 *
 * @param prop1 PROP chunk to compare
 * @param prop2 PROP chunk to compare
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the given forms are equal, else FALSE
 */
int IFF_compareProp(const IFF_Prop *prop1, const IFF_Prop *prop2, const IFF_Extension *extension, const unsigned int extensionLength);

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
IFF_Chunk *IFF_getChunkFromProp(const IFF_Prop *prop, const char *chunkId);

#ifdef __cplusplus
}
#endif

#endif
