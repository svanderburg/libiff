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
#include <chunk.h>
#include <form.h>
#include <rawchunk.h>
#include <id.h>
#include "nestedformdata.h"

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk = IFF_read("nestedform.TEST", NULL, 0);
    
    if(chunk == NULL)
	return 1;
    else
    {
	int status = 0;
	
	/* Check whether the structure is a FORM */
	if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	{
	    IFF_Form *outerForm = (IFF_Form*)chunk;
	    
	    if(IFF_compareId(outerForm->formType, "BLA ") != 0)
	    {
		fprintf(stderr, "formType must be: 'BLA '\n");
		status = 1;
	    }
	    
	    if(outerForm->chunkLength == 2)
	    {
		if(IFF_compareId(outerForm->chunk[0]->chunkId, "FORM") == 0)
		{
		    IFF_Form *form = (IFF_Form*)outerForm->chunk[0];
		    
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
				if(memcmp(heloChunk->chunkData, helo1Data, HELO_1_BYTES_SIZE) != 0)
				{
				    unsigned int i;
				    
				    fprintf(stderr, "'HELO' body should consists of '");
				    
				    for(i = 0; i < HELO_1_BYTES_SIZE; i++)
					fprintf(stderr, "%c", helo1Data[i]);
				    
				    fprintf(stderr, "'\n");
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
				if(memcmp(byeChunk->chunkData, bye1Data, BYE_1_BYTES_SIZE) != 0)
				{
				    unsigned int i;
				    
				    fprintf(stderr, "'BYE ' body should consists of '");
				    
				    for(i = 0; i < BYE_1_BYTES_SIZE; i++)
					fprintf(stderr, "%c", bye1Data[i]);
				    
				    fprintf(stderr, "'\n");
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
		    fprintf(stderr, "FORM subchunk must be a FORM!\n");
		    status = 1;
		}
		
		if(IFF_compareId(outerForm->chunk[1]->chunkId, "FORM") == 0)
		{
		    IFF_Form *form = (IFF_Form*)outerForm->chunk[1];
		    
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
				if(memcmp(heloChunk->chunkData, helo2Data, HELO_2_BYTES_SIZE) != 0)
				{
				    unsigned int i;
				    
				    fprintf(stderr, "'HELO' body should consists of '");
				    
				    for(i = 0; i < HELO_2_BYTES_SIZE; i++)
					fprintf(stderr, "%c", helo2Data[i]);
				    
				    fprintf(stderr, "'\n");
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
				if(memcmp(byeChunk->chunkData, bye2Data, BYE_2_BYTES_SIZE) != 0)
				{
				    unsigned int i;
				    
				    fprintf(stderr, "'BYE ' body should consists of '");
				    
				    for(i = 0; i < BYE_2_BYTES_SIZE; i++)
					fprintf(stderr, "%c", bye2Data[i]);
				    
				    fprintf(stderr, "'\n");
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
		    fprintf(stderr, "FORM subchunk must be a FORM!\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "We must have 2 chunks in the FORM!\n");
		status = 1;
	    }
	}
	else
	{
	    fprintf(stderr, "File must be a FORM!\n");
	    status = 1;
	}
	
	IFF_free(chunk, NULL, 0);
	
	return status;
    }
}
