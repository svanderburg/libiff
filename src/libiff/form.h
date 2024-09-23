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
#include "attributepath.h"
#include "error.h"
#include "prop.h"
#include "groupstructure.h"

extern IFF_ChunkInterface IFF_formInterface;

/**
 * @brief A special group chunk, which contains an arbitrary number of group chunks and data chunks.
 */
struct IFF_Form
{
    /** Pointer to the parent chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Chunk *parent;

    /** Pointer to the chunk interface exposing operations to manage the chunk */
    IFF_ChunkInterface *chunkInterface;

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
    unsigned int chunksLength;

    /** An array of chunk pointers referring to the sub chunks */
    IFF_Chunk **chunks;

    /** Defines the structure of the functional fields inside this struct */
    IFF_GroupStructure *groupStructure;
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
IFF_Form *IFF_createForm(const IFF_Long chunkSize, const IFF_ID formType, IFF_GroupStructure *formStructure);

/**
 * Creates a new empty form chunk instance with a given form type.
 * Sub chunks can be added with the IFF_addChunkToForm() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Form *IFF_createEmptyForm(const IFF_ID formType, IFF_GroupStructure *formStructure);

/**
 * Adds a chunk to the body of the given FORM. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param form An instance of a FORM chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_addChunkToForm(IFF_Form *form, IFF_Chunk *chunk);

/**
 * Removes a chunk from the body of the given FORM at the given index. This function also decrements
 * the chunk size and chunk length counter.
 *
 * @param form An instance of a FORM chunk
 * @param index Index of a chunk
 * @return The chunk that was removed or NULL if it cannot be removed
 */
IFF_Chunk *IFF_removeChunkFromFormByIndex(IFF_Form *form, const unsigned int index);

/**
 * Updates a chunk in the body of the given FORM at the given index and updates the chunk size.
 *
 * @param form An instance of a FORM chunk
 * @param index Index of a chunk
 * @param chunk A chunk that needs to be put in the position
 * @return The chunk that was previously at the specified index or NULL if it does not exists
 */
IFF_Chunk *IFF_updateChunkInFormByIndex(IFF_Form *form, const unsigned int index, IFF_Chunk *chunk);

/**
 * Reads a form chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a form chunk
 * @param registry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the FORM has been successfully read, or FALSE if an error has occured
 */
IFF_Chunk *IFF_parseFormContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Writes a form chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a form chunk
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the FORM has been successfully written, else FALSE
 */
IFF_Bool IFF_writeFormContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Checks whether the given form type conforms to the IFF specification.
 *
 * @param formType A 4 character form identifier
 * @return TRUE if the form type is valid, else FALSE
 */
IFF_QualityLevel IFF_checkFormType(const IFF_ID formType, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId);

/**
 * Checks whether the form chunk and its sub chunks conform to the IFF specification.
 *
 * @param chunk An instance of a form chunk
 * @return TRUE if the form is valid, else FALSE.
 */
IFF_QualityLevel IFF_checkFormContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

void IFF_clearFormContents(IFF_Chunk *chunk);

/**
 * Displays a textual representation of the form chunk and its sub chunks on the standard output.
 *
 * @param chunk An instance of a form chunk
 * @param indentLevel Indent level of the textual representation
 */
void IFF_printFormContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel);

IFF_Bool IFF_compareFormContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

IFF_Bool IFF_traverseFormChunkHierarchy(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk);

void IFF_recalculateFormChunkSize(IFF_Chunk *chunk);

/**
 * Recursively searches for all FORMs with the given form types in a chunk hierarchy.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formTypes An array of 4 character form identifiers
 * @param formTypesLength Length of the form types array
 * @param formsLength An integer in which the length of the resulting array is stored
 * @return An array of forms having the given form type
 */
IFF_Form **IFF_searchFormsFromArray(IFF_Chunk *chunk, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength);

/**
 * Recursively searches for all FORMs with the given form type in a chunk hierarchy.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param formType A 4 character form identifier
 * @param formsLength An integer in which the length of the resulting array is stored
 * @return An array of forms having the given form type
 */
IFF_Form **IFF_searchForms(IFF_Chunk *chunk, const IFF_ID formType, unsigned int *formsLength);

#ifdef __cplusplus
}
#endif

#endif
