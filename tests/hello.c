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

#include "hello.h"
#include <stdlib.h>
#include <io.h>
#include <error.h>
#include <util.h>
#include "test.h"

#define CHUNKID "HELO"

TEST_Hello *TEST_createHello(void)
{
    TEST_Hello *hello = (TEST_Hello*)IFF_allocateChunk(CHUNKID, sizeof(TEST_Hello));
    
    if(hello != NULL)
	hello->chunkSize = 2 * sizeof(IFF_UByte) + sizeof(IFF_UWord);
    
    return hello;
}

IFF_Chunk *TEST_readHello(FILE *file, const IFF_Long chunkSize)
{
    TEST_Hello *hello = TEST_createHello();
    
    if(hello != NULL)
    {
	if(!IFF_readUByte(file, &hello->a, CHUNKID, "a"))
	{
	    TEST_free((IFF_Chunk*)hello);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &hello->b, CHUNKID, "b"))
	{
	    TEST_free((IFF_Chunk*)hello);
	    return NULL;
	}
    
	if(!IFF_readUWord(file, &hello->c, CHUNKID, "c"))
	{
	    TEST_free((IFF_Chunk*)hello);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)hello;
}

int TEST_writeHello(FILE *file, const IFF_Chunk *chunk)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;
    
    if(!IFF_writeUByte(file, hello->a, CHUNKID, "a"))
	return FALSE;
    
    if(!IFF_writeUByte(file, hello->b, CHUNKID, "b"))
	return FALSE;
    
    if(!IFF_writeUWord(file, hello->c, CHUNKID, "c"))
	return FALSE;
    
    return TRUE;
}

int TEST_checkHello(const IFF_Chunk *chunk)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;
    
    if((hello->c < 0) || (hello->c > 1024))
    {
	IFF_error("'HELO'.c must be between 0 and 1024\n");
	return FALSE;
    }
    
    return TRUE;
}

void TEST_freeHello(IFF_Chunk *chunk)
{
}

void TEST_printHello(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "a = %c;\n", hello->a);
    IFF_printIndent(stdout, indentLevel, "b = %c;\n", hello->b);
    IFF_printIndent(stdout, indentLevel, "c = %u;\n", hello->c);
}

int TEST_compareHello(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const TEST_Hello *hello1 = (const TEST_Hello*)chunk1;
    const TEST_Hello *hello2 = (const TEST_Hello*)chunk2;
    
    if(hello1->a != hello2->a)
	return FALSE;

    if(hello1->b != hello2->b)
	return FALSE;

    if(hello1->c != hello2->c)
	return FALSE;

    return TRUE;
}
