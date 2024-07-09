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

#include "formdata.h"
#include <stdlib.h>
#include <string.h>
#include <rawchunk.h>
#include <id.h>

#define ID_TEST IFF_MAKEID('T', 'E', 'S', 'T')
#define ID_HELO IFF_MAKEID('H', 'E', 'L', 'O')
#define ID_BYE  IFF_MAKEID('B', 'Y', 'E', ' ')

#define HELO_BYTES_SIZE 4
#define BYE_BYTES_SIZE 4

static IFF_UByte heloData[] = {'a', 'b', 'c', 'd'};
static IFF_UByte byeData[] = {'E', 'F', 'G', 'H'};

static IFF_Chunk *createTestDataChunk(const IFF_ID chunkId, const IFF_Long chunkSize, IFF_UByte *data)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createRawChunk(chunkId, chunkSize);
    IFF_copyDataToRawChunkData(rawChunk, data);

    return (IFF_Chunk*)rawChunk;
}

static IFF_Chunk *createHelloChunk(void)
{
    return createTestDataChunk(ID_HELO, HELO_BYTES_SIZE, heloData);
}

static IFF_Chunk *createByeChunk(void)
{
    return createTestDataChunk(ID_BYE, BYE_BYTES_SIZE, byeData);
}

IFF_Form *IFF_createTestForm(void)
{
    IFF_Form *form = IFF_createEmptyForm(ID_TEST);
    IFF_Chunk *heloChunk = createHelloChunk();
    IFF_Chunk *byeChunk = createByeChunk();

    IFF_addChunkToForm(form, heloChunk);
    IFF_addChunkToForm(form, byeChunk);

    return form;
}
