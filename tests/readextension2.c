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
#include <stdio.h>
#include "extensiondata2.h"

int main(int argc, char *argv[])
{
    IFF_IOError *error = NULL;
    IFF_Chunk *chunk = TEST_read("extension2.TEST", &error);
    int status;

    if(error == NULL)
    {
        TEST_Conversation *conversation = IFF_createTestConversation();
        status = !TEST_compare(chunk, (IFF_Chunk*)conversation);
        TEST_free((IFF_Chunk*)conversation);
    }
    else
    {
        IFF_printReadError(stderr, error);
        IFF_freeIOError(error);
        status = 1;
    }

    TEST_free(chunk);

    return status;
}
