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

#include <stdio.h>
#include "chunk.h"
#include "id.h"
#include "form.h"
#include "rawchunk.h"
#include "iff.h"

#define HELO_BYTES_SIZE 4
#define BYE_BYTES_SIZE 4

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk = IFF_read("lookupproperty-nested.TEST", NULL, 0);

    if(chunk == NULL)
	return 1;
    else
    {
	int status = 0;
	unsigned int formsLength;
	IFF_Form **forms = IFF_searchForms(chunk, "TEST", &formsLength);
	
	if(formsLength == 2)
	{
	    IFF_Form *form = forms[1]; /* We should take the last form (in the inner list) */
	    
	    IFF_RawChunk *heloChunk = (IFF_RawChunk*)IFF_getChunkFromForm(form, "HELO");
	    IFF_RawChunk *byeChunk = (IFF_RawChunk*)IFF_getChunkFromForm(form, "BYE ");
	    
	    if(heloChunk != NULL && IFF_compareId(heloChunk->chunkId, "HELO") == 0)
	    {
		if(heloChunk->chunkSize == HELO_BYTES_SIZE)
		{
		    if(heloChunk->chunkData[0] != '1' ||
		       heloChunk->chunkData[1] != '2' ||
		       heloChunk->chunkData[2] != '3' ||
		       heloChunk->chunkData[3] != '4')
		    {
			fprintf(stderr, "Error: 'HELO' chunk contents should be: '1', '2', '3', '4'!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "Error: size of helo chunk should be: %u!\n", HELO_BYTES_SIZE);
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "Error: we should be able to find a HELO chunk!\n");
		status = 1;
	    }
	    
	    if(byeChunk != NULL && IFF_compareId(byeChunk->chunkId, "BYE ") == 0)
	    {
		if(byeChunk->chunkSize == BYE_BYTES_SIZE)
		{
		    if(byeChunk->chunkData[0] != 'q' ||
		       byeChunk->chunkData[1] != 'w' ||
		       byeChunk->chunkData[2] != 'e' ||
		       byeChunk->chunkData[3] != 'r')
		    {
			fprintf(stderr, "Error: 'BYE ' chunk contents should be: 'q', 'w', 'e', 'r'!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "Error: size of helo chunk should be: %u!\n", BYE_BYTES_SIZE);
		    status = 1;
		}
	    }
	}
	else
	{
	    fprintf(stderr, "Error: we should be able to find 2 TEST forms!\n");
	    status = 1;
	}
	
	IFF_free(chunk, NULL, 0);
	
	return status;
    }
}
