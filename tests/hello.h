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

typedef struct
{
    IFF_Group *parent;
    
    IFF_ID chunkId;
    IFF_Long chunkSize;
    
    IFF_UByte a;
    IFF_UByte b;
    IFF_UWord c;
}
TEST_Hello;

TEST_Hello *TEST_createHello(void);

IFF_Chunk *TEST_readHello(FILE *file, IFF_Long chunkSize);

int TEST_writeHello(FILE *file, const IFF_Chunk *chunk);

int TEST_checkHello(const IFF_Chunk *chunk);

void TEST_freeHello(IFF_Chunk *chunk);

void TEST_printHello(const IFF_Chunk *chunk, unsigned int indentLevel);

int TEST_compareHello(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#endif
