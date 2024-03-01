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

#define ID_EMPTY IFF_MAKEID(' ', ' ', ' ', ' ')

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

static IFF_Bool skipUnknownBytes(FILE *file, const IFF_Chunk *chunk, const IFF_Long bytesProcessed)
{
    if(bytesProcessed < chunk->chunkSize)
    {
        long bytesToSkip = chunk->chunkSize - bytesProcessed;

        if(fseek(file, bytesToSkip, SEEK_CUR) == 0)
        {
            IFF_error("Cannot skip: %d bytes in data chunk: '", bytesToSkip);
            IFF_errorId(chunk->chunkId);
            IFF_error("'\n");
            return TRUE;
        }
        else
            return FALSE;
    }
    else
        return TRUE;
}

IFF_Chunk *IFF_readChunk(FILE *file, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_ID chunkId;
    IFF_Long chunkSize;
    IFF_Chunk *chunk;
    IFF_Long bytesProcessed = 0;
    IFF_ChunkType *chunkType;

    /* Read chunk id */
    if(!IFF_readId(file, &chunkId, ID_EMPTY, ""))
        return NULL;

    /* Read chunk size */
    if(!IFF_readLong(file, &chunkSize, chunkId, "chunkSize"))
        return NULL;

    /* Query chunk type */
    chunkType = IFF_findChunkType(formType, chunkId, chunkRegistry);

    /* Create the chunk */
    chunk = chunkType->createExtensionChunk(chunkId, chunkSize);

    if(chunk == NULL)
        return NULL;

    /* Read remaining bytes (procedure depends on chunk id type) */

    if(!chunkType->readExtensionChunkFields(file, chunk, chunkRegistry, &bytesProcessed))
    {
        IFF_freeChunk(chunk, formType, chunkRegistry);
        return NULL;
    }

    /* Skip unknown bytes */
    if(!skipUnknownBytes(file, chunk, bytesProcessed))
    {
        IFF_freeChunk(chunk, formType, chunkRegistry);
        return NULL;
    }

    /* If the chunk size is odd, we have to read the padding byte */
    if(!IFF_readPaddingByte(file, chunkSize, chunk->chunkId))
    {
        IFF_freeChunk(chunk, formType, chunkRegistry);
        return NULL;
    }

    return chunk;
}

static IFF_Bool writeZeroFillerBytes(FILE *file, const IFF_Chunk *chunk, const IFF_Long bytesProcessed)
{
    if(bytesProcessed < chunk->chunkSize)
    {
        size_t bytesToSkip = chunk->chunkSize - bytesProcessed;
        IFF_UByte *emptyData = (IFF_UByte*)calloc(bytesToSkip, sizeof(IFF_UByte));
        IFF_Bool status = fwrite(emptyData, sizeof(IFF_UByte), bytesToSkip, file) == bytesToSkip;

        if(!status)
        {
            IFF_error("Cannot write: %u zero bytes in data chunk: '", bytesToSkip);
            IFF_errorId(chunk->chunkId);
            IFF_error("'\n");
        }

        free(emptyData);
        return status;
    }
    else
        return TRUE;
}

IFF_Bool IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_Long bytesProcessed = 0;
    IFF_ChunkType *chunkType;

    if(!IFF_writeId(file, chunk->chunkId, chunk->chunkId, "chunkId"))
        return FALSE;

    if(!IFF_writeLong(file, chunk->chunkSize, chunk->chunkId, "chunkSize"))
        return FALSE;

    /* Query chunk type */
    chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);

    if(!chunkType->writeExtensionChunkFields(file, chunk, chunkRegistry, &bytesProcessed))
        return FALSE;

    if(!writeZeroFillerBytes(file, chunk, bytesProcessed))
        return FALSE;

    /* If the chunk size is odd, we have to write the padding byte */
    if(!IFF_writePaddingByte(file, chunk->chunkSize, chunk->chunkId))
        return FALSE;

    return TRUE;
}

IFF_Bool IFF_checkChunk(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    if(!IFF_checkId(chunk->chunkId))
        return FALSE;
    else
    {
        /* Query chunk type */
        IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);

        return chunkType->checkExtensionChunk(chunk, chunkRegistry);
    }
}

void IFF_freeChunk(IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);

    chunkType->freeExtensionChunk(chunk, chunkRegistry);
    free(chunk);
}

void IFF_printChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);

    IFF_printIndent(stdout, indentLevel, "'");
    IFF_printId(chunk->chunkId);
    printf("' = {\n");

    IFF_printIndent(stdout, indentLevel + 1, "chunkSize = %d;\n", chunk->chunkSize);
    chunkType->printExtensionChunk(chunk, indentLevel, chunkRegistry);
    IFF_printIndent(stdout, indentLevel, "}\n\n");
}

IFF_Bool IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    if(chunk1->chunkId == chunk2->chunkId && chunk1->chunkSize == chunk2->chunkSize)
    {
        IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk1->chunkId, chunkRegistry);
        return chunkType->compareExtensionChunk(chunk1, chunk2, chunkRegistry);
    }
    else
        return FALSE;
}
