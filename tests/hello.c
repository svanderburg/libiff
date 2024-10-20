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
#include <structure.h>
#include <error.h>
#include <util.h>
#include "test.h"

IFF_ChunkInterface TEST_helloInterface = {&TEST_parseHelloContents, &TEST_writeHelloContents, &TEST_checkHelloContents, &TEST_clearHelloContents, &TEST_printHelloContents, &TEST_compareHelloContents, NULL, NULL};

TEST_Hello *TEST_createHelloChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    TEST_Hello *hello = (TEST_Hello*)IFF_createChunk(chunkId, chunkSize, sizeof(TEST_Hello), &TEST_helloInterface);

    if(hello != NULL)
    {
        hello->a = '\0';
        hello->b = '\0';
        hello->c = 0;
    }

    return hello;
}

TEST_Hello *TEST_createHello(const IFF_Long chunkSize)
{
    return TEST_createHelloChunk(TEST_ID_HELO, chunkSize);
}

static void *getFieldPointer(void *object, const unsigned int index)
{
    TEST_Hello *hello = (TEST_Hello*)object;

    switch(index)
    {
        case 0:
            return &hello->a;
        case 1:
            return &hello->b;
        case 2:
            return &hello->c;
        default:
            return NULL;
    }
}

static IFF_Field fields[] = {
    { "a", &IFF_Type_UByte, IFF_CARDINALITY_SINGLE },
    { "b", &IFF_Type_UByte, IFF_CARDINALITY_SINGLE },
    { "c", &IFF_Type_UWord, IFF_CARDINALITY_SINGLE }
};

static IFF_Structure helloStructure = {
    3,
    fields,
    getFieldPointer,
    NULL
};

IFF_Chunk *TEST_parseHelloContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    TEST_Hello *hello = TEST_createHelloChunk(chunkId, chunkSize);

    IFF_readStructure(file, &helloStructure, hello, (IFF_Chunk*)hello, attributePath, bytesProcessed, error);

    return (IFF_Chunk*)hello;
}

IFF_Bool TEST_writeHelloContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_writeStructure(file, &helloStructure, (void*)chunk, chunk, attributePath, bytesProcessed, error);
}

IFF_QualityLevel TEST_checkHelloContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;

    if((hello->c < 0) || (hello->c > 1024))
    {
        printCheckMessage(attributePath, "c", chunk->chunkId, data, "must be between 0 and 1024, value is: %u", hello->c);
        qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_QUALITY_INCONSISTENT);
    }

    return qualityLevel;
}

void TEST_clearHelloContents(IFF_Chunk *chunk)
{
}

void TEST_printHelloContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printStructureContents(file, indentLevel, &helloStructure, (void*)chunk);
}

IFF_Bool TEST_compareHelloContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareStructure(&helloStructure, (void*)chunk1, (void*)chunk2);
}
