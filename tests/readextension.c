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

#include "test.h"
#include <stdio.h>
#include "form.h"
#include "id.h"
#include "hello.h"
#include "bye.h"

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk = TEST_read("extension.TEST");
    
    if(chunk == NULL)
	return 1;
    else
    {
	int status = 0;
	
	if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	{
	    IFF_Form *form = (IFF_Form*)chunk;
	    
	    if(IFF_compareId(form->formType, "TEST") == 0)
	    {
		if(form->chunkLength == 2)
		{
		    if(IFF_compareId(form->chunk[0]->chunkId, "HELO") == 0)
		    {
			TEST_Hello *hello = (TEST_Hello*)form->chunk[0];
		    
			if(hello->a != 'a')
			{
			    fprintf(stderr, "ERROR: 'HELO'.a must be: 'a'\n");
			    status = 1;
			}
		    
			if(hello->b != 'b')
			{
			    fprintf(stderr, "ERROR: 'HELO'.b must be: 'b'\n");
			    status = 1;
			}
		    
			if(hello->c != 4096)
			{
			    fprintf(stderr, "ERROR: 'HELO'.c must be: 4096\n");
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "ERROR: First subchunk must be a 'HELO' chunk!\n");
			status = 1;
		    }
		
		    if(IFF_compareId(form->chunk[1]->chunkId, "BYE ") == 0)
		    {
			TEST_Bye *bye = (TEST_Bye*)form->chunk[1];
		    
			if(bye->one != 1)
			{
			    fprintf(stderr, "ERROR: 'BYE '.one must be: 1\n");
			    status = 1;
			}
		    
			if(bye->two != 2)
			{
			    fprintf(stderr, "ERROR: 'BYE '.two must be: 2\n");
			    status = 1;
			}
		    }
		    else
		    {
			fprintf(stderr, "ERROR: Second subchunk must be a 'BYE ' chunk!\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "ERROR: Form should contain 2 sub chunks!\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "ERROR: Form type must be: 'TEST'\n");
		status = 1;
	    }
	}
	else
	{
	    fprintf(stderr, "ERROR: The file must be a FORM!\n");
	    status = 1;
	}
	
	TEST_free(chunk);
	
	return status;
    }
}
