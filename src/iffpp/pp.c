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

#include "pp.h"
#include "iff.h"

int IFF_prettyPrint(const char *filename, const int options)
{
    IFF_Chunk *chunk;
    
    /* Parse the chunk */
    if(filename == NULL)
	chunk = IFF_readFd(stdin, NULL, 0);
    else
	chunk = IFF_read(filename, NULL, 0);
    
    if(chunk == NULL)
    {
	fprintf(stderr, "Cannot open IFF file!\n");
	return 1;
    }
    else
    {
	int status;
	
	/* Check the file */
	if((options & IFFPP_DISABLE_CHECK) || IFF_check(chunk, NULL, 0))
	{
	    /* Print the file */
	    IFF_print(chunk, 0, NULL, 0);
	    
	    status = 0;
	}
	else
	    status = 1;
	
	/* Free the chunk structure */
	IFF_free(chunk, NULL, 0);
	    
	return status;
    }
}
