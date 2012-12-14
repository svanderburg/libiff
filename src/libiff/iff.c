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
#include <stdio.h>
#include <stdlib.h>
#include "id.h"
#include "util.h"
#include "error.h"

IFF_Chunk *IFF_readFd(FILE *file, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_Chunk *chunk;
    int byte;
    
    /* Read the chunk */
    chunk = IFF_readChunk(file, NULL, extension, extensionLength);
    
    if(chunk == NULL)
    {
	IFF_error("ERROR: cannot open main chunk!\n");
	return NULL;
    }
    
    /* We should have reached the EOF now */
    
    if((byte = fgetc(file)) != EOF)
	IFF_error("WARNING: Trailing IFF contents found: %d!\n", byte);

    /* Return the parsed main chunk */
    return chunk;
}

IFF_Chunk *IFF_read(const char *filename, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_Chunk *chunk;
    FILE *file = fopen(filename, "rb");
    
    /* Open the IFF file */
    if(file == NULL)
    {
	IFF_error("ERROR: cannot open file: %s\n", filename);
	return NULL;
    }

    /* Parse the main chunk */
    chunk = IFF_readFd(file, extension, extensionLength);
    
    /* Close the file */
    fclose(file);

    /* Return the chunk */
    return chunk;
}

int IFF_writeFd(FILE *file, const IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(IFF_writeChunk(file, chunk, NULL, extension, extensionLength))
	return TRUE;
    else
	return FALSE;
}

int IFF_write(const char *filename, const IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength)
{
    int status = TRUE;
    FILE *file = fopen(filename, "wb");
    
    if(file == NULL)
    {
	IFF_error("ERROR: cannot open file: %s\n", filename);
	return FALSE;
    }
    
    if(!IFF_writeFd(file, chunk, extension, extensionLength))
	status = FALSE;
    
    fclose(file);
    
    return status;
}

void IFF_free(IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_freeChunk(chunk, NULL, extension, extensionLength);
}

int IFF_check(const IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength)
{
    /* The main chunk must be of ID: FORM, CAT or LIST */
    
    if(IFF_compareId(chunk->chunkId, "FORM") != 0 &&
       IFF_compareId(chunk->chunkId, "CAT ") != 0 &&
       IFF_compareId(chunk->chunkId, "LIST") != 0)
    {
	IFF_error("Not a valid IFF-85 file: First bytes should start with either: 'FORM', 'CAT ' or 'LIST'\n");
	return FALSE;
    }
    else
	return IFF_checkChunk(chunk, NULL, extension, extensionLength);
}

void IFF_print(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printChunk(chunk, indentLevel, NULL, extension, extensionLength);
}

int IFF_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_compareChunk(chunk1, chunk2, NULL, extension, extensionLength);
}
