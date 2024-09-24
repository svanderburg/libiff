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
#include "value.h"
#include "id.h"
#include "util.h"

IFF_ChunkInterface IFF_rawChunkInterface = {&IFF_parseRawChunkContents, &IFF_writeRawChunkContents, &IFF_checkRawChunkContents, &IFF_clearRawChunkContents, &IFF_printRawChunkContents, &IFF_compareRawChunkContents, NULL, NULL};

IFF_RawChunk *IFF_createRawChunkWithInterface(const IFF_ID chunkId, const IFF_Long chunkSize, IFF_ChunkInterface *chunkInterface)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createChunk(chunkId, chunkSize, sizeof(IFF_RawChunk), chunkInterface);

    if(rawChunk != NULL)
    {
        rawChunk->chunkData = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));

        if(rawChunk->chunkData == NULL)
        {
            free(rawChunk);
            return NULL;
        }
    }

    return rawChunk;
}

IFF_RawChunk *IFF_createRawChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    return IFF_createRawChunkWithInterface(chunkId, chunkSize, &IFF_rawChunkInterface);
}

void IFF_copyDataToRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *data)
{
    memcpy(rawChunk->chunkData, data, rawChunk->chunkSize);
}

IFF_UByte *IFF_updateRawChunkData(IFF_RawChunk *rawChunk, IFF_UByte *chunkData, IFF_Long chunkSize, IFF_Long *obsoleteChunkDataSize)
{
    IFF_UByte *obsoleteChunkData = rawChunk->chunkData;
    *obsoleteChunkDataSize = rawChunk->chunkSize;
    IFF_decreaseChunkSizeByValue((IFF_Chunk*)rawChunk, *obsoleteChunkDataSize);

    rawChunk->chunkData = chunkData;
    IFF_increaseChunkSizeByValue((IFF_Chunk*)rawChunk, chunkSize);

    return obsoleteChunkData;
}

IFF_Chunk *IFF_parseRawChunkContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_RawChunk *rawChunk = IFF_createRawChunkWithInterface(chunkId, chunkSize, chunkInterface);

    if(rawChunk != NULL)
    {
        if(IFF_readUByteArrayField(file, rawChunk->chunkData, rawChunk->chunkSize, (const IFF_Chunk*)rawChunk, attributePath, "chunkData", bytesProcessed, error) == IFF_FIELD_MORE)
            ;
    }

    return (IFF_Chunk*)rawChunk;
}

IFF_Bool IFF_writeRawChunkContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    const IFF_RawChunk *rawChunk = (const IFF_RawChunk*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeUByteArrayField(file, rawChunk->chunkData, rawChunk->chunkSize, (const IFF_Chunk*)rawChunk, attributePath, "chunkData", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_QualityLevel IFF_checkRawChunkContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_QUALITY_PERFECT;
}

void IFF_clearRawChunkContents(IFF_Chunk *chunk)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)chunk;
    free(rawChunk->chunkData);
}

void IFF_printRawChunkContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const IFF_RawChunk *rawChunk = (const IFF_RawChunk*)chunk;
    IFF_printUByteHexArrayField(file, indentLevel, "chunkData", rawChunk->chunkData, rawChunk->chunkSize, 10);
}

IFF_Bool IFF_compareRawChunkContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const IFF_RawChunk *rawChunk1 = (const IFF_RawChunk*)chunk1;
    const IFF_RawChunk *rawChunk2 = (const IFF_RawChunk*)chunk2;

    return memcmp(rawChunk1->chunkData, rawChunk2->chunkData, rawChunk1->chunkSize) == 0;
}
