/**
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

#include "iff.h"
#include <stdlib.h>
#include "form.h"
#include "rawchunk.h"

#define HELLO_BYTES_SIZE 4
#define BYE_BYTES_SIZE 5

int main(int argc, char *argv[])
{
    IFF_RawChunk *heloChunk, *byeChunk;
    IFF_UByte *heloBytes, *byeBytes;
    int status;
    
    IFF_Form *form = IFF_createForm("TEST");
    
    heloChunk = IFF_createRawChunk("HELO");
    
    heloBytes = (IFF_UByte*)malloc(HELLO_BYTES_SIZE * sizeof(IFF_UByte));
    heloBytes[0] = 'a';
    heloBytes[1] = 'b';
    heloBytes[2] = 'c';
    heloBytes[3] = 'd';
    
    IFF_setRawChunkData(heloChunk, heloBytes, HELLO_BYTES_SIZE);
    
    IFF_addToForm(form, (IFF_Chunk*)heloChunk);
    
    byeChunk = IFF_createRawChunk("BYE ");
    
    byeBytes = (IFF_UByte*)malloc(BYE_BYTES_SIZE * sizeof(IFF_UByte));
    byeBytes[0] = 'E';
    byeBytes[1] = 'F';
    byeBytes[2] = 'G';
    byeBytes[3] = 'H';
    byeBytes[4] = 'I';
    
    IFF_setRawChunkData(byeChunk, byeBytes, BYE_BYTES_SIZE);
    
    IFF_addToForm(form, (IFF_Chunk*)byeChunk);
    
    status = IFF_write("hello-pad.TEST", (IFF_Chunk*)form, NULL, 0);
    
    IFF_free((IFF_Chunk*)form, NULL, 0);
    
    return (!status);
}
