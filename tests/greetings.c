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

#include "greetings.h"
#include <stdlib.h>
#include <structure.h>

IFF_ChunkInterface TEST_greetingsInterface = {&TEST_parseGreetingsContents, &TEST_writeGreetingsContents, &TEST_checkGreetingsContents, &TEST_clearGreetingsContents, &TEST_printGreetingsContents, &TEST_compareGreetingsContents, NULL, NULL};

TEST_Greetings *TEST_createGreetingsChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    TEST_Greetings *greetings = (TEST_Greetings*)IFF_createChunk(chunkId, chunkSize, sizeof(TEST_Greetings), &TEST_greetingsInterface);

    if(greetings != NULL)
    {
        greetings->greetsLength = greetings->chunkSize / sizeof(TEST_Greet);
        greetings->greets = NULL;
    }

    return greetings;
}

TEST_Greetings *TEST_createGreetings(void)
{
    return TEST_createGreetingsChunk(TEST_ID_GRTS, 0);
}

TEST_Greet *TEST_addGreetToGreetings(TEST_Greetings *greetings)
{
    return (TEST_Greet*)IFF_addArrayElementToChunk((IFF_Chunk*)greetings, (void**)&greetings->greets, sizeof(TEST_Greet), &greetings->greetsLength);
}

static void **getArrayFieldPointer(void *object, const unsigned int index, unsigned int *arrayLength)
{
    if(index == 0)
    {
        TEST_Greetings *greetings = (TEST_Greetings*)object;
        *arrayLength = greetings->chunkSize / sizeof(TEST_Greet);
        return (void**)&greetings->greets;
    }
    else
        return NULL;
}

static IFF_Field fields[] = {
    { "greets", &TEST_Type_Greet, IFF_CARDINALITY_MULTIPLE }
};

static IFF_Structure greetingsStructure = {
    1,
    fields,
    NULL,
    getArrayFieldPointer
};

IFF_Chunk *TEST_parseGreetingsContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    TEST_Greetings *greetings = TEST_createGreetingsChunk(chunkId, chunkSize);

    IFF_readStructure(file, &greetingsStructure, greetings, (IFF_Chunk*)greetings, attributePath, bytesProcessed, error);

    return (IFF_Chunk*)greetings;
}

IFF_Bool TEST_writeGreetingsContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_deriveSuccess(IFF_writeStructure(file, &greetingsStructure, (void*)chunk, chunk, attributePath, bytesProcessed, error));
}

IFF_QualityLevel TEST_checkGreetingsContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_QUALITY_PERFECT;
}

void TEST_clearGreetingsContents(IFF_Chunk *chunk)
{
    IFF_clearStructure(&greetingsStructure, chunk);
}

void TEST_printGreetingsContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printStructureContents(file, indentLevel, &greetingsStructure, (void*)chunk);
}

IFF_Bool TEST_compareGreetingsContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareStructure(&greetingsStructure, (void*)chunk1, (void*)chunk2);
}
