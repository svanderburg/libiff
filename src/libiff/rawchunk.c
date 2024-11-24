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
#include "structure.h"
#include "id.h"
#include "string.h"
#include "util.h"

IFF_ChunkInterface IFF_rawChunkInterface = {&IFF_parseRawChunkContents, &IFF_writeRawChunkContents, &IFF_checkRawChunkContents, &IFF_clearRawChunkContents, &IFF_printRawChunkContents, &IFF_compareRawChunkContents, NULL, NULL};

void IFF_clearRawChunk(IFF_RawChunk *rawChunk)
{
    rawChunk->chunkDataLength = 0;
    rawChunk->chunkData = NULL;
}

IFF_RawChunk *IFF_deriveRawChunk(const IFF_Chunk *chunk)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createDerivedChunk(chunk, sizeof(IFF_RawChunk));

    if(rawChunk != NULL)
        IFF_clearRawChunk(rawChunk);

    return rawChunk;
}

IFF_RawChunk *IFF_createRawChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createChunk(&IFF_rawChunkInterface, chunkId, chunkSize, sizeof(IFF_RawChunk));

    rawChunk->chunkDataLength = chunkSize;
    rawChunk->chunkData = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));

    return rawChunk;
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

    rawChunk->chunkDataLength = chunkSize;
    rawChunk->chunkData = chunkData;
    IFF_increaseChunkSizeByValue((IFF_Chunk*)rawChunk, chunkSize);

    return obsoleteChunkData;
}

typedef enum
{
    FIELD_INDEX_CHUNK_DATA = 0
}
FieldIndex;

static IFF_Field fields[] = {
    { "chunkData", &IFF_Type_UByte, IFF_CARDINALITY_MULTIPLE }
};

void **IFF_getRawChunkArrayFieldPointer(void *object, const unsigned int index, IFF_Long **arrayLength)
{
    if(index == FIELD_INDEX_CHUNK_DATA)
    {
        IFF_RawChunk *rawChunk = (IFF_RawChunk*)object;
        *arrayLength = &rawChunk->chunkDataLength;
        return (void**)&rawChunk->chunkData;
    }
    else
        return NULL;
}

IFF_Long IFF_getSpecifiedRawChunkArrayFieldLengthFunction(void *object, const unsigned int index)
{
    if(index == FIELD_INDEX_CHUNK_DATA)
    {
        IFF_RawChunk *rawChunk = (IFF_RawChunk*)object;
        return rawChunk->chunkSize;
    }
    else
        return 0;
}

static IFF_Structure rawChunkStructure = {
    1,
    fields,
    NULL,
    IFF_getRawChunkArrayFieldPointer,
    IFF_getSpecifiedRawChunkArrayFieldLengthFunction
};

IFF_Chunk *IFF_parseRawChunkContents(FILE *file, IFF_Chunk *chunk, const IFF_Registry *registry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_RawChunk *rawChunk = IFF_deriveRawChunk(chunk);

    if(rawChunk != NULL)
        IFF_readStructure(file, &rawChunkStructure, rawChunk, (IFF_Chunk*)rawChunk, attributePath, bytesProcessed, error);

    return (IFF_Chunk*)rawChunk;
}

IFF_Bool IFF_writeRawChunkContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_deriveSuccess(IFF_writeStructure(file, &rawChunkStructure, (void*)chunk, chunk, attributePath, bytesProcessed, error));
}

IFF_QualityLevel IFF_checkRawChunkContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_checkStructureArrayLengths(&rawChunkStructure, (void*)chunk, chunk, attributePath, printCheckMessage, data);
}

void IFF_clearRawChunkContents(IFF_Chunk *chunk)
{
    IFF_clearStructure(&rawChunkStructure, chunk);
}

void IFF_printRawChunkContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printStructureContents(file, indentLevel, &rawChunkStructure, (void*)chunk);
}

IFF_Bool IFF_compareRawChunkContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareStructure(&rawChunkStructure, (void*)chunk1, (void*)chunk2);
}
