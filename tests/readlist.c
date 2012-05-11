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
#include "list.h"
#include "prop.h"
#include "form.h"
#include "rawchunk.h"
#include "id.h"

#define HELO_BYTES_SIZE 4
#define BYE_1_BYTES_SIZE 4
#define BYE_2_BYTES_SIZE 4

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk = IFF_read("list.TEST", NULL, 0);
    
    if(chunk == NULL)
	return 1;
    else
    {
	int status = 0;
	
	/* Check whether the structure is a LIST */
	if(IFF_compareId(chunk->chunkId, "LIST") == 0)
	{
	    IFF_List *list = (IFF_List*)chunk;
	    
	    if(IFF_compareId(list->contentsType, "TEST") != 0)
	    {
		fprintf(stderr, "contentsType of LIST must be: 'TEST'\n");
		status = 1;
	    }
	    
	    if(list->propLength == 1)
	    {
		IFF_Prop *prop = list->prop[0];
		
		if(IFF_compareId(prop->formType, "TEST") == 0)
		{
		    if(prop->chunkLength == 1)
		    {
			IFF_RawChunk *heloChunk = (IFF_RawChunk*)prop->chunk[0];
			
			if(IFF_compareId(heloChunk->chunkId, "HELO") == 0)
			{
			    if(heloChunk->chunkSize == HELO_BYTES_SIZE)
			    {
				if(heloChunk->chunkData[0] != 'q' ||
				   heloChunk->chunkData[1] != 'w' ||
				   heloChunk->chunkData[2] != 'e' ||
				   heloChunk->chunkData[3] != 'r')
				{
				    fprintf(stderr, "Body of HELO chunk must consist of: 'q','w','e','r'\n");
				    status = 1;
				}
			    }
			    else
			    {
				fprintf(stderr, "Chunk size of the HELO chunk must be: %u\n", HELO_BYTES_SIZE);
				status = 1;
			    }
			}
			else
			{
			    fprintf(stderr, "ID of PROP chunk must be: 'HELO'\n");
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "PROP must contain one chunk!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "PROP formType must be: 'TEST'\n");
		    status = 1;
		}
	    }
	    
	    if(list->chunkLength == 2)
	    {
		if(IFF_compareId(list->chunk[0]->chunkId, "FORM") == 0)
		{
		    IFF_Form *form = (IFF_Form*)list->chunk[0];
		    
		    if(IFF_compareId(form->formType, "TEST") != 0)
		    {
			fprintf(stderr, "formType must be: 'TEST'\n");
			status = 1;
		    }
		    
		    if(form->chunkLength == 1)
		    {
			IFF_RawChunk *byeChunk = (IFF_RawChunk*)form->chunk[0];
			
			if(IFF_compareId(byeChunk->chunkId, "BYE ") == 0)
			{
			    if(byeChunk->chunkSize == BYE_1_BYTES_SIZE)
			    {
				if(byeChunk->chunkData[0] != 'a' ||
				   byeChunk->chunkData[1] != 'b' ||
				   byeChunk->chunkData[2] != 'c' ||
				   byeChunk->chunkData[3] != 'd')
				{
				    fprintf(stderr, "'BYE ' body should consist of: 'a','b','c','d'\n");
				    status = 1;
				}
			    }
			    else
			    {
				fprintf(stderr, "Size of bye chunk must be: %u\n", BYE_1_BYTES_SIZE);
				status = 1;
			    }
			}
			else
			{
			    fprintf(stderr, "FORM subchunk header must be: 'BYE '\n");
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "FORM must contain 1 sub chunk!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "List must contain a FORM sub chunk!\n");
		    status = 1;
		}
		
		if(IFF_compareId(list->chunk[1]->chunkId, "FORM") == 0)
		{
		    IFF_Form *form = (IFF_Form*)list->chunk[1];
		    
		    if(IFF_compareId(form->formType, "TEST") != 0)
		    {
			fprintf(stderr, "formType must be: 'TEST'\n");
			status = 1;
		    }
		    
		    if(form->chunkLength == 1)
		    {
			IFF_RawChunk *byeChunk = (IFF_RawChunk*)form->chunk[0];
			
			if(IFF_compareId(byeChunk->chunkId, "BYE ") == 0)
			{
			    if(byeChunk->chunkSize == BYE_2_BYTES_SIZE)
			    {
				if(byeChunk->chunkData[0] != 'E' ||
				   byeChunk->chunkData[1] != 'F' ||
				   byeChunk->chunkData[2] != 'G' ||
				   byeChunk->chunkData[3] != 'H')
				{
				    fprintf(stderr, "'BYE ' body should consist of: 'E','F','G','H'\n");
				    status = 1;
				}
			    }
			    else
			    {
				fprintf(stderr, "Size of bye chunk must be: %u\n", BYE_2_BYTES_SIZE);
				status = 1;
			    }
			}
			else
			{
			    fprintf(stderr, "FORM subchunk header must be: 'BYE '\n");
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "FORM must contain 1 sub chunk!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "List must contain a FORM sub chunk!\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "The LIST must contain 2 sub chunks!\n");
		status = 1;
	    }
	}
	else
	{
	    fprintf(stderr, "File must be a list!\n");
	    status = 1;
	}
	
	IFF_free(chunk, NULL, 0);
	
	return status;
    }
}
