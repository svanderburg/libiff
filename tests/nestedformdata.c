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

#include "nestedformdata.h"
#include <stdlib.h>
#include <string.h>
#include <rawchunk.h>
#include <id.h>

#define HELO_1_BYTES_SIZE 4
#define BYE_1_BYTES_SIZE 3
#define HELO_2_BYTES_SIZE 5
#define BYE_2_BYTES_SIZE 4

#define ID_HELO IFF_MAKEID('H', 'E', 'L', 'O')
#define ID_BYE IFF_MAKEID('B', 'Y', 'E', ' ')
#define ID_TEST IFF_MAKEID('T', 'E', 'S', 'T')
#define ID_BLA IFF_MAKEID('B', 'L', 'A', ' ')

IFF_UByte helo1Data[] = {'a', 'b', 'c', 'd'};
IFF_UByte bye1Data[] = {'E', 'F', 'G'};
IFF_UByte helo2Data[] = {'a', 'b', 'c', 'd', 'e'};
IFF_UByte bye2Data[] = {'F', 'G', 'H', 'I'};

static IFF_Chunk *createTestDataChunk(const IFF_ID chunkId, const IFF_Long chunkSize, IFF_UByte *data)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createRawChunk(chunkId, chunkSize);
    IFF_copyDataToRawChunkData(rawChunk, data);

    return (IFF_Chunk*)rawChunk;
}

static IFF_Chunk *createHelo1Chunk(void)
{
    return createTestDataChunk(ID_HELO, HELO_1_BYTES_SIZE, helo1Data);
}

static IFF_Chunk *createBye1Chunk(void)
{
    return createTestDataChunk(ID_BYE, BYE_1_BYTES_SIZE, bye1Data);
}

static IFF_Chunk *createHelo2Chunk(void)
{
    return createTestDataChunk(ID_HELO, HELO_2_BYTES_SIZE, helo2Data);
}

static IFF_Chunk *createBye2Chunk(void)
{
    return createTestDataChunk(ID_BYE, BYE_2_BYTES_SIZE, bye2Data);
}

static IFF_Chunk *createTestForm(IFF_Chunk *heloChunk, IFF_Chunk *byeChunk)
{
    IFF_Form *testForm = IFF_createEmptyForm(ID_TEST, NULL);

    IFF_addChunkToForm(testForm, heloChunk);
    IFF_addChunkToForm(testForm, byeChunk);

    return (IFF_Chunk*)testForm;
}

static IFF_Chunk *createTest1Form(void)
{
    IFF_Chunk *helo1Chunk = createHelo1Chunk();
    IFF_Chunk *bye1Chunk = createBye1Chunk();

    return createTestForm(helo1Chunk, bye1Chunk);
}

static IFF_Chunk *createTest2Form(void)
{
    IFF_Chunk *helo2Chunk = createHelo2Chunk();
    IFF_Chunk *bye2Chunk = createBye2Chunk();

    return createTestForm(helo2Chunk, bye2Chunk);
}

IFF_Form *IFF_createTestForm(void)
{
    IFF_Chunk *test1Form = createTest1Form();
    IFF_Chunk *test2Form = createTest2Form();
    IFF_Form *outerForm = IFF_createEmptyForm(ID_BLA, NULL);

    IFF_addChunkToForm(outerForm, test1Form);
    IFF_addChunkToForm(outerForm, test2Form);

    return outerForm;
}
