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

#ifndef __IFF_CHUNKREGISTRY_H
#define __IFF_CHUNKREGISTRY_H

typedef struct IFF_ChunkInterface IFF_ChunkInterface;
typedef struct IFF_ChunkType IFF_ChunkType;
typedef struct IFF_ChunkTypesNode IFF_ChunkTypesNode;
typedef struct IFF_FormChunkTypes IFF_FormChunkTypes;
typedef struct IFF_ChunkRegistry IFF_ChunkRegistry;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "attributepath.h"
#include "error.h"

typedef IFF_QualityLevel (*IFF_checkMainChunkFunction) (const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * @brief An interface of operations to manage a particular chunk
 */
struct IFF_ChunkInterface
{
    /** Function responsible for creating the given chunk */
    IFF_Chunk *(*createChunk) (const IFF_ID chunkId, const IFF_Long chunkSize);

    /** Function responsible for reading the given chunk */
    IFF_Bool (*readChunkContents) (FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

    /** Function responsible for writing the given chunk */
    IFF_Bool (*writeChunkContents) (FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

    /** Function responsible for checking the given chunk */
    IFF_QualityLevel (*checkChunkContents) (const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

    /** Function responsible for freeing the contents of the given chunk */
    void (*clearChunkContents) (IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

    /** Function responsible for printing the contents of the given chunk */
    void (*printChunkContents) (FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

    /** Function responsible for comparing the contents of the given chunk */
    IFF_Bool (*compareChunkContents) (const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

    /** Function responsible for traversing the chunk and its sub chunks. This function is only relevant for chunks embedding other chunks. If it refers to NULL, then it lacks the ability to do it */
    IFF_Bool (*traverseChunkHierarchy) (IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk, const IFF_ChunkRegistry *chunkRegistry);

    /** Recalculates the size of the given chunk and its parent chunks. This function is only relevant for chunks embedding other chunks. If it refers to NULL, then it lacks the ability to do it */
    void (*recalculateChunkSize) (IFF_Chunk *chunk);
};

/**
 * @brief Defines how a particular chunk with a specific chunk id should be managed
 */
struct IFF_ChunkType
{
    /** A 4 character chunk id */
    IFF_ID chunkId;

    /** Interface definition that specifies how the chunk should be managed */
    IFF_ChunkInterface *chunkInterface;
};

struct IFF_ChunkTypesNode
{
    /** Specifies the number of chunk types that have a specific meaning inside a FORM */
    unsigned int chunkTypesLength;

    /** An array specifying how chunks within the FORM context should be managed */
    IFF_ChunkType *chunkTypes;

    /** Link to the parent node that provides additional application chunks */
    IFF_ChunkTypesNode *parent;
};

/**
 * @brief Defines how chunks inside a FORM with a particular formType should be managed.
 */
struct IFF_FormChunkTypes
{
    /** A 4 character form type id */
    IFF_ID formType;

    /** Link to the first node that defines how application specific chunks in a FORM should be managed */
    IFF_ChunkTypesNode *chunkTypesNode;
};

/**
 * @brief Defines a registry of chunk types that should be managed in a certain way
 */
struct IFF_ChunkRegistry
{
    /** Specifies the number of FORM types that have their own chunk types */
    unsigned int formChunkTypesLength;

    /** An array specifying the types of chunks per FORM type */
    IFF_FormChunkTypes *formChunkTypes;

    /** Link to the first node that specifies how chunks with global identifier should be handled */
    IFF_ChunkTypesNode *globalChunkTypesNode;

    /** Interface of the default chunk, that is used when no FORM-specific or global identifier matches */
    IFF_ChunkInterface *defaultChunkInterface;

    /** Function that checks the main chunk's validity */
    IFF_checkMainChunkFunction checkMainChunk;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Searches for a chunk interface that can deal with a chunk in a given form with a form type and a given chunk id
 *
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @param formType A 4 character form type id. If the formType is 0 then only the global chunk types will be considered
 * @param chunkId A 4 character chunk id
 * @return The chunk interface that specifies how a chunk type within a form should be handled
 */
IFF_ChunkInterface *IFF_findChunkInterface(const IFF_ChunkRegistry *chunkRegistry, const IFF_ID formType, const IFF_ID chunkId);

#ifdef __cplusplus
}
#endif

#endif
