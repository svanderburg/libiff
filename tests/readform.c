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
#include <string.h>
#include <iff.h>
#include <form.h>
#include <rawchunk.h>
#include <id.h>
#include "formdata.h"

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk = IFF_read("hello.TEST", NULL, 0);
    
    if(chunk == NULL)
	return 1;
    else
    {
	int status = 0;
	
	/* Check whether the structure is a FORM */
	if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	{
	    IFF_Form *form = (IFF_Form*)chunk;
	    
	    /* Form type must be TEST */
	    
	    if(IFF_compareId(form->formType, "TEST") != 0)
	    {
		fprintf(stderr, "formType must be: TEST\n");
		status = 1;
	    }
	    
	    /* We need to have 2 sub chunks in the FORM */
	    
	    if(form->chunkLength == 2)
	    {
		IFF_RawChunk *heloChunk = (IFF_RawChunk*)form->chunk[0];
		IFF_RawChunk *byeChunk = (IFF_RawChunk*)form->chunk[1];
		
		/* Check whether first sub chunk has ID 'HELO' */
		
		if(IFF_compareId(heloChunk->chunkId, "HELO") == 0)
		{
		    /* Chunk size has to be 4 bytes */
		    
		    if(heloChunk->chunkSize == HELO_BYTES_SIZE)
		    {
			if(memcmp(heloChunk->chunkData, heloData, HELO_BYTES_SIZE) != 0)
			{
			    unsigned int i;
			    fprintf(stderr, "'HELO' chunk body should consist of: '");
			    
			    for(i = 0; i < HELO_BYTES_SIZE; i++)
				fprintf(stderr, "%c", heloData[i]);
			    
			    fprintf(stderr, "'\n");
			    
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "Size of 'HELO' chunk must be: %u!\n", HELO_BYTES_SIZE);
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "First chunk has to be of ID: 'HELO'\n");
		    status = 1;
		}
		
		/* Check whether second sub chunk has ID 'BYE ' */
		
		if(IFF_compareId(byeChunk->chunkId, "BYE ") == 0)
		{
		    /* Chunk size has to be 4 bytes */
		    
		    if(byeChunk->chunkSize == BYE_BYTES_SIZE)
		    {
			if(memcmp(byeChunk->chunkData, byeData, BYE_BYTES_SIZE) != 0)
			{
			    unsigned int i;
			    fprintf(stderr, "'BYE ' chunk body should consist of '");
			    
			    for(i = 0; i < BYE_BYTES_SIZE; i++)
				fprintf(stderr, "%c", byeData[i]);
				
			    fprintf(stderr, "'\n");
			    
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "Size of 'BYE ' chunk must be: %u!\n", BYE_BYTES_SIZE);
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "Second chunk has to be of ID: 'BYE '\n");
		    status = 1;
		}
	    }
	    else
	    {
		status = 1;
		fprintf(stderr, "We need 2 sub chunks in the FORM!\n");
	    }
	}
	else
	{
	    fprintf(stderr, "File needs to be a FORM!\n");
	    status = 1;
	}
	
	IFF_free(chunk, NULL, 0);
	
	return status;
    }
}
