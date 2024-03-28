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
#include "id.h"
#include "util.h"
#include "error.h"

IFF_Chunk *IFF_createChunk(const IFF_ID chunkId, const IFF_Long chunkSize, size_t structSize)
{
    IFF_Chunk *chunk = (IFF_Chunk*)malloc(structSize);

    if(chunk != NULL)
    {
        chunk->parent = NULL;
        chunk->chunkId = chunkId;
        chunk->chunkSize = chunkSize;
    }

    return chunk;
}

static IFF_Chunk *readChunkBody(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_ChunkType *chunkType = IFF_findChunkType(chunkRegistry, formType, chunkId);
    IFF_Chunk *chunk = chunkType->createExtensionChunk(chunkId, chunkSize);

    if(chunk != NULL)
    {
        IFF_Long bytesProcessed = 0;

        /* Read remaining bytes (procedure depends on chunk id type) */
        if(!chunkType->readExtensionChunkFields(file, chunk, chunkRegistry, attributePath, &bytesProcessed, error)
            || !IFF_skipUnknownBytes(file, chunk->chunkId, chunkSize, bytesProcessed, attributePath, error)
            || !IFF_readPaddingByte(file, chunkSize, chunk->chunkId, attributePath, error))
            return chunk;
    }

    return chunk;
}

IFF_Chunk *IFF_readChunk(FILE *file, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_ID chunkId;
    IFF_Long chunkSize;

    if(!IFF_readId(file, &chunkId, attributePath, "chunkId", 0, error)
        || !IFF_readLong(file, &chunkSize, attributePath, "chunkSize", chunkId, error))
        return NULL;

    return readChunkBody(file, chunkId, chunkSize, formType, chunkRegistry, attributePath, error);
}

static IFF_Bool writeChunkBody(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    IFF_ChunkType *chunkType = IFF_findChunkType(chunkRegistry, formType, chunk->chunkId);
    IFF_Long bytesProcessed = 0;

    return chunkType->writeExtensionChunkFields(file, chunk, chunkRegistry, attributePath, &bytesProcessed, error)
        && IFF_writeZeroFillerBytes(file, chunk->chunkId, chunk->chunkSize, bytesProcessed, attributePath, error)
        && IFF_writePaddingByte(file, chunk->chunkSize, chunk->chunkId, attributePath, error);
}

IFF_Bool IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    return IFF_writeId(file, chunk->chunkId, attributePath, "chunkId", chunk->chunkId, error)
        && IFF_writeLong(file, chunk->chunkSize, attributePath, "chunkSize", chunk->chunkId, error)
        && writeChunkBody(file, chunk, formType, chunkRegistry, attributePath, error);
}

IFF_Bool IFF_checkChunk(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessage printCheckMessage, void *data)
{
    if(!IFF_checkId(chunk->chunkId, attributePath, "chunkId", printCheckMessage, data, 0))
        return FALSE;
    else
    {
        IFF_ChunkType *chunkType = IFF_findChunkType(chunkRegistry, formType, chunk->chunkId);
        return chunkType->checkExtensionChunk(chunk, chunkRegistry, attributePath, printCheckMessage, data);
    }
}

void IFF_freeChunk(IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    if(chunk != NULL)
    {
        IFF_ChunkType *chunkType = IFF_findChunkType(chunkRegistry, formType, chunk->chunkId);
        chunkType->freeExtensionChunk(chunk, chunkRegistry);
        free(chunk);
    }
}

void IFF_printChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_ChunkType *chunkType = IFF_findChunkType(chunkRegistry, formType, chunk->chunkId);

    IFF_printIndent(stdout, indentLevel, "'");
    IFF_printId(chunk->chunkId);
    printf("' = {\n");
    IFF_printIndent(stdout, indentLevel + 1, "chunkSize = %d;\n", chunk->chunkSize);
    chunkType->printExtensionChunk(chunk, indentLevel + 1, chunkRegistry);
    IFF_printIndent(stdout, indentLevel, "}\n\n");
}

IFF_Bool IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    if(chunk1->chunkId == chunk2->chunkId && chunk1->chunkSize == chunk2->chunkSize)
    {
        IFF_ChunkType *chunkType = IFF_findChunkType(chunkRegistry, formType, chunk1->chunkId);
        return chunkType->compareExtensionChunk(chunk1, chunk2, chunkRegistry);
    }
    else
        return FALSE;
}
