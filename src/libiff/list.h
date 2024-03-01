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

#ifndef __IFF_LIST_H
#define __IFF_LIST_H

#define IFF_ID_LIST IFF_MAKEID('L', 'I', 'S', 'T')

typedef struct IFF_List IFF_List;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "prop.h"

/**
 * @brief A special group chunk, which contains one or more FORM, LIST or CAT chunks and PROP chunks which share common data chunks with the nested group chunks.
 */
struct IFF_List
{
    /** Pointer to the parent group chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Group *parent;

    /** Contains the ID of this chunk, which equals to 'LIST' */
    IFF_ID chunkId;

    /** Contains the size of the chunk data in bytes */
    IFF_Long chunkSize;

    /**
     * Contains a type ID which hints about the contents of this list.
     * 'JJJJ' is used if this concatenation stores forms of multiple form types.
     * If only one form type is used in this concatenation, this contents type
     * should be equal to that form type.
     */
    IFF_ID contentsType;

    /** Contains the number of sub chunks stored in this list chunk */
    unsigned int chunkLength;

    /** An array of chunk pointers referring to the sub chunks */
    IFF_Chunk **chunk;

    /** Contains the number of PROP chunks stored in this list chunk */
    unsigned int propLength;

    /** An array of chunk pointers referring to the PROP chunks */
    IFF_Prop **prop;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new list chunk instance with a given chunk size and contents type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkSize Size of the chunk data
 * @param contentsType Contents type hinting what the contents of the list is.
 * @return A list chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_List *IFF_createList(const IFF_Long chunkSize, const IFF_ID contentsType);

/**
 * Creates a new list chunk instance with a given contents type.
 * The resulting chunk must be freed by using IFF_free().
 * Sub chunks can be added with the IFF_addToList() function.
 *
 * @param contentsType Contents type hinting what the contents of the list is.
 * @return A list chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_List *IFF_createEmptyListWithContentsType(const IFF_ID contentsType);

/**
 * Creates a new list chunk instance with the JJJJ contents type.
 * The resulting chunk must be freed by using IFF_free().
 * Sub chunks can be added with the IFF_addToList() function.
 *
 * @return A list chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_List *IFF_createEmptyList(void);

/**
 * Creates an empty list with no contents type specified. This function should not
 * be directly used to create a list.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character id
 * @param chunkSize Size of the chunk data
 * @return List chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Chunk *IFF_createUnparsedList(const IFF_ID chunkId, const IFF_Long chunkSize);

/**
 * Adds a PROP chunk to the body of the given list. This function also increments the
 * chunk size and PROP length counter.
 *
 * @param list An instance of a list struct
 * @param prop A PROP chunk
 */
void IFF_addPropToList(IFF_List *list, IFF_Prop *prop);

/**
 * Adds a chunk to the body of the given list. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param list An instance of a list struct
 * @param chunk A FORM, CAT or LIST chunk
 */
void IFF_addToList(IFF_List *list, IFF_Chunk *chunk);

/**
 * Adds a chunk to the body of the given list.
 * If it sees that all sub chunks have the same group type, it adopts it as its contents type.
 * If there is no uniform group type possible it sets the contents type to: 'JJJJ'.
 * This function also increments the chunk size and chunk length counter.
 *
 * @param list An instance of a list struct
 * @param chunk A FORM, CAT or LIST chunk
 */
void IFF_addToListAndUpdateContentsType(IFF_List *list, IFF_Chunk *chunk);

/**
 * Reads a list chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a list chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the list has been successfully read, or FALSE if an error has occured
 */
IFF_Bool IFF_readList(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Writes a list chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a list chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the list has been successfully written, else FALSE
 */
IFF_Bool IFF_writeList(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

/**
 * Checks whether the list chunk and its sub chunks conform to the IFF specification.
 *
 * @param chunk An instance of a list chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the list is valid, else FALSE.
 */
IFF_Bool IFF_checkList(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Recursively frees the memory of the sub chunks and PROP chunks of the given list chunk.
 *
 * @param chunk An instance of a list chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_freeList(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Displays a textual representation of the list chunk and its sub chunks on the standard output.
 *
 * @param chunk An instance of a list chunk
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printList(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Checks whether the given lists' contents is equal to each other.
 *
 * @param chunk1 List to compare
 * @param chunk2 List to compare
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the given concatenations are equal, else FALSE
 */
IFF_Bool IFF_compareList(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

/**
 * Returns an array of form structs of the given form types, which are recursively retrieved from the given list.
 *
 * @param list An instance of a list chunk
 * @param formTypes An array of 4 character form type IDs
 * @param formTypesLength Length of the form types array
 * @param formsLength Returns the length of the resulting array
 * @return An array of form structs
 */
IFF_Form **IFF_searchFormsInList(IFF_List *list, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength);

/**
 * Recalculates the chunk size of the given list chunk.
 *
 * @param list An instance of a list chunk
 */
void IFF_updateListChunkSizes(IFF_List *list);

/**
 * Retrieves a PROP chunk with the given form type from a list.
 *
 * @param list An instance of a list chunk
 * @param formType Form type describing the purpose of the sub chunks.
 * @return The requested PROP chunk, or NULL if the PROP chunk does not exists.
 */
IFF_Prop *IFF_getPropFromList(const IFF_List *list, const IFF_ID formType);

#ifdef __cplusplus
}
#endif

#endif
