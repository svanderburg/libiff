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

#ifndef __IFF_CAT_H
#define __IFF_CAT_H

typedef struct IFF_CAT IFF_CAT;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A special group chunk, which contains one or more FORM, LIST or CAT chunks.
 */
struct IFF_CAT
{
    /** Pointer to the parent group chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Group *parent;

    /** Contains a 4 character ID of this chunk, which equals to 'CAT ' */
    IFF_ID chunkId;
    
    /** Contains the size of the chunk data in bytes */
    IFF_Long chunkSize;
    
    /**
     * Contains a type ID which hints about the contents of this concatenation.
     * 'JJJJ' is used if this concatenation stores forms of multiple form types.
     * If only one form type is used in this concatenation, this contents type
     * should be equal to that form type.
     */
    IFF_ID contentsType;
    
    /** Contains the number of sub chunks stored in this concatenation chunk */
    unsigned int chunkLength;
    
    /** An array of chunk pointers referring to the sub chunks */
    IFF_Chunk **chunk;
};

/**
 * Creates a new concatentation chunk instance with the given contents type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param contentsType Contents type hinting what the contents of the CAT is.
 * @return CAT chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_CAT *IFF_createCAT(const char *contentsType);

/**
 * Adds a chunk to the body of the given CAT. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param cat An instance of a CAT struct
 * @param chunk A FORM, CAT or LIST chunk
 */
void IFF_addToCAT(IFF_CAT *cat, IFF_Chunk *chunk);

/**
 * Reads a concatenation chunk and its sub chunks from a file. The resulting chunk must be
 * freed by using IFF_free().
 *
 * @param file File descriptor of the file
 * @param chunkSize Size of the chunk data
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return The concation struct derived from the file, or NULL if an error has occured
 */
IFF_CAT *IFF_readCAT(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes a concatenation chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param cat An instance of a concatenation chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the CAT has been successfully written, else FALSE
 */
int IFF_writeCAT(FILE *file, const IFF_CAT *cat, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks a sub chunk in a CAT for its validity.
 *
 * @param group An instance of a concatenation chunk
 * @param subChunk A sub chunk member of this concatenation chunk
 * @return TRUE if the sub chunk is valid, else FALSE
 */
int IFF_checkCATSubChunk(const IFF_Group *group, const IFF_Chunk *subChunk);

/**
 * Checks whether the concatenation chunk and its sub chunks conform to the IFF specification.
 *
 * @param cat An instance of a concatenation chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the CAT is valid, else FALSE.
 */
int IFF_checkCAT(const IFF_CAT *cat, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Recursively frees the memory of the sub chunks of the given concatenation chunk.
 *
 * @param cat An instance of a concatenation chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_freeCAT(IFF_CAT *cat, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays a textual representation of the concatenation chunk and its sub chunks on the standard output.
 *
 * @param cat An instance of a concatenation chunk
 * @param indentLevel Indent level of the textual representation
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_printCAT(const IFF_CAT *cat, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the given concatenations' contents is equal to each other.
 *
 * @param cat1 Concatenation to compare
 * @param cat2 Concatenation to compare
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the given concatenations are equal, else FALSE
 */
int IFF_compareCAT(const IFF_CAT *cat1, const IFF_CAT *cat2, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Returns an array of form structs of the given formType, which are recursively retrieved from the given CAT.
 *
 * @param cat An instance of a concatenation chunk
 * @param formType A 4 character form type ID
 * @param formsLength Returns the length of the resulting array
 * @return An array of form structs
 */
IFF_Form **IFF_searchFormsInCAT(IFF_CAT *cat, const char *formType, unsigned int *formsLength);

/**
 * Recalculates the chunk size of the given concatentation chunk.
 *
 * @param cat An instance of a concatenation chunk
 */
void IFF_updateCATChunkSizes(IFF_CAT *cat);

#ifdef __cplusplus
}
#endif

#endif
