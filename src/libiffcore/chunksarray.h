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

#ifndef __IFF_CHUNKSARRAY_H
#define __IFF_CHUNKSARRAY_H
#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "attributepath.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

IFF_Chunk **IFF_addChunkToChunksArray(IFF_Chunk **chunks, unsigned int *chunksLength, IFF_Chunk *chunk);

IFF_Chunk *IFF_replaceChunkInChunksArrayByIndex(IFF_Chunk **chunks, const unsigned int chunksLength, const unsigned int index, IFF_Chunk *chunk);

IFF_Chunk **IFF_removeChunkFromChunksArrayByIndex(IFF_Chunk **chunks, unsigned int *chunksLength, const unsigned int index, IFF_Chunk **obsoleteChunk);

IFF_Chunk **IFF_appendChunksArrayToChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength, IFF_Chunk **appendChunks, const unsigned int appendChunksLength, unsigned int *resultChunksLength);

IFF_Bool IFF_writeChunksArray(FILE *file, IFF_Chunk **chunks, const unsigned int chunksLength, const IFF_ID scopeId, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_QualityLevel IFF_checkChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength, const IFF_ID scopeId, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

void IFF_freeChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength, const IFF_ID scopeId);

void IFF_printChunksArray(FILE *file, IFF_Chunk **chunks, const unsigned int chunksLength, const unsigned int indentLevel, const IFF_ID scopeId);

IFF_Bool IFF_compareChunksArray(IFF_Chunk **chunks1, const unsigned int chunks1Length, IFF_Chunk **chunks2, const unsigned int chunks2Length, const IFF_ID scopeId);

IFF_Bool IFF_traverseChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength, const IFF_ID scopeId, void *data, IFF_visitChunkFunction visitChunk);

IFF_Long IFF_addChunksArraySize(IFF_Chunk **chunks, const unsigned int chunksLength, const IFF_Long chunkSize);

#ifdef __cplusplus
}
#endif

#endif
