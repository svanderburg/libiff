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

#include "bye.h"
#include <stdlib.h>
#include <structure.h>
#include <error.h>
#include <util.h>
#include "test.h"

IFF_ChunkInterface TEST_byeInterface = {&TEST_parseByeContents, &TEST_writeByeContents, &TEST_checkByeContents, &TEST_clearByeContents, &TEST_printByeContents, &TEST_compareByeContents, NULL, NULL};

static void initBye(TEST_Bye *bye)
{
    bye->one = 0;
    bye->two = 0;
}

TEST_Bye *TEST_deriveByeChunk(const IFF_Chunk *chunk)
{
    TEST_Bye *bye = (TEST_Bye*)IFF_createDerivedChunk(chunk, sizeof(TEST_Bye));

    if(bye != NULL)
        initBye(bye);

    return bye;
}

TEST_Bye *TEST_createBye(const IFF_Long chunkSize)
{
    TEST_Bye *bye = (TEST_Bye*)IFF_createChunk(&TEST_byeInterface, TEST_ID_BYE, chunkSize, sizeof(TEST_Bye));

    if(bye != NULL)
        initBye(bye);

    return bye;
}

typedef enum
{
    FIELD_INDEX_ONE = 0,
    FIELD_INDEX_TWO = 1
}
FieldIndex;

static void *getFieldPointer(void *object, const unsigned int index)
{
    TEST_Bye *bye = (TEST_Bye*)object;

    switch(index)
    {
        case FIELD_INDEX_ONE:
            return &bye->one;
        case FIELD_INDEX_TWO:
            return &bye->two;
        default:
            return NULL;
    }
}

static IFF_Field fields[] = {
    { "one", &IFF_Type_Long, IFF_CARDINALITY_SINGLE },
    { "two", &IFF_Type_Long, IFF_CARDINALITY_SINGLE }
};

static IFF_Structure byeStructure = {
    2,
    fields,
    getFieldPointer,
    NULL,
    NULL
};

IFF_Chunk *TEST_parseByeContents(FILE *file, IFF_Chunk *chunk, const IFF_Registry *registry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    TEST_Bye *bye = TEST_deriveByeChunk(chunk);

    IFF_readStructure(file, &byeStructure, bye, (IFF_Chunk*)bye, attributePath, bytesProcessed, error);

    return (IFF_Chunk*)bye;
}

IFF_Bool TEST_writeByeContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_deriveSuccess(IFF_writeStructure(file, &byeStructure, (void*)chunk, chunk, attributePath, bytesProcessed, error));
}

IFF_QualityLevel TEST_checkByeContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_QUALITY_PERFECT;
}

void TEST_clearByeContents(IFF_Chunk *chunk)
{
}

void TEST_printByeContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printStructureContents(file, indentLevel, &byeStructure, (void*)chunk);
}

IFF_Bool TEST_compareByeContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareStructure(&byeStructure, (void*)chunk1, (void*)chunk2);
}
