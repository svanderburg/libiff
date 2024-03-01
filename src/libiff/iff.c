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
#include "util.h"
#include "form.h"
#include "cat.h"
#include "list.h"
#include "error.h"
#include "defaultregistry.h"

static const IFF_ChunkRegistry *selectChunkRegistry(const IFF_ChunkRegistry *chunkRegistry)
{
    if(chunkRegistry == NULL)
        return &IFF_defaultChunkRegistry;
    else
        return chunkRegistry;
}

IFF_Chunk *IFF_readFd(FILE *file, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_Chunk *chunk;
    int byte;

    /* Read the chunk */
    chunk = IFF_readChunk(file, 0, selectChunkRegistry(chunkRegistry));

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

IFF_Chunk *IFF_readFile(const char *filename, const IFF_ChunkRegistry *chunkRegistry)
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
    chunk = IFF_readFd(file, chunkRegistry);

    /* Close the file */
    fclose(file);

    /* Return the chunk */
    return chunk;
}

IFF_Chunk *IFF_read(const char *filename, const IFF_ChunkRegistry *chunkRegistry)
{
    if(filename == NULL)
        return IFF_readFd(stdin, chunkRegistry);
    else
        return IFF_readFile(filename, chunkRegistry);
}

IFF_Bool IFF_writeFd(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_writeChunk(file, chunk, 0, selectChunkRegistry(chunkRegistry));
}

IFF_Bool IFF_writeFile(const char *filename, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_Bool status;
    FILE *file = fopen(filename, "wb");

    if(file == NULL)
    {
        IFF_error("ERROR: cannot open file: %s\n", filename);
        return FALSE;
    }

    status = IFF_writeFd(file, chunk, chunkRegistry);
    fclose(file);
    return status;
}

IFF_Bool IFF_write(const char *filename, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    if(filename == NULL)
        return IFF_writeFd(stdout, chunk, chunkRegistry);
    else
        return IFF_writeFile(filename, chunk, chunkRegistry);
}

void IFF_free(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_freeChunk(chunk, 0, selectChunkRegistry(chunkRegistry));
}

IFF_Bool IFF_check(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    /* The main chunk must be of ID: FORM, CAT or LIST */

    if(chunk->chunkId != IFF_ID_FORM &&
       chunk->chunkId != IFF_ID_CAT &&
       chunk->chunkId != IFF_ID_LIST)
    {
        IFF_error("Not a valid IFF-85 file: First bytes should start with either: 'FORM', 'CAT ' or 'LIST'\n");
        return FALSE;
    }
    else
        return IFF_checkChunk(chunk, 0, selectChunkRegistry(chunkRegistry));
}

void IFF_print(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printChunk(chunk, indentLevel, 0, selectChunkRegistry(chunkRegistry));
}

IFF_Bool IFF_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_compareChunk(chunk1, chunk2, 0, selectChunkRegistry(chunkRegistry));
}
