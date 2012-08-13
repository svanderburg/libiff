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

#include "bye.h"
#include <stdlib.h>
#include <io.h>
#include <error.h>
#include <util.h>
#include "test.h"

#define CHUNKID "BYE "

TEST_Bye *TEST_createBye(void)
{
    TEST_Bye *bye = (TEST_Bye*)IFF_allocateChunk(CHUNKID, sizeof(TEST_Bye));
    
    if(bye != NULL)
	bye->chunkSize = 2 * sizeof(IFF_Long);
    
    return bye;
}

IFF_Chunk *TEST_readBye(FILE *file, const IFF_Long chunkSize)
{
    TEST_Bye *bye = TEST_createBye();
    
    if(bye != NULL)
    {
	if(!IFF_readLong(file, &bye->one, CHUNKID, "one"))
	{
	    TEST_free((IFF_Chunk*)bye);
	    return NULL;
	}
    
	if(!IFF_readLong(file, &bye->two, CHUNKID, "two"))
	{
	    TEST_free((IFF_Chunk*)bye);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)bye;
}

int TEST_writeBye(FILE *file, const IFF_Chunk *chunk)
{
    const TEST_Bye *bye = (const TEST_Bye*)chunk;
    
    if(!IFF_writeLong(file, bye->one, CHUNKID, "one"))
	return FALSE;
    
    if(!IFF_writeLong(file, bye->two, CHUNKID, "two"))
	return FALSE;
    
    return TRUE;
}

int TEST_checkBye(const IFF_Chunk *chunk)
{
    return TRUE;
}

void TEST_freeBye(IFF_Chunk *chunk)
{
}

void TEST_printBye(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const TEST_Bye *bye = (const TEST_Bye*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "one = %d;\n", bye->one);
    IFF_printIndent(stdout, indentLevel, "two = %d;\n", bye->two);
}

int TEST_compareBye(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const TEST_Bye *bye1 = (const TEST_Bye*)chunk1;
    const TEST_Bye *bye2 = (const TEST_Bye*)chunk2;

    if(bye1->one != bye2->one)
	return FALSE;

    if(bye1->two != bye2->two)
	return FALSE;

    return TRUE;
}
