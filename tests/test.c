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

#include "test.h"
#include "iff.h"
#include "hello.h"
#include "bye.h"

#define TEST_NUM_OF_FORM_TYPES 1
#define TEST_NUM_OF_EXTENSION_CHUNKS 2

static IFF_FormExtension testFormExtension[] = {
    {"BYE ", &TEST_readBye, &TEST_writeBye, &TEST_checkBye, &TEST_freeBye, &TEST_printBye, &TEST_compareBye},
    {"HELO", &TEST_readHello, &TEST_writeHello, &TEST_checkHello, &TEST_freeHello, &TEST_printHello, &TEST_compareHello}
};

static IFF_Extension extension[] = {
    {"TEST", TEST_NUM_OF_EXTENSION_CHUNKS, testFormExtension}
};

IFF_Chunk *TEST_read(const char *filename)
{
    return IFF_read(filename, extension, TEST_NUM_OF_FORM_TYPES);
}

int TEST_write(const char *filename, const IFF_Chunk *chunk)
{
    return IFF_write(filename, chunk, extension, TEST_NUM_OF_FORM_TYPES);
}

void TEST_free(IFF_Chunk *chunk)
{
    IFF_free(chunk, extension, TEST_NUM_OF_FORM_TYPES);
}

int TEST_check(const IFF_Chunk *chunk)
{
    return IFF_check(chunk, extension, TEST_NUM_OF_FORM_TYPES);
}

void TEST_print(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_print(chunk, indentLevel, extension, TEST_NUM_OF_FORM_TYPES);
}

int TEST_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compare(chunk1, chunk2, extension, TEST_NUM_OF_FORM_TYPES);
}
