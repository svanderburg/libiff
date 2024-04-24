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
#include "io.h"
#include "id.h"
#include "util.h"

IFF_ChunkInterface IFF_rawChunkInterface = {&IFF_createRawChunk, &IFF_readRawChunkContents, &IFF_writeRawChunkContents, &IFF_checkRawChunkContents, &IFF_clearRawChunkContents, &IFF_printRawChunkContents, &IFF_compareRawChunkContents};

IFF_Chunk *IFF_createRawChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createChunk(chunkId, chunkSize, sizeof(IFF_RawChunk));

    if(rawChunk != NULL)
    {
        rawChunk->chunkData = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));

        if(rawChunk->chunkData == NULL)
        {
            free(rawChunk);
            return NULL;
        }
    }

    return (IFF_Chunk*)rawChunk;
}

void IFF_copyDataToRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *data)
{
    memcpy(rawChunk->chunkData, data, rawChunk->chunkSize);
}

void IFF_setRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *chunkData, IFF_Long chunkSize)
{
    rawChunk->chunkData = chunkData;
    rawChunk->chunkSize = chunkSize;
}

IFF_Bool IFF_readRawChunkContents(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)chunk;

    if(fread(rawChunk->chunkData, sizeof(IFF_UByte), rawChunk->chunkSize, file) < rawChunk->chunkSize)
    {
        *error = IFF_createDataIOError(file, rawChunk->chunkSize, attributePath, "chunkData", "raw data", rawChunk->chunkId);
        return FALSE;
    }
    else
    {
        *bytesProcessed = *bytesProcessed + rawChunk->chunkSize;
        return TRUE;
    }
}

IFF_Bool IFF_writeRawChunkContents(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    const IFF_RawChunk *rawChunk = (const IFF_RawChunk*)chunk;

    if(fwrite(rawChunk->chunkData, sizeof(IFF_UByte), rawChunk->chunkSize, file) < rawChunk->chunkSize)
    {
        *error = IFF_createDataIOError(file, rawChunk->chunkSize, attributePath, "chunkData", "raw data", rawChunk->chunkId);
        return FALSE;
    }
    else
    {
        *bytesProcessed = *bytesProcessed + rawChunk->chunkSize;
        return TRUE;
    }
}

IFF_QualityLevel IFF_checkRawChunkContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_QUALITY_PERFECT;
}

void IFF_clearRawChunkContents(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)chunk;
    free(rawChunk->chunkData);
}

void IFF_printChunkDataBytes(FILE *file, const void *value, const unsigned int indentLevel, IFF_printValueFunction printByteValue)
{
    const IFF_RawChunk *rawChunk = (const IFF_RawChunk*)value;
    IFF_Long i;

    fputs("{\n", file);
    IFF_printIndent(file, indentLevel + 1, "");

    for(i = 0; i < rawChunk->chunkSize; i++)
    {
        if(i > 0)
        {
            fputs(", ", file);

            if(i % 10 == 0)
            {
                fputc('\n', file);
                IFF_printIndent(file, indentLevel + 1, "");
            }
        }

        printByteValue(file, &rawChunk->chunkData[i], indentLevel + 1);
    }

    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

void IFF_printChunkDataUByteHex(FILE *file, const void *value, const unsigned int indentLevel)
{
    IFF_printChunkDataBytes(file, value, indentLevel, IFF_printUByteHex);
}

void IFF_printRawChunkContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printField(file, indentLevel, "chunkData", chunk, IFF_printChunkDataUByteHex);
}

IFF_Bool IFF_compareRawChunkContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_RawChunk *rawChunk1 = (const IFF_RawChunk*)chunk1;
    const IFF_RawChunk *rawChunk2 = (const IFF_RawChunk*)chunk2;

    return memcmp(rawChunk1->chunkData, rawChunk2->chunkData, rawChunk1->chunkSize) == 0;
}
