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

#define IFF_ID_CAT IFF_MAKEID('C', 'A', 'T', ' ')
#define IFF_ID_JJJJ IFF_MAKEID('J', 'J', 'J', 'J')

typedef struct IFF_CAT IFF_CAT;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"

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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new concatentation chunk instance with a given chunk size and
 * contents type. The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkSize Size of the chunk data
 * @param contentsType Contents type hinting what the contents of the CAT is.
 * @return CAT chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_CAT *IFF_createCAT(const IFF_Long chunkSize, const IFF_ID contentsType);

/**
 * Creates a new empty concatentation chunk instance with a given contents type.
 * Sub chunks can be added with the IFF_addToCAT() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param contentsType Contents type hinting what the contents of the CAT is.
 * @return CAT chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_CAT *IFF_createEmptyCATWithContentsType(const IFF_ID contentsType);

/**
 * Creates a new empty concatentation chunk instance using the JJJJ contents type.
 * Sub chunks can be added with the IFF_addToCAT() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @return CAT chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_CAT *IFF_createEmptyCAT(void);

/**
 * Creates an empty CAT with no contents type specified. This function should not
 * be directly used to create a CAT.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk data
 * @return CAT chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Chunk *IFF_createUnparsedCAT(const IFF_ID chunkId, const IFF_Long chunkSize);

/**
 * Adds a chunk to the body of the given CAT. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param cat An instance of a CAT struct
 * @param chunk A FORM, CAT or LIST chunk
 */
void IFF_addToCAT(IFF_CAT *cat, IFF_Chunk *chunk);

/**
 * Adds a chunk to the body of the given CAT and updates the contents type.
 * If it sees that all sub chunks have the same group type, it adopts it as its contents type.
 * If there is no uniform group type possible it sets the contents type to: 'JJJJ'.
 * This function also increments the chunk size and chunk length counter.
 *
 * @param cat An instance of a CAT struct
 * @param chunk A FORM, CAT or LIST chunk
 */
void IFF_addToCATAndUpdateContentsType(IFF_CAT *cat, IFF_Chunk *chunk);

/**
 * Reads a concatenation chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a concatenation chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the CAT has been successfully read, or FALSE if an error has occured
 */
IFF_Bool IFF_readCAT(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Writes a concatenation chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a concatenation chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the CAT has been successfully written, else FALSE
 */
IFF_Bool IFF_writeCAT(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Checks a sub chunk in a CAT for its validity.
 *
 * @param group An instance of a group chunk
 * @param subChunk A sub chunk member of this concatenation chunk
 * @return TRUE if the sub chunk is valid, else FALSE
 */
IFF_Bool IFF_checkCATSubChunk(const IFF_Group *group, const IFF_Chunk *subChunk);

/**
 * Checks whether the concatenation chunk and its sub chunks conform to the IFF specification.
 *
 * @param chunk An instance of a concatenation chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the CAT is valid, else FALSE.
 */
IFF_Bool IFF_checkCAT(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Recursively frees the memory of the sub chunks of the given concatenation chunk.
 *
 * @param chunk An instance of a concatenation chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeCAT(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of the concatenation chunk and its sub chunks on the standard output.
 *
 * @param chunk An instance of a concatenation chunk
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printCAT(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether the given concatenations' contents is equal to each other.
 *
 * @param chunk1 Concatenation to compare
 * @param chunk2 Concatenation to compare
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given concatenations are equal, else FALSE
 */
IFF_Bool IFF_compareCAT(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Returns an array of form structs of the given formType, which are recursively retrieved from the given CAT.
 *
 * @param cat An instance of a concatenation chunk
 * @param formTypes An array of 4 character form type IDs
 * @param formTypesLength Length of the form types array
 * @param formsLength Returns the length of the resulting array
 * @return An array of form structs
 */
IFF_Form **IFF_searchFormsInCAT(IFF_CAT *cat, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength);

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
