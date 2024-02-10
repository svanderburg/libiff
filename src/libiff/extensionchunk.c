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

#include "extensionchunk.h"
#include <stdlib.h>
#include "io.h"
#include "error.h"

static IFF_Bool skipUnknownBytes(FILE *file, const IFF_Long chunkSize, const IFF_Long bytesProcessed, const IFF_ID chunkId)
{
    if(bytesProcessed < chunkSize)
    {
        long bytesToSkip = chunkSize - bytesProcessed;

        if(fseek(file, bytesToSkip, SEEK_CUR) == 0)
        {
            IFF_error("Cannot skip: %d bytes in data chunk: '", bytesToSkip);
            IFF_errorId(chunkId);
            IFF_error("'\n");
            return TRUE;
        }
        else
            return FALSE;
    }
    else
        return TRUE;
}

IFF_Chunk *IFF_readExtensionChunk(FILE *file, const IFF_Long chunkSize, const IFF_FormExtension *formExtension)
{
    IFF_Long bytesProcessed = 0;
    IFF_Chunk *chunk = formExtension->createExtensionChunk(chunkSize);

    if(chunk != NULL)
    {
        if(!formExtension->readExtensionChunkFields(file, chunk, &bytesProcessed) ||
          !skipUnknownBytes(file, chunkSize, bytesProcessed, chunk->chunkId))
        {
            IFF_freeChunk(chunk, 0, NULL, 0);
            return NULL;
        }
    }

    return chunk;
}

static IFF_Bool writeZeroFillerBytes(FILE *file, const IFF_Long chunkSize, const IFF_Long bytesProcessed, const IFF_ID chunkId)
{
    if(bytesProcessed < chunkSize)
    {
        size_t bytesToSkip = chunkSize - bytesProcessed;
        IFF_UByte *emptyData = (IFF_UByte*)calloc(bytesToSkip, sizeof(IFF_UByte));
        IFF_Bool status = fwrite(emptyData, sizeof(IFF_UByte), bytesToSkip, file) == bytesToSkip;

        if(!status)
        {
            IFF_error("Cannot write: %u zero bytes in data chunk: '", bytesToSkip);
            IFF_errorId(chunkId);
            IFF_error("'\n");
        }

        free(emptyData);
        return status;
    }
    else
        return TRUE;
}

IFF_Bool IFF_writeExtensionChunk(FILE *file, const IFF_Chunk *chunk, const IFF_FormExtension *formExtension)
{
    IFF_Long bytesProcessed = 0;

    if(!formExtension->writeExtensionChunkFields(file, chunk, &bytesProcessed))
        return FALSE;

    if(!writeZeroFillerBytes(file, chunk->chunkSize, bytesProcessed, chunk->chunkId))
        return FALSE;

    return TRUE;
}

void IFF_freeExtensionChunk(IFF_Chunk *chunk, const IFF_FormExtension *formExtension)
{
    formExtension->freeExtensionChunk(chunk);
}

IFF_Bool IFF_checkExtensionChunk(const IFF_Chunk *chunk, const IFF_FormExtension *formExtension)
{
    return formExtension->checkExtensionChunk(chunk);
}

void IFF_printExtensionChunk(const IFF_Chunk *chunk, unsigned int indentLevel, const IFF_FormExtension *formExtension)
{
    formExtension->printExtensionChunk(chunk, indentLevel);
}

IFF_Bool IFF_compareExtensionChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_FormExtension *formExtension)
{
    return formExtension->compareExtensionChunk(chunk1, chunk2);
}