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

#define HELO_BYTES_SIZE 4
#define BYE_1_BYTES_SIZE 4
#define BYE_2_BYTES_SIZE 4

IFF_UByte heloData[] = {'q', 'w', 'e', 'r'};
IFF_UByte bye1Data[] = {'a', 'b', 'c', 'd'};
IFF_UByte bye2Data[] = {'E', 'F', 'G', 'H'};

IFF_List *IFF_createTestList()
{
    IFF_Prop *testProp;
    IFF_UByte *heloBytes, *bye1Bytes, *bye2Bytes;
    IFF_RawChunk *heloChunk, *bye1Chunk, *bye2Chunk;
    IFF_Form *test1Form, *test2Form;
    IFF_List *list;
    
    heloBytes = (IFF_UByte*)malloc(HELO_BYTES_SIZE * sizeof(IFF_UByte));
    memcpy(heloBytes, heloData, HELO_BYTES_SIZE);
    
    heloChunk = IFF_createRawChunk("HELO");
    IFF_setRawChunkData(heloChunk, heloBytes, HELO_BYTES_SIZE);
    
    testProp = IFF_createProp("TEST");
    IFF_addToProp(testProp, (IFF_Chunk*)heloChunk);
    
    bye1Bytes = (IFF_UByte*)malloc(BYE_1_BYTES_SIZE * sizeof(IFF_UByte));
    memcpy(bye1Bytes, bye1Data, BYE_1_BYTES_SIZE);
    
    bye1Chunk = IFF_createRawChunk("BYE ");
    IFF_setRawChunkData(bye1Chunk, bye1Bytes, BYE_1_BYTES_SIZE);
    
    test1Form = IFF_createForm("TEST");
    IFF_addToForm(test1Form, (IFF_Chunk*)bye1Chunk);
    
    bye2Bytes = (IFF_UByte*)malloc(BYE_2_BYTES_SIZE * sizeof(IFF_UByte));
    memcpy(bye2Bytes, bye2Data, BYE_2_BYTES_SIZE);
    
    bye2Chunk = IFF_createRawChunk("BYE ");
    IFF_setRawChunkData(bye2Chunk, bye2Bytes, BYE_2_BYTES_SIZE);
    
    test2Form = IFF_createForm("TEST");
    IFF_addToForm(test2Form, (IFF_Chunk*)bye2Chunk);
    
    list = IFF_createList("TEST");
    IFF_addPropToList(list, testProp);
    IFF_addToList(list, (IFF_Chunk*)test1Form);
    IFF_addToList(list, (IFF_Chunk*)test2Form);
    
    return list;
}
