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

#ifndef __IFF_FORM_H
#define __IFF_FORM_H

#define IFF_ID_FORM IFF_MAKEID('F', 'O', 'R', 'M')

typedef struct IFF_Form IFF_Form;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"

/**
 * @brief A special group chunk, which contains an arbitrary number of group chunks and data chunks.
 */
struct IFF_Form
{
    /** Pointer to the parent group chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Group *parent;

    /** Contains the ID of this chunk, which equals to 'FORM' */
    IFF_ID chunkId;

    /** Contains the size of the chunk data in bytes */
    IFF_Long chunkSize;

    /**
     * Contains a form type, which is used for most application file formats as an
     * application file format identifier
     */
    IFF_ID formType;

    /** Contains the number of sub chunks stored in this form chunk */
    unsigned int chunkLength;

    /** An array of chunk pointers referring to the sub chunks */
    IFF_Chunk **chunk;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new form chunk instance with a given chunk size and form type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkSize Size of the chunk data
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Form *IFF_createForm(const IFF_Long chunkSize, const IFF_ID formType);

/**
 * Creates a new empty form chunk instance with a given form type.
 * Sub chunks can be added with the IFF_addToForm() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Form *IFF_createEmptyForm(const IFF_ID formType);

/**
 * Creates an empty FORM with no form type specified. This function should not
 * be directly used to create a FORM.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk data
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Chunk *IFF_createUnparsedForm(const IFF_ID chunkId, const IFF_Long chunkSize);

/**
 * Adds a chunk to the body of the given FORM. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param form An instance of a FORM chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_addToForm(IFF_Form *form, IFF_Chunk *chunk);

/**
 * Reads a form chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a form chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the FORM has been successfully read, or FALSE if an error has occured
 */
IFF_Bool IFF_readForm(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Writes a form chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a form chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the FORM has been successfully written, else FALSE
 */
IFF_Bool IFF_writeForm(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Checks whether the given form type conforms to the IFF specification.
 *
 * @param formType A 4 character form identifier
 * @return TRUE if the form type is valid, else FALSE
 */
IFF_Bool IFF_checkFormType(const IFF_ID formType);

/**
 * Checks whether the form chunk and its sub chunks conform to the IFF specification.
 *
 * @param chunk An instance of a form chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the form is valid, else FALSE.
 */
IFF_Bool IFF_checkForm(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Recursively frees the memory of the sub chunks of the given form chunk.
 *
 * @param chunk An instance of a form chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeForm(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of the form chunk and its sub chunks on the standard output.
 *
 * @param chunk An instance of a form chunk
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printForm(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether the given forms' contents is equal to each other.
 *
 * @param chunk1 Form to compare
 * @param chunk2 Form to compare
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given forms are equal, else FALSE
 */
IFF_Bool IFF_compareForm(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Merges two given IFF form arrays in the target array.
 *
 * @param target Target form array
 * @param targetLength Length of the target form array
 * @param source Source form array
 * @param sourceLength Length of the source form array
 * @return A reallocated target form array containing the forms of both the source and target arrays
 */
IFF_Form **IFF_mergeFormArray(IFF_Form **target, unsigned int *targetLength, IFF_Form **source, const unsigned int sourceLength);

/**
 * Returns an array of form structs of the given form types, which are recursively retrieved from the given form.
 *
 * @param form An instance of a form chunk
 * @param formTypes An array of 4 character form type IDs
 * @param formTypesLength Length of the form types array
 * @param formsLength Returns the length of the resulting array
 * @return An array of form structs
 */
IFF_Form **IFF_searchFormsInForm(IFF_Form *form, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength);

/**
 * Recalculates the chunk size of the given form chunk.
 *
 * @param form An instance of a form chunk
 */
void IFF_updateFormChunkSizes(IFF_Form *form);

/**
 * Retrieves the chunk with the given chunk ID from the given form.
 *
 * @param form An instance of a form chunk
 * @param chunkId An arbitrary chunk ID
 * @return The chunk with the given chunk ID, or NULL if the chunk can't be found
 */
IFF_Chunk *IFF_getDataChunkFromForm(const IFF_Form *form, const IFF_ID chunkId);

/**
 * Retrieves the chunk with the given chunk ID from the given form.
 * If the chunk does not exist and the form is member of a list with shared
 * properties, this function will recursively lookup the chunk from the
 * shared list properties.
 *
 * @param form An instance of a form chunk
 * @param chunkId An arbitrary chunk ID
 * @return The chunk with the given chunk ID, or NULL if the chunk can't be found
 */
IFF_Chunk *IFF_getChunkFromForm(const IFF_Form *form, const IFF_ID chunkId);

/**
 * Retrieves all the chunks with the given chunk ID from the given form. The resulting array must be freed by using free().
 *
 * @param form An instance of a form chunk
 * @param chunkId An arbitrary chunk ID
 * @param chunksLength A pointer to a variable in which the length of the array is stored
 * @return An array with pointers to the chunks with the requested chunk ID, or NULL if there can't be any chunk found
 */
IFF_Chunk **IFF_getChunksFromForm(const IFF_Form *form, const IFF_ID chunkId, unsigned int *chunksLength);

#ifdef __cplusplus
}
#endif

#endif
