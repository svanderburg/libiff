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

#include "chunk.h"
#include <string.h>
#include <stdlib.h>
#include "io.h"
#include "value.h"
#include "id.h"
#include "util.h"
#include "error.h"
#include "field.h"
#include "structure.h"

IFF_Chunk *IFF_createChunk(IFF_ChunkInterface *chunkInterface, const IFF_ID chunkId, const IFF_Long chunkSize, size_t structSize)
{
    IFF_Chunk *chunk = (IFF_Chunk*)malloc(structSize);

    if(chunk != NULL)
    {
        chunk->parent = NULL;
        chunk->chunkInterface = chunkInterface;
        chunk->chunkId = chunkId;
        chunk->chunkSize = chunkSize;
    }

    return chunk;
}

static IFF_Chunk *readChunkBody(FILE *file, IFF_Chunk *chunk, const IFF_ID scopeId, const IFF_Registry *registry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_ChunkInterface *chunkInterface = IFF_findChunkInterface(registry, scopeId, chunk->chunkId);
    IFF_Long bytesProcessed = 0;
    IFF_Chunk *result;

    chunk->chunkInterface = chunkInterface;
    result = chunkInterface->parseChunkContents(file, chunk, registry, attributePath, &bytesProcessed, error);

    if(*error == NULL && chunk != NULL)
    {
        if(IFF_skipUnknownBytes(file, chunk, bytesProcessed, attributePath, error) &&
            IFF_readOptionalPaddingByte(file, chunk, attributePath, error))
            ;
    }

    return result;
}

static void initEmptyChunk(IFF_Chunk *chunk)
{
    memset(chunk, '\0', sizeof(IFF_Chunk));
}

IFF_Chunk *IFF_createDerivedChunk(const IFF_Chunk *chunk, size_t structSize)
{
    IFF_Chunk *derived = (IFF_Chunk*)malloc(structSize);

    if(derived != NULL)
        memcpy(derived, chunk, sizeof(IFF_Chunk));

    return derived;
}

IFF_Chunk *IFF_parseChunk(FILE *file, const IFF_ID scopeId, const IFF_Registry *registry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_Chunk chunk;
    initEmptyChunk(&chunk);

    if(!IFF_readChunkIdField(file, &chunk.chunkId, 0, attributePath, "chunkId", error)
        || !IFF_readChunkSizeField(file, &chunk.chunkSize, chunk.chunkId, attributePath, "chunkSize", error))
        return NULL;

    return readChunkBody(file, &chunk, scopeId, registry, attributePath, error);
}

static IFF_Bool writeChunkBody(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_Long bytesProcessed = 0;

    return chunk->chunkInterface->writeChunkContents(file, chunk, attributePath, &bytesProcessed, error)
        && IFF_writeZeroFillerBytes(file, chunk, bytesProcessed, attributePath, error)
        && IFF_writeOptionalPaddingByte(file, chunk, attributePath, error);
}

IFF_Bool IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    return IFF_writeChunkIdField(file, &chunk->chunkId, chunk->chunkId, attributePath, "chunkId", error)
        && IFF_writeChunkSizeField(file, &chunk->chunkSize, chunk->chunkId, attributePath, "chunkSize", error)
        && writeChunkBody(file, chunk, attributePath, error);
}

IFF_QualityLevel IFF_checkChunk(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkId(chunk->chunkId, attributePath, "chunkId", printCheckMessage, data, 0));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, chunk->chunkInterface->checkChunkContents(chunk, attributePath, printCheckMessage, data));

    return qualityLevel;
}

void IFF_freeChunk(IFF_Chunk *chunk)
{
    if(chunk != NULL)
    {
        chunk->chunkInterface->clearChunkContents(chunk);
        free(chunk);
    }
}

typedef enum
{
    FIELD_INDEX_CHUNK_ID = 0,
    FIELD_INDEX_CHUNK_SIZE = 1
}
FieldIndex;

static void *getFieldPointer(void *object, const unsigned int index)
{
    IFF_Chunk *chunk = (IFF_Chunk*)object;

    switch(index)
    {
        case FIELD_INDEX_CHUNK_ID:
            return &chunk->chunkId;
        case FIELD_INDEX_CHUNK_SIZE:
            return &chunk->chunkSize;
        default:
            return NULL;
    }
}

static IFF_Field fields[] = {
    { "chunkId", &IFF_Type_ID, IFF_CARDINALITY_SINGLE },
    { "chunkSize", &IFF_Type_Long, IFF_CARDINALITY_SINGLE }
};

