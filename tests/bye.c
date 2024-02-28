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

IFF_Chunk *TEST_createBye(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    TEST_Bye *bye = (TEST_Bye*)IFF_createChunk(chunkId, chunkSize, sizeof(TEST_Bye));

    if(bye != NULL)
    {
        bye->one = 0;
        bye->two = 0;
    }

    return (IFF_Chunk*)bye;
}

IFF_Bool TEST_readBye(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    TEST_Bye *bye = (TEST_Bye*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readLongField(file, &bye->one, chunk, "one", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readLongField(file, &bye->two, chunk, "two", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool TEST_writeBye(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const TEST_Bye *bye = (const TEST_Bye*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeLongField(file, bye->one, chunk, "one", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeLongField(file, bye->two, chunk, "two", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool TEST_checkBye(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return TRUE;
}

void TEST_freeBye(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
}

void TEST_printBye(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const TEST_Bye *bye = (const TEST_Bye*)chunk;

    IFF_printIndent(stdout, indentLevel, "one = %d;\n", bye->one);
    IFF_printIndent(stdout, indentLevel, "two = %d;\n", bye->two);
}

IFF_Bool TEST_compareBye(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const TEST_Bye *bye1 = (const TEST_Bye*)chunk1;
    const TEST_Bye *bye2 = (const TEST_Bye*)chunk2;

    if(bye1->one != bye2->one)
        return FALSE;

    if(bye1->two != bye2->two)
        return FALSE;

    return TRUE;
}
