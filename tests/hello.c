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
#include <field.h>
#include <error.h>
#include <util.h>
#include "test.h"

IFF_Chunk *TEST_createHelloChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    TEST_Hello *hello = (TEST_Hello*)IFF_createChunk(chunkId, chunkSize, sizeof(TEST_Hello));

    if(hello != NULL)
    {
        hello->a = '\0';
        hello->b = '\0';
        hello->c = 0;
    }

    return (IFF_Chunk*)hello;
}

TEST_Hello *TEST_createHello(const IFF_Long chunkSize)
{
    return (TEST_Hello*)TEST_createHelloChunk(TEST_ID_HELO, chunkSize);
}

IFF_Bool TEST_readHello(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    TEST_Hello *hello = (TEST_Hello*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readUByteField(file, &hello->a, chunk, attributePath, "a", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &hello->b, chunk, attributePath, "b", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUWordField(file, &hello->c, chunk, attributePath, "c", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool TEST_writeHello(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeUByteField(file, hello->a, chunk, attributePath, "a", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, hello->b, chunk, attributePath, "b", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUWordField(file, hello->c, chunk, attributePath, "c", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool TEST_checkHello(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessage printCheckMessage, void *data)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;

    if((hello->c < 0) || (hello->c > 1024))
    {
        printCheckMessage(attributePath, "c", chunk->chunkId, data, "must be between 0 and 1024, value is: %u", hello->c);
        return FALSE;
    }

    return TRUE;
}

void TEST_freeHello(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
}

void TEST_printHello(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;

    IFF_printIndent(stdout, indentLevel, "a = %c;\n", hello->a);
    IFF_printIndent(stdout, indentLevel, "b = %c;\n", hello->b);
    IFF_printIndent(stdout, indentLevel, "c = %u;\n", hello->c);
}

IFF_Bool TEST_compareHello(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const TEST_Hello *hello1 = (const TEST_Hello*)chunk1;
    const TEST_Hello *hello2 = (const TEST_Hello*)chunk2;

    if(hello1->a != hello2->a)
        return FALSE;

    if(hello1->b != hello2->b)
        return FALSE;

    if(hello1->c != hello2->c)
        return FALSE;

    return TRUE;
}
