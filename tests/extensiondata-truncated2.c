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

#include "extensiondata-truncated2.h"
#include <form.h>
#include "test.h"
#include "hello.h"
#include "bye.h"

static IFF_Chunk *createHelloChunk(void)
{
    TEST_Hello *hello = (TEST_Hello*)TEST_createHello(TEST_HELO_DEFAULT_SIZE - sizeof(IFF_UByte)); /* Truncated so that the size is in the middle of the last field */

    hello->a = 'a';
    hello->b = 'b';

    return (IFF_Chunk*)hello;
}

static IFF_Chunk *createByeChunk(void)
{
    TEST_Bye *bye = (TEST_Bye*)TEST_createBye(TEST_BYE_DEFAULT_SIZE - sizeof(IFF_Word)); /* Truncated so that the size is in the middle of the last field */

    bye->one = 1;

    return (IFF_Chunk*)bye;
}

IFF_Form *IFF_createTestForm(void)
{
    IFF_Chunk *hello = createHelloChunk();
    IFF_Chunk *bye = createByeChunk();
    IFF_Form *form = IFF_createEmptyForm(TEST_ID_TEST);

    IFF_addToForm(form, hello);
    IFF_addToForm(form, bye);

    return form;
}
