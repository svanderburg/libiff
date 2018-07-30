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

typedef struct IFF_Form IFF_Form;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * Creates a new form chunk instance with the given form type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Form *IFF_createForm(const char *formType);

/**
 * Adds a chunk to the body of the given FORM. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param form An instance of a FORM chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_addToForm(IFF_Form *form, IFF_Chunk *chunk);

/**
 * Reads a form chunk and its sub chunks from a file. The resulting chunk must be
 * freed by using IFF_free().
 *
 * @param file File descriptor of the file
 * @param chunkSize Size of the chunk data
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return The form struct derived from the file, or NULL if an error has occured
 */
IFF_Form *IFF_readForm(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes a form chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param form An instance of a form chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the FORM has been successfully written, else FALSE
 */
int IFF_writeForm(FILE *file, const IFF_Form *form, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the given form type conforms to the IFF specification.
 *
 * @param formType A 4 character form identifier
 * @return TRUE if the form type is valid, else FALSE
 */
int IFF_checkFormType(const IFF_ID formType);

/**
 * Checks whether the form chunk and its sub chunks conform to the IFF specification.
 *
 * @param form An instance of a form chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the form is valid, else FALSE.
 */
int IFF_checkForm(const IFF_Form *form, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Recursively frees the memory of the sub chunks of the given form chunk.
 *
 * @param form An instance of a form chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_freeForm(IFF_Form *form, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays a textual representation of the form chunk and its sub chunks on the standard output.
 *
 * @param form An instance of a form chunk
 * @param indentLevel Indent level of the textual representation
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_printForm(const IFF_Form *form, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the given forms' contents is equal to each other.
 *
 * @param form1 Form to compare
 * @param form2 Form to compare
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the given forms are equal, else FALSE
 */
int IFF_compareForm(const IFF_Form *form1, const IFF_Form *form2, const IFF_Extension *extension, const unsigned int extensionLength);

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
IFF_Form **IFF_searchFormsInForm(IFF_Form *form, const char **formTypes, const unsigned int formTypesLength, unsigned int *formsLength);

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
IFF_Chunk *IFF_getDataChunkFromForm(const IFF_Form *form, const char *chunkId);

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
IFF_Chunk *IFF_getChunkFromForm(const IFF_Form *form, const char *chunkId);

/**
 * Retrieves all the chunks with the given chunk ID from the given form. The resulting array must be freed by using free().
 *
 * @param form An instance of a form chunk
 * @param chunkId An arbitrary chunk ID
 * @param chunksLength A pointer to a variable in which the length of the array is stored
 * @return An array with pointers to the chunks with the requested chunk ID, or NULL if there can't be any chunk found
 */
IFF_Chunk **IFF_getChunksFromForm(const IFF_Form *form, const char *chunkId, unsigned int *chunksLength);

#ifdef __cplusplus
}
#endif

#endif
