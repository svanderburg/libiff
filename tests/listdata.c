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

#include "listdata.h"
#include <stdlib.h>
#include <string.h>
#include <form.h>
#include <prop.h>
#include <rawchunk.h>
#include <id.h>

#define HELO_BYTES_SIZE 4
#define BYE_1_BYTES_SIZE 4
#define BYE_2_BYTES_SIZE 4

#define ID_HELO IFF_MAKEID('H', 'E', 'L', 'O')
#define ID_BYE IFF_MAKEID('B', 'Y', 'E', ' ')
#define ID_TEST IFF_MAKEID('T', 'E', 'S', 'T')

IFF_UByte heloData[] = {'q', 'w', 'e', 'r'};
IFF_UByte bye1Data[] = {'a', 'b', 'c', 'd'};
IFF_UByte bye2Data[] = {'E', 'F', 'G', 'H'};

static IFF_Chunk *createTestDataChunk(const IFF_ID chunkId, const IFF_Long chunkSize, IFF_UByte *data)
{
    IFF_RawChunk *rawChunk = IFF_createRawChunk(chunkId, chunkSize);
    IFF_copyDataToRawChunkData(rawChunk, data);

    return (IFF_Chunk*)rawChunk;
}

static IFF_Chunk *createHeloChunk(void)
{
    return createTestDataChunk(ID_HELO, HELO_BYTES_SIZE, heloData);
}

static IFF_Chunk *createBye1Chunk(void)
{
    return createTestDataChunk(ID_BYE, BYE_1_BYTES_SIZE, bye1Data);
}

static IFF_Chunk *createBye2Chunk(void)
{
    return createTestDataChunk(ID_BYE, BYE_2_BYTES_SIZE, bye2Data);
}

static IFF_Prop *createTestProp(void)
{
    IFF_Chunk *heloChunk = createHeloChunk();
    IFF_Prop *testProp = IFF_createEmptyProp(ID_TEST);

    IFF_addToProp(testProp, heloChunk);

    return testProp;
}

static IFF_Chunk *createTestForm(IFF_Chunk *byeChunk)
{
    IFF_Form *testForm = IFF_createEmptyForm(ID_TEST);
    IFF_addToForm(testForm, byeChunk);
    return (IFF_Chunk*)testForm;
}

static IFF_Chunk *createTest1Form(void)
{
    IFF_Chunk *bye1Chunk = createBye1Chunk();
    return createTestForm(bye1Chunk);
}

static IFF_Chunk *createTest2Form(void)
{
    IFF_Chunk *bye2Chunk = createBye2Chunk();
    return createTestForm(bye2Chunk);
}

IFF_List *IFF_createTestList(void)
{
    IFF_Prop *testProp = createTestProp();
    IFF_Chunk *test1Form = createTest1Form();
    IFF_Chunk *test2Form = createTest2Form();
    IFF_List *list = IFF_createEmptyListWithContentsType(ID_TEST);

    IFF_addPropToList(list, testProp);
    IFF_addToList(list, test1Form);
    IFF_addToList(list, test2Form);

    return list;
}
