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

#include "datachunk.h"
#include "rawchunk.h"
#include "extensionchunk.h"

IFF_Chunk *IFF_createDataChunk(const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunkId, chunkRegistry);

    if(chunkType == NULL)
        return (IFF_Chunk*)IFF_createRawChunk(chunkId, chunkSize);
    else
        return IFF_createExtensionChunk(chunkId, chunkSize, chunkType);
}

IFF_Bool IFF_readDataChunk(FILE *file, IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);
    IFF_Long bytesProcessed = 0; /* TODO: remove */

    if(chunkType == NULL)
        return IFF_readRawChunk(file, chunk, chunkRegistry, &bytesProcessed);
    else
        return IFF_readExtensionChunk(file, chunk, chunkRegistry, chunkType);
}

IFF_Bool IFF_writeDataChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);
    IFF_Long bytesProcessed = 0; /* TODO: remove */

    if(chunkType == NULL)
        return IFF_writeRawChunk(file, chunk, chunkRegistry, &bytesProcessed);
    else
        return IFF_writeExtensionChunk(file, chunk, chunkRegistry, chunkType);
}

IFF_Bool IFF_checkDataChunk(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);

    if(chunkType == NULL)
        return TRUE;
    else
        return IFF_checkExtensionChunk(chunk, chunkRegistry, chunkType);
}

void IFF_freeDataChunk(IFF_Chunk *chunk, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);

    if(chunkType == NULL)
        IFF_freeRawChunk(chunk, chunkRegistry);
    else
        IFF_freeExtensionChunk(chunk, chunkRegistry, chunkType);
}

void IFF_printDataChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk->chunkId, chunkRegistry);

    if(chunkType == NULL)
        IFF_printRawChunk(chunk, indentLevel + 1, chunkRegistry);
    else
        IFF_printExtensionChunk(chunk, indentLevel + 1, chunkRegistry, chunkType);
}

IFF_Bool IFF_compareDataChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_ChunkType *chunkType = IFF_findChunkType(formType, chunk1->chunkId, chunkRegistry);

    if(chunkType == NULL)
        return IFF_compareRawChunk(chunk1, chunk2, chunkRegistry);
    else
        return IFF_compareExtensionChunk(chunk1, chunk2, chunkRegistry, chunkType);
}
