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
#include "group.h"
#include "attributepath.h"

extern IFF_ChunkInterface IFF_catInterface;

/**
 * @brief A special group chunk, which contains one or more FORM, LIST or CAT chunks.
 */
struct IFF_CAT
{
    /** Pointer to the parent chunk, in which this chunk is located. The parent points to NULL if there is no parent. */
    IFF_Chunk *parent;

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
    unsigned int chunksLength;

    /** An array of chunk pointers referring to the sub chunks */
    IFF_Chunk **chunks;
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
 * Sub chunks can be added with the IFF_addChunkToCAT() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param contentsType Contents type hinting what the contents of the CAT is.
 * @return CAT chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_CAT *IFF_createEmptyCATWithContentsType(const IFF_ID contentsType);

/**
 * Creates a new empty concatentation chunk instance using the JJJJ contents type.
 * Sub chunks can be added with the IFF_addChunkToCAT() function.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @return CAT chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_CAT *IFF_createEmptyCAT(void);

/**
 * Adds a chunk to the body of the given CAT. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param cat An instance of a CAT struct
 * @param chunk A FORM, CAT or LIST chunk
 */
void IFF_addChunkToCAT(IFF_CAT *cat, IFF_Chunk *chunk);

/**
 * Adds a chunk to the body of the given CAT and updates the contents type.
 * If it sees that all sub chunks have the same group type, it adopts it as its contents type.
 * If there is no uniform group type possible it sets the contents type to: 'JJJJ'.
 * This function also increments the chunk size and chunk length counter.
 *
 * @param cat An instance of a CAT struct
 * @param chunk A FORM, CAT or LIST chunk
 */
void IFF_addChunkToCATAndUpdateContentsType(IFF_CAT *cat, IFF_Chunk *chunk);

/**
 * Removes a chunk from the body of the given CAT. This function also decrements
 * the chunk size and chunk length counter.
 *
 * @param cat An instance of a CAT struct
 * @param index Index of a chunk
 * @return The chunk that was removed or NULL if it cannot be removed
 */
IFF_Chunk *IFF_removeChunkFromCAT(IFF_CAT *cat, unsigned int index);

/**
 * Updates a chunk in the body of the given CAT and updates the chunk size.
 *
 * @param cat An instance of a CAT struct
 * @param index Index of a chunk
 * @param chunk A chunk that needs to be put in the position
 * @return The chunk that was previously at the specified index or NULL if it does not exists
 */
IFF_Chunk *IFF_updateChunkInCAT(IFF_CAT *cat, unsigned int index, IFF_Chunk *chunk);

/**
 * Updates a chunk in the body of the given CAT and updates the chunk size.
 * If it sees that all sub chunks have the same group type, it adopts it as its contents type.
 * If there is no uniform group type possible it sets the contents type to: 'JJJJ'.
 *
 * @param cat An instance of a CAT struct
 * @param index Index of a chunk
 * @param chunk A chunk that needs to be put in the position
 * @return The chunk that was previously at the specified index or NULL if it does not exists
 */
IFF_Chunk *IFF_updateChunkInCATAndUpdateContentsType(IFF_CAT *cat, unsigned int index, IFF_Chunk *chunk);

/**
 * Reads a concatenation chunk and its sub chunks from a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a concatenation chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the CAT has been successfully read, or FALSE if an error has occured
 */
IFF_Bool IFF_readCATContents(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Writes a concatenation chunk and its sub chunks to a file.
 *
 * @param file File descriptor of the file
 * @param chunk An instance of a concatenation chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the CAT has been successfully written, else FALSE
 */
IFF_Bool IFF_writeCATContents(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

/**
 * Checks a sub chunk in a CAT for its validity.
 *
 * @param group An instance of a group chunk
 * @param subChunk A sub chunk member of this concatenation chunk
 * @return TRUE if the sub chunk is valid, else FALSE
 */
IFF_QualityLevel IFF_checkCATSubChunk(const IFF_Group *group, const IFF_Chunk *subChunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Checks whether the concatenation chunk and its sub chunks conform to the IFF specification.
 *
 * @param chunk An instance of a concatenation chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the CAT is valid, else FALSE.
 */
IFF_QualityLevel IFF_checkCATContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Displays a textual representation of the concatenation chunk and its sub chunks on the standard output.
 *
 * @param chunk An instance of a concatenation chunk
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printCATContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
