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

typedef struct IFF_List IFF_List;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "prop.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * Creates a new list chunk instance with the given contents type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param contentsType Contents type hinting what the contents of the list is.
 * @return A list chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_List *IFF_createList(const char *contentsType);

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
 * Reads a list chunk and its sub chunks from a file. The resulting chunk must be
 * freed by using IFF_free().
 *
 * @param file File descriptor of the file
 * @param chunkSize Size of the chunk data
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return The list struct derived from the file, or NULL if an error has occured
 */
IFF_List *IFF_readList(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes a list chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param list An instance of a list chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the list has been successfully written, else FALSE
 */
int IFF_writeList(FILE *file, const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the list chunk and its sub chunks conform to the IFF specification.
 *
 * @param list An instance of a list chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the list is valid, else FALSE.
 */
int IFF_checkList(const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Recursively frees the memory of the sub chunks and PROP chunks of the given list chunk.
 *
 * @param list An instance of a list chunk
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_freeList(IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays a textual representation of the list chunk and its sub chunks on the standard output.
 *
 * @param list An instance of a list chunk
 * @param indentLevel Indent level of the textual representation
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_printList(const IFF_List *list, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the given lists' contents is equal to each other.
 *
 * @param list1 List to compare
 * @param list2 List to compare
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the given concatenations are equal, else FALSE
 */
int IFF_compareList(const IFF_List *list1, const IFF_List *list2, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Returns an array of form structs of the given form types, which are recursively retrieved from the given list.
 *
 * @param list An instance of a list chunk
 * @param formTypes An array of 4 character form type IDs
 * @param formTypesLength Length of the form types array
 * @param formsLength Returns the length of the resulting array
 * @return An array of form structs
 */
IFF_Form **IFF_searchFormsInList(IFF_List *list, const char **formTypes, const unsigned int formTypesLength, unsigned int *formsLength);

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
IFF_Prop *IFF_getPropFromList(const IFF_List *list, const char *formType);

#ifdef __cplusplus
}
#endif

#endif
