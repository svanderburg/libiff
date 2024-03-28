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
#include "defaultregistry.h"

static const IFF_ChunkRegistry *selectChunkRegistry(const IFF_ChunkRegistry *chunkRegistry)
{
    if(chunkRegistry == NULL)
        return &IFF_defaultChunkRegistry;
    else
        return chunkRegistry;
}

IFF_Chunk *IFF_readFd(FILE *file, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error)
{
    int byte;
    IFF_AttributePath *attributePath = IFF_createAttributePath();

    /* Read the chunk */
    IFF_Chunk *chunk = IFF_readChunk(file, 0, selectChunkRegistry(chunkRegistry), attributePath, error);

    /* We should have reached the EOF now */
    if((byte = fgetc(file)) != EOF)
        fprintf(stderr, "WARNING: Trailing IFF contents found: %d!\n", byte);

    /* Remove the attribute path if we no longer need it */
    if(*error == NULL)
        IFF_freeAttributePath(attributePath);

    /* Return the parsed main chunk */
    return chunk;
}

IFF_Chunk *IFF_readFile(const char *filename, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error)
{
    FILE *file = fopen(filename, "rb");

    if(file == NULL)
    {
        *error = IFF_createFileIOError(filename);
        return NULL;
    }
    else
    {
        IFF_Chunk *chunk = IFF_readFd(file, chunkRegistry, error);
        fclose(file);
        return chunk;
    }
}

IFF_Chunk *IFF_read(const char *filename, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error)
{
    if(filename == NULL)
        return IFF_readFd(stdin, chunkRegistry, error);
    else
        return IFF_readFile(filename, chunkRegistry, error);
}

IFF_Bool IFF_writeFd(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error)
{
    IFF_AttributePath *attributePath = IFF_createAttributePath();
    IFF_Bool status = IFF_writeChunk(file, chunk, 0, selectChunkRegistry(chunkRegistry), attributePath, error);

    if(*error == NULL)
        IFF_freeAttributePath(attributePath);

    return status;
}

IFF_Bool IFF_writeFile(const char *filename, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error)
{
    FILE *file = fopen(filename, "wb");

    if(file == NULL)
    {
        *error = IFF_createFileIOError(filename);
        return FALSE;
    }
    else
    {
        IFF_Bool status = IFF_writeFd(file, chunk, chunkRegistry, error);
        fclose(file);
        return status;
    }
}

IFF_Bool IFF_write(const char *filename, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_IOError **error)
{
    if(filename == NULL)
        return IFF_writeFd(stdout, chunk, chunkRegistry, error);
    else
        return IFF_writeFile(filename, chunk, chunkRegistry, error);
}

void IFF_free(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_freeChunk(chunk, 0, selectChunkRegistry(chunkRegistry));
}

IFF_Bool IFF_advancedCheck(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_printCheckMessage printCheckMessage, void *data)
{
    IFF_AttributePath *attributePath = IFF_createAttributePath();
    IFF_Bool status;

    if(chunk == NULL)
    {
        printCheckMessage(attributePath, NULL, 0, data, "The file cannot be processed");
        status = FALSE;
    }
    else if(chunk->chunkId != IFF_ID_FORM && /* The main chunk must be of ID: FORM, CAT or LIST */
       chunk->chunkId != IFF_ID_CAT &&
       chunk->chunkId != IFF_ID_LIST)
    {
        IFF_ID2 chunkId;
        IFF_idToString(chunk->chunkId, chunkId);
        printCheckMessage(attributePath, "chunkId", chunk->chunkId, data, "is invalid: the first chunkId should be: \"FORM\", \"CAT \" or \"LIST\", value is: \"%.4s\"", chunkId);
        status = FALSE;
    }
    else
        status = IFF_checkChunk(chunk, 0, selectChunkRegistry(chunkRegistry), attributePath, printCheckMessage, NULL);

    IFF_freeAttributePath(attributePath);
    return status;
}

IFF_Bool IFF_check(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_advancedCheck(chunk, chunkRegistry, IFF_printCheckMessageOnStderr, NULL);
}

void IFF_print(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printChunk(chunk, indentLevel, 0, selectChunkRegistry(chunkRegistry));
}

IFF_Bool IFF_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_compareChunk(chunk1, chunk2, 0, selectChunkRegistry(chunkRegistry));
}
