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

#include "iff.h"
#include "chunk.h"
#include "cat.h"
#include "form.h"
#include "rawchunk.h"
#include "id.h"

#define HELO_1_BYTES_SIZE 4
#define BYE_1_BYTES_SIZE 3
#define HELO_2_BYTES_SIZE 5
#define BYE_2_BYTES_SIZE 4

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk = IFF_read("cat.TEST", NULL, 0);
    
    if(chunk == NULL)
	return 1;
    else
    {
	int status = 0;
	
	/* Check whether the structure is a CAT */
	if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
	{
	    IFF_CAT *cat = (IFF_CAT*)chunk;
	    
	    if(IFF_compareId(cat->contentsType, "TEST") != 0)
	    {
		fprintf(stderr, "contentsType of CAT must be: TEST\n");
		status = 1;
	    }
	    
	    if(cat->chunkLength == 2)
	    {
		if(IFF_compareId(cat->chunk[0]->chunkId, "FORM") == 0)
		{
		    IFF_Form *form = (IFF_Form*)cat->chunk[0];
		    
		    if(IFF_compareId(form->formType, "TEST") != 0)
		    {
			fprintf(stderr, "formType of FORM must be: TEST\n");
			status = 1;
		    }
		    
		    if(form->chunkLength == 2)
		    {
			IFF_RawChunk *heloChunk = (IFF_RawChunk*)form->chunk[0];
			IFF_RawChunk *byeChunk = (IFF_RawChunk*)form->chunk[1];
			
			if(IFF_compareId(heloChunk->chunkId, "HELO") == 0)
			{
			    if(heloChunk->chunkSize == HELO_1_BYTES_SIZE)
			    {
				if(heloChunk->chunkData[0] != 'a' ||
				   heloChunk->chunkData[1] != 'b' ||
				   heloChunk->chunkData[2] != 'c' ||
				   heloChunk->chunkData[3] != 'd')
				{
				    fprintf(stderr, "'HELO' body should consists of 'a','b','c','d'\n");
				    status = 1;
				}
			    }
			    else
			    {
				fprintf(stderr, "Size of 'HELO' chunk must be: %u!\n", HELO_1_BYTES_SIZE);
				status = 1;
			    }
			}
			else
			{
			    fprintf(stderr, "First form sub chunk must be: 'HELO'\n");
			    status = 1;
			}
			
			if(IFF_compareId(byeChunk->chunkId, "BYE ") == 0)
			{
			    if(byeChunk->chunkSize == BYE_1_BYTES_SIZE)
			    {
				if(byeChunk->chunkData[0] != 'E' ||
				   byeChunk->chunkData[1] != 'F' ||
				   byeChunk->chunkData[2] != 'G')
				{
				    fprintf(stderr, "'BYE ' body should consists of 'E','F','G'\n");
				    status = 1;
				}
			    }
			    else
			    {
				fprintf(stderr, "Size of 'BYE ' chunk must be: %u!\n", BYE_1_BYTES_SIZE);
				status = 1;
			    }
			}
			else
			{
			    fprintf(stderr, "First form sub chunk must be: 'BYE '\n");
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "FORMs must have 2 sub chunks!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "CAT subchunk must be a FORM!\n");
		    status = 1;
		}
		
		if(IFF_compareId(cat->chunk[1]->chunkId, "FORM") == 0)
		{
		    IFF_Form *form = (IFF_Form*)cat->chunk[1];
		    
		    if(IFF_compareId(form->formType, "TEST") != 0)
		    {
			fprintf(stderr, "formType of FORM must be: TEST\n");
			status = 1;
		    }
		    
		    if(form->chunkLength == 2)
		    {
			IFF_RawChunk *heloChunk = (IFF_RawChunk*)form->chunk[0];
			IFF_RawChunk *byeChunk = (IFF_RawChunk*)form->chunk[1];
			
			if(IFF_compareId(heloChunk->chunkId, "HELO") == 0)
			{
			    if(heloChunk->chunkSize == HELO_2_BYTES_SIZE)
			    {
				if(heloChunk->chunkData[0] != 'a' ||
				   heloChunk->chunkData[1] != 'b' ||
				   heloChunk->chunkData[2] != 'c' ||
				   heloChunk->chunkData[3] != 'd' ||
				   heloChunk->chunkData[4] != 'e')
				{
				    fprintf(stderr, "'HELO' body should consists of 'a','b','c','d','e'\n");
				    status = 1;
				}
			    }
			    else
			    {
				fprintf(stderr, "Size of 'HELO' chunk must be: %u!\n", HELO_2_BYTES_SIZE);
				status = 1;
			    }
			}
			else
			{
			    fprintf(stderr, "First form sub chunk must be: 'HELO'\n");
			    status = 1;
			}
			
			if(IFF_compareId(byeChunk->chunkId, "BYE ") == 0)
			{
			    if(byeChunk->chunkSize == BYE_2_BYTES_SIZE)
			    {
				if(byeChunk->chunkData[0] != 'F' ||
				   byeChunk->chunkData[1] != 'G' ||
				   byeChunk->chunkData[2] != 'H' ||
				   byeChunk->chunkData[3] != 'I')
				{
				    fprintf(stderr, "'BYE ' body should consists of 'E','F','G','H'\n");
				    status = 1;
				}
			    }
			    else
			    {
				fprintf(stderr, "Size of 'BYE ' chunk must be: %u!\n", BYE_2_BYTES_SIZE);
				status = 1;
			    }
			}
			else
			{
			    fprintf(stderr, "First form sub chunk must be: 'BYE '\n");
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "FORMs must have 2 sub chunks!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "CAT subchunk must be a FORM!\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "We must have 2 chunks in the CAT!\n");
		status = 1;
	    }
	}
	else
	{
	    fprintf(stderr, "File needs to be a CAT!\n");
	    status = 1;
	}
	
	IFF_free(chunk, NULL, 0);
	
	return status;
    }
}
