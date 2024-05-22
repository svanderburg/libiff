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

#ifndef __TEST_HELLO_H
#define __TEST_HELLO_H

#include <ifftypes.h>
#include <chunk.h>
#include <stdio.h>
#include <id.h>

#define TEST_ID_HELO IFF_MAKEID('H', 'E', 'L', 'O')
#define TEST_HELO_DEFAULT_SIZE (2 * sizeof(IFF_UByte) + sizeof(IFF_UWord))

extern IFF_ChunkInterface TEST_helloInterface;

typedef struct
{
    IFF_Chunk *parent;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    IFF_UByte a;
    IFF_UByte b;
    IFF_UWord c;
}
TEST_Hello;

IFF_Chunk *TEST_createHelloChunk(const IFF_ID chunkId, const IFF_Long chunkSize);

TEST_Hello *TEST_createHello(const IFF_Long chunkSize);

IFF_Bool TEST_readHelloContents(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_Bool TEST_writeHelloContents(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_QualityLevel TEST_checkHelloContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

void TEST_clearHelloContents(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

void TEST_printHelloContents(FILE *file, const IFF_Chunk *chunk, unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

IFF_Bool TEST_compareHelloContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

#endif
