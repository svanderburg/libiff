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

IFF_Chunk *IFF_createChunk(const IFF_ID chunkId, const IFF_Long chunkSize, size_t structSize, IFF_ChunkInterface *chunkInterface)
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

static IFF_Chunk *readChunkBody(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID scopeId, const IFF_Registry *registry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_ChunkInterface *chunkInterface = IFF_findChunkInterface(registry, scopeId, chunkId);
    IFF_Long bytesProcessed = 0;
    IFF_Chunk *chunk = chunkInterface->parseChunkContents(file, chunkId, chunkSize, registry, chunkInterface, attributePath, &bytesProcessed, error);

    if(*error == NULL && chunk != NULL)
    {
        if(IFF_skipUnknownBytes(file, chunk->chunkId, chunkSize, bytesProcessed, attributePath, error) &&
            IFF_readPaddingByte(file, chunkSize, chunk->chunkId, attributePath, error))
            ;
    }

    return chunk;
}

IFF_Chunk *IFF_parseChunk(FILE *file, const IFF_ID scopeId, const IFF_Registry *registry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_ID chunkId;
    IFF_Long chunkSize;

    if(!IFF_readChunkIdField(file, &chunkId, 0, attributePath, "chunkId", error)
        || !IFF_readChunkSizeField(file, &chunkSize, chunkId, attributePath, "chunkSize", error))
        return NULL;

    return readChunkBody(file, chunkId, chunkSize, scopeId, registry, attributePath, error);
}

static IFF_Bool writeChunkBody(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_Long bytesProcessed = 0;

    return chunk->chunkInterface->writeChunkContents(file, chunk, attributePath, &bytesProcessed, error)
        && IFF_writeZeroFillerBytes(file, chunk->chunkId, chunk->chunkSize, bytesProcessed, attributePath, error)
        && IFF_writePaddingByte(file, chunk->chunkSize, chunk->chunkId, attributePath, error);
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

static void printChunkIdField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_ID chunkId)
{
    IFF_printFirstField(file, indentLevel, attributeName, &chunkId, IFF_printIdValue);
}

void IFF_printChunk(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    fputs("{\n", file);
    printChunkIdField(file, indentLevel + 1, "chunkId", chunk->chunkId);
    IFF_printLongField(file, indentLevel + 1, "chunkSize", chunk->chunkSize);
    chunk->chunkInterface->printChunkContents(file, chunk, indentLevel + 1);
    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

IFF_Bool IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    if(chunk1->chunkId == chunk2->chunkId && chunk1->chunkSize == chunk2->chunkSize)
        return chunk1->chunkInterface->compareChunkContents(chunk1, chunk2);
    else
        return FALSE;
}

IFF_Bool IFF_traverseChunkHierarchy(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk)
{
    if(chunk->chunkInterface->traverseChunkHierarchy == NULL)
        return TRUE;
    else
        return chunk->chunkInterface->traverseChunkHierarchy(chunk, data, visitChunk);
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
