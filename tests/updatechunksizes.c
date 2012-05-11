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
#include "iff.h"
#include "chunk.h"
#include "form.h"
#include "cat.h"
#include "rawchunk.h"

#define ABCD_BYTES_SIZE 4

int main(int argc, char *argv[])
{
    IFF_UByte *chunkData;
    IFF_RawChunk *rawChunk;
    IFF_Form *form;
    IFF_CAT *cat;
    int status = 0;
    
    chunkData = (IFF_UByte*)malloc(ABCD_BYTES_SIZE * sizeof(IFF_UByte));
    chunkData[0] = 'A';
    chunkData[1] = 'B';
    chunkData[2] = 'C';
    chunkData[3] = 'D';
    
    rawChunk = IFF_createRawChunk("ABCD");
    IFF_setRawChunkData(rawChunk, chunkData, ABCD_BYTES_SIZE);
    
    form = IFF_createForm("TEST");
    IFF_addToForm(form, (IFF_Chunk*)rawChunk);
    
    cat = IFF_createCAT("TEST");
    IFF_addToCAT(cat, (IFF_Chunk*)form);
    
    /* Intentionally increase the size of the ABCD chunk */
    
    rawChunk->chunkData = (IFF_UByte*)realloc(chunkData, (ABCD_BYTES_SIZE + 1) * sizeof(IFF_UByte));
    rawChunk->chunkSize++;
    
    /* The IFF file should be invalid now as the, form chunk size is too small */
    if(!IFF_check((IFF_Chunk*)cat, NULL, 0))
    {
	/* Update the chunk sizes */
	IFF_updateChunkSizes((IFF_Chunk*)rawChunk);
	
	/* Now the IFF should be valid */
	
	if(!IFF_check((IFF_Chunk*)cat, NULL, 0))
	    status = 1;
    }
    else
    {
	fprintf(stderr, "The IFF file should be invalid!\n");
	status = 1;
    }
    
    IFF_free((IFF_Chunk*)cat, NULL, 0);
    
    return status;
}
