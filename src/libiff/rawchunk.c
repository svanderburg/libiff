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

#include "rawchunk.h"
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "io.h"
#include "id.h"
#include "util.h"

IFF_RawChunk *IFF_createRawChunk(const char *chunkId)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_allocateChunk(chunkId, sizeof(IFF_RawChunk));
    
    if(rawChunk != NULL)
	rawChunk->chunkData = NULL;
    
    return rawChunk;
}

void IFF_setRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *chunkData, IFF_Long chunkSize)
{
    rawChunk->chunkData = chunkData;
    rawChunk->chunkSize = chunkSize;
}

void IFF_setTextData(IFF_RawChunk *rawChunk, const char *text)
{
    size_t textLength = strlen(text);
    IFF_UByte *chunkData = (IFF_UByte*)malloc(textLength * sizeof(IFF_UByte));
    
    memcpy(chunkData, text, textLength);
    IFF_setRawChunkData(rawChunk, chunkData, textLength);
}

IFF_RawChunk *IFF_readRawChunk(FILE *file, const char *chunkId, const IFF_Long chunkSize)
{
    IFF_UByte *chunkData = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));
    IFF_RawChunk *rawChunk = IFF_createRawChunk(chunkId);
	
    /* Read remaining bytes verbatim */
	
    if(fread(chunkData, sizeof(IFF_UByte), chunkSize, file) < chunkSize)
    {
	IFF_error("Error reading raw chunk body of chunk: '");
	IFF_errorId(chunkId);
	IFF_error("'\n");
	IFF_freeChunk((IFF_Chunk*)rawChunk, NULL, NULL, 0);
	return NULL;
    }
	    
    /* If the chunk size is odd, we have to read the padding byte */
    if(!IFF_readPaddingByte(file, chunkSize, chunkId))
    {
	IFF_freeChunk((IFF_Chunk*)rawChunk, NULL, NULL, 0);
	return NULL;
    }
    
    /* Add data to the created chunk */
    IFF_setRawChunkData(rawChunk, chunkData, chunkSize);
    
    /* Return the resulting raw chunk */
    return rawChunk;
}

int IFF_writeRawChunk(FILE *file, const IFF_RawChunk *rawChunk)
{
    if(fwrite(rawChunk->chunkData, sizeof(IFF_UByte), rawChunk->chunkSize, file) < rawChunk->chunkSize)
    {
	IFF_error("Error writing raw chunk body of chunk '");
	IFF_errorId(rawChunk->chunkId);
	IFF_error("'\n");
	return FALSE;
    }
	
    /* If the chunk size is odd, we have to write the padding byte */
    if(!IFF_writePaddingByte(file, rawChunk->chunkSize, rawChunk->chunkId))
	return FALSE;
    
    return TRUE;
}

void IFF_freeRawChunk(IFF_RawChunk *rawChunk)
{
    free(rawChunk->chunkData);
}

void IFF_printText(const IFF_RawChunk *rawChunk, const unsigned int indentLevel)
{
    unsigned int i;
	
    IFF_printIndent(stdout, indentLevel, "text = '\n");
    IFF_printIndent(stdout, indentLevel + 1, "");
	
    for(i = 0; i < rawChunk->chunkSize; i++)
        printf("%c", rawChunk->chunkData[i]);

    printf("\n");
    IFF_printIndent(stdout, indentLevel, "';\n");
}

void IFF_printRaw(const IFF_RawChunk *rawChunk, const unsigned int indentLevel)
{
    unsigned int i;
    IFF_UByte byte;
	
    IFF_printIndent(stdout, indentLevel, "bytes = \n");
    IFF_printIndent(stdout, indentLevel + 1, "");
	
    for(i = 0; i < rawChunk->chunkSize; i++)
    {
	if(i > 0 && i % 10 == 0)
	{
	    printf("\n");
	    IFF_printIndent(stdout, indentLevel + 1, "");
	}
	    
	byte = rawChunk->chunkData[i];
	    
	/* Print extra 0 for small numbers */
	if(byte <= 0xf)
	    printf("0");
	    
	printf("%x ", byte);
    }
	
    printf("\n");
    IFF_printIndent(stdout, indentLevel, ";\n");
}

void IFF_printRawChunk(const IFF_RawChunk *rawChunk, unsigned int indentLevel)
{
    if(IFF_compareId(rawChunk->chunkId, "TEXT") == 0)
	IFF_printText(rawChunk, indentLevel);
    else
	IFF_printRaw(rawChunk, indentLevel);
}

int IFF_compareRawChunk(const IFF_RawChunk *rawChunk1, const IFF_RawChunk *rawChunk2)
{
    return (memcmp(rawChunk1->chunkData, rawChunk2->chunkData, rawChunk1->chunkSize) == 0);
}
