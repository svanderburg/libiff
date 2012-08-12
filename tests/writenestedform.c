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

#include <stdlib.h>
#include <string.h>
#include <iff.h>
#include <form.h>
#include <rawchunk.h>
#include <id.h>
#include "nestedformdata.h"

int main(int argc, char *argv[])
{
    IFF_Form *test1Form, *test2Form, *outerForm;
    IFF_RawChunk *helo1Chunk, *helo2Chunk, *bye1Chunk, *bye2Chunk;
    IFF_UByte *helo1Bytes, *helo2Bytes, *bye1Bytes, *bye2Bytes;
    int status;
    
    helo1Bytes = (IFF_UByte*)malloc(HELO_1_BYTES_SIZE * sizeof(IFF_UByte));
    memcpy(helo1Bytes, helo1Data, HELO_1_BYTES_SIZE);

    helo1Chunk = IFF_createRawChunk("HELO");
    IFF_setRawChunkData(helo1Chunk, helo1Bytes, HELO_1_BYTES_SIZE);
    
    bye1Bytes = (IFF_UByte*)malloc(BYE_1_BYTES_SIZE * sizeof(IFF_UByte));
    memcpy(bye1Bytes, bye1Data, BYE_1_BYTES_SIZE);
    
    bye1Chunk = IFF_createRawChunk("BYE ");
    IFF_setRawChunkData(bye1Chunk, bye1Bytes, BYE_1_BYTES_SIZE);
    
    test1Form = IFF_createForm("TEST");
    IFF_addToForm(test1Form, (IFF_Chunk*)helo1Chunk);
    IFF_addToForm(test1Form, (IFF_Chunk*)bye1Chunk);
    
    helo2Bytes = (IFF_UByte*)malloc(HELO_2_BYTES_SIZE * sizeof(IFF_UByte));
    memcpy(helo2Bytes, helo2Data, HELO_2_BYTES_SIZE);
    
    helo2Chunk = IFF_createRawChunk("HELO");
    IFF_setRawChunkData(helo2Chunk, helo2Bytes, HELO_2_BYTES_SIZE);
    
    bye2Bytes = (IFF_UByte*)malloc(BYE_2_BYTES_SIZE * sizeof(IFF_UByte));
    memcpy(bye2Bytes, bye2Data, BYE_2_BYTES_SIZE);
    
    bye2Chunk = IFF_createRawChunk("BYE ");
    IFF_setRawChunkData(bye2Chunk, bye2Bytes, BYE_2_BYTES_SIZE);
    
    test2Form = IFF_createForm("TEST");
    IFF_addToForm(test2Form, (IFF_Chunk*)helo2Chunk);
    IFF_addToForm(test2Form, (IFF_Chunk*)bye2Chunk);
    
    outerForm = IFF_createForm("BLA ");
    IFF_addToForm(outerForm, (IFF_Chunk*)test1Form);
    IFF_addToForm(outerForm, (IFF_Chunk*)test2Form);
    
    status = IFF_write("nestedform.TEST", (IFF_Chunk*)outerForm, NULL, 0);
    
    IFF_free((IFF_Chunk*)outerForm, NULL, 0);
    
    return (!status);
}