static IFF_Structure chunkHeaderStructure = {
    2,
    fields,
    getFieldPointer,
    NULL
};

void IFF_printChunk(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    fputs("{\n", file);
    IFF_printStructureFields(file, indentLevel + 1, &chunkHeaderStructure, (void*)chunk);
    chunk->chunkInterface->printChunkContents(file, chunk, indentLevel + 1);
    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

IFF_Bool IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareStructure(&chunkHeaderStructure, (void*)chunk1, (void*)chunk2)
        && chunk1->chunkInterface->compareChunkContents(chunk1, chunk2);
}

IFF_Bool IFF_traverseChunkHierarchy(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk)
{
    return chunk->chunkInterface->traverseChunkHierarchy == NULL
        || chunk->chunkInterface->traverseChunkHierarchy(chunk, data, visitChunk);
}

void IFF_recalculateChunkHierarchySizes(IFF_Chunk *chunk)
{
    if(chunk->chunkInterface->recalculateChunkSize != NULL)
        chunk->chunkInterface->recalculateChunkSize(chunk);

    /* If the given chunk has a parent, recursively update these as well */
    if(chunk->parent != NULL)
        IFF_recalculateChunkHierarchySizes(chunk->parent);
}

IFF_Long IFF_computeActualChunkSize(const IFF_Chunk *chunk)
{
    IFF_Long actualChunkSize = IFF_ID_SIZE + sizeof(IFF_Long) + chunk->chunkSize;

    /* If the size of the nested chunk size is odd, we have to count the padding byte as well */
    if(chunk->chunkSize % 2 != 0)
        actualChunkSize++;

    return actualChunkSize;
}

IFF_Long IFF_addChunkSize(const IFF_Long chunkSize, const IFF_Chunk *chunk)
{
    return chunkSize + IFF_computeActualChunkSize(chunk);
}

void IFF_increaseChunkSizeByValue(IFF_Chunk *chunk, IFF_Long increment)
{
    chunk->chunkSize += increment;

    if(chunk->parent != NULL)
        IFF_increaseChunkSizeByValue(chunk->parent, increment);
}

void IFF_increaseChunkSize(IFF_Chunk *chunk, const IFF_Chunk *attachedChunk)
{
    IFF_Long actualChunkSize = IFF_computeActualChunkSize(attachedChunk);
    IFF_increaseChunkSizeByValue(chunk, actualChunkSize);
}

void IFF_decreaseChunkSizeByValue(IFF_Chunk *chunk, IFF_Long decrement)
{
    chunk->chunkSize -= decrement;

    if(chunk->parent != NULL)
        IFF_decreaseChunkSizeByValue(chunk->parent, decrement);
}

void IFF_decreaseChunkSize(IFF_Chunk *chunk, const IFF_Chunk *attachedChunk)
{
    IFF_Long actualChunkSize = IFF_computeActualChunkSize(attachedChunk);
    IFF_decreaseChunkSizeByValue(chunk, actualChunkSize);
}

void IFF_attachSubChunkToChunk(IFF_Chunk *chunk, IFF_Chunk *subChunk)
{
    subChunk->parent = chunk;
    IFF_increaseChunkSize(chunk, subChunk);
}

void IFF_detachSubChunkFromChunk(IFF_Chunk *obsoleteChunk)
{
    if(obsoleteChunk != NULL && obsoleteChunk->parent != NULL)
    {
        IFF_decreaseChunkSize(obsoleteChunk->parent, obsoleteChunk);
        obsoleteChunk->parent = NULL;
    }
}

void IFF_replaceSubChunkOfChunk(IFF_Chunk *chunk, IFF_Chunk *obsoleteChunk, IFF_Chunk *subChunk)
{
    IFF_detachSubChunkFromChunk(obsoleteChunk);
    IFF_attachSubChunkToChunk(chunk, subChunk);
}

void *IFF_addArrayElementToChunk(IFF_Chunk *chunk, void **array, const IFF_Long elementSize, IFF_Long *arrayLength)
{
    chunk->chunkSize += elementSize;
    return IFF_addElementToArray(array, elementSize, arrayLength);
}

void *IFF_removeArrayElementFromChunk(IFF_Chunk *chunk, void *array, const IFF_Long elementSize, const unsigned int index, IFF_Long *arrayLength)
{
    chunk->chunkSize -= elementSize;
    return IFF_removeElementFromArrayByIndex(array, elementSize, index, arrayLength);
}
