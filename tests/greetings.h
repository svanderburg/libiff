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

#ifndef __TEST_GREETINGS_H
#define __TEST_GREETINGS_H
#include <ifftypes.h>
#include <chunk.h>
#include "greet.h"

#define TEST_ID_GRTS IFF_MAKEID('G', 'R', 'T', 'S')

extern IFF_ChunkInterface TEST_greetingsInterface;

typedef struct
{
    IFF_Chunk *parent;
    IFF_ChunkInterface *chunkInterface;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    IFF_Long greetsLength;
    TEST_Greet *greets;
}
TEST_Greetings;

TEST_Greetings *TEST_deriveGreetingsChunk(const IFF_Chunk *chunk);

TEST_Greetings *TEST_createGreetings(void);

TEST_Greet *TEST_addGreetToGreetings(TEST_Greetings *greetings);

void TEST_removeGreetFromGreetings(TEST_Greetings *greetings, const unsigned int index);

IFF_Chunk *TEST_parseGreetingsContents(FILE *file, IFF_Chunk *chunk, const IFF_Registry *registry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_Bool TEST_writeGreetingsContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_QualityLevel TEST_checkGreetingsContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

void TEST_clearGreetingsContents(IFF_Chunk *chunk);

void TEST_printGreetingsContents(FILE *file, const IFF_Chunk *chunk, unsigned int indentLevel);

IFF_Bool TEST_compareGreetingsContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#endif
