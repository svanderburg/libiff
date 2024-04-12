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
#include <field.h>
#include <error.h>
#include <util.h>
#include "test.h"

IFF_Chunk *TEST_createByeChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    TEST_Bye *bye = (TEST_Bye*)IFF_createChunk(chunkId, chunkSize, sizeof(TEST_Bye));

    if(bye != NULL)
    {
        bye->one = 0;
        bye->two = 0;
    }

    return (IFF_Chunk*)bye;
}

TEST_Bye *TEST_createBye(const IFF_Long chunkSize)
{
    return (TEST_Bye*)TEST_createByeChunk(TEST_ID_BYE, chunkSize);
}

IFF_Bool TEST_readByeContents(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    TEST_Bye *bye = (TEST_Bye*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readLongField(file, &bye->one, chunk, attributePath, "one", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readLongField(file, &bye->two, chunk, attributePath, "two", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool TEST_writeByeContents(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    const TEST_Bye *bye = (const TEST_Bye*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeLongField(file, bye->one, chunk, attributePath, "one", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeLongField(file, bye->two, chunk, attributePath, "two", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_QualityLevel TEST_checkByeContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_QUALITY_PERFECT;
}

void TEST_clearByeContents(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
}

void TEST_printByeContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const TEST_Bye *bye = (const TEST_Bye*)chunk;

    IFF_printLongField(stdout, indentLevel, "one", bye->one);
    IFF_printLongField(stdout, indentLevel, "two", bye->two);
}

IFF_Bool TEST_compareByeContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const TEST_Bye *bye1 = (const TEST_Bye*)chunk1;
    const TEST_Bye *bye2 = (const TEST_Bye*)chunk2;

    if(bye1->one != bye2->one)
        return FALSE;

    if(bye1->two != bye2->two)
        return FALSE;

    return TRUE;
}
