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

#ifndef __IFF_GROUP_H
#define __IFF_GROUP_H

typedef struct IFF_Group IFF_Group;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "form.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief An abstract group chunk, which contains all common properties of the compound chunk types. This chunk type should never be used directly.
 */
struct IFF_Group
{
    /** Pointer to the parent group chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Group *parent;

    /** Contains a 4 character ID of this chunk */
    IFF_ID chunkId;
    
    /** Contains the size of the chunk data in bytes */
    IFF_Long chunkSize;
    
    /** Could be either a formType or a contentsType */
    IFF_ID groupType;
    
    /** Contains the number of sub chunks stored in this group chunk */
    unsigned int chunkLength;
    
    /** An array of chunk pointers referring to the sub chunks */
    IFF_Chunk **chunk;
};

/**
 * Initializes the members of the group chunk instances with default values.
 *
 * @param group A group chunk instance
 * @param groupType A group type ID
 */
void IFF_initGroup(IFF_Group *group, const char *groupType);

/**
 * Creates a new group chunk instance with the chunk id and group type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkId A 4 character chunk id.
 * @param groupType Type describing the purpose of the sub chunks.
 * @return Group chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Group *IFF_createGroup(const char *chunkId, const char *groupType);

/**
 * Adds a chunk to the body of the given group. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param group An instance of a group chunk
 * @param chunk An arbitrary group or data chunk
 */
void IFF_addToGroup(IFF_Group *group, IFF_Chunk *chunk);

/**
 * Reads a group chunk and its sub chunks from a file. The resulting chunk must be
 * freed by using IFF_free().
 *
 * @param file File descriptor of the file
 * @param chunkId A 4 character chunk id
 * @param chunkSize Size of the chunk data
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param groupTypeIsFormType Indicates whether the groupType represents a formType
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return The group struct derived from the file, or NULL if an error has occured
 */
IFF_Group *IFF_readGroup(FILE *file, const char *chunkId, const IFF_Long chunkSize, const char *groupTypeName, const int groupTypeIsFormType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes all sub chunks inside a group to a file.
 *
 * @param file File descriptor of the file
 * @param group An instance of a group chunk
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the sub chunks have been successfully written, else FALSE
 */
int IFF_writeGroupSubChunks(FILE *file, const IFF_Group *group, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes a group chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param group An instance of a group chunk
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the group has been successfully written, else FALSE
 */
int IFF_writeGroup(FILE *file, const IFF_Group *group, const char *formType, const char *groupTypeName, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the given chunk size matches the chunk size of the group
 *
 * @param group An instance of a group chunk
 * @param chunkSize A chunk size
 * @return TRUE if the chunk sizes are equal, else FALSE
 */
int IFF_checkGroupChunkSize(const IFF_Group *group, const IFF_Long chunkSize);

/**
 * Checks whether the group sub chunks are valid
 *
 * @param group An instance of a group chunk
 * @param subChunkCheck Pointer to a function, which checks an individual sub chunk for its validity
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return The size of the sub chunks together, or -1 if a failure has occured
 */
IFF_Long IFF_checkGroupSubChunks(const IFF_Group *group, int (*subChunkCheck) (const IFF_Group *group, const IFF_Chunk *subChunk), const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the group chunk and its sub chunks conform to the IFF specification.
 *
 * @param group An instance of a group chunk
 * @param groupTypeCheck Pointer to a function, which checks the groupType for its validity
 * @param subChunkCheck Pointer to a function, which checks an individual sub chunk for its validity
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the form is valid, else FALSE.
 */
int IFF_checkGroup(const IFF_Group *group, int (*groupTypeCheck) (const char *groupType), int (*subChunkCheck) (const IFF_Group *group, const IFF_Chunk *subChunk), const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Recursively frees the memory of the sub chunks of the given group chunk.
 *
 * @param group An instance of a group chunk
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_freeGroup(IFF_Group *group, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays the group type on the standard output.
 *
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param groupType A group type ID
 * @param indentLevel Indent level of the textual representation
 */
void IFF_printGroupType(const char *groupTypeName, const char *groupType, const unsigned int indentLevel);

/**
 * Displays a textual representation of the sub chunks on the standard output.
 *
 * @param group An instance of a group chunk
 * @param indentLevel Indent level of the textual representation
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_printGroupSubChunks(const IFF_Group *group, const unsigned int indentLevel, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays a textual representation of the group chunk and its sub chunks on the standard output.
 *
 * @param group An instance of a group chunk
 * @param indentLevel Indent level of the textual representation
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param groupTypeName Specifies what the group type is called. Could be 'formType' or 'contentsType'
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 */
void IFF_printGroup(const IFF_Group *group, const unsigned int indentLevel, const char *formType, const char *groupTypeName, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether the given group chunks' contents is equal to each other.
 *
 * @param group1 Group to compare
 * @param group2 Group to compare
 * @param formType Form type id describing in which FORM the sub chunk is located. NULL is used for sub chunks in other group chunks.
 * @param extension Extension array which specifies how application file format chunks should be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the given groups are equal, else FALSE
 */
int IFF_compareGroup(const IFF_Group *group1, const IFF_Group *group2, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Returns an array of form structs of the given formType, which are recursively retrieved from the given group.
 *
 * @param group An instance of a group chunk
 * @param formType A 4 character form type ID
 * @param formsLength Returns the length of the resulting array
 * @return An array of form structs
 */
IFF_Form **IFF_searchFormsInGroup(IFF_Group *group, const char *formType, unsigned int *formsLength);

/**
 * Recalculates the chunk size of the given group chunk.
 *
 * @param group An instance of a group chunk
 */
void IFF_updateGroupChunkSizes(IFF_Group *group);

#ifdef __cplusplus
}
#endif

#endif
