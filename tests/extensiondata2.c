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

#include "extensiondata.h"
#include <textchunk.h>
#include "hello.h"
#include "bye.h"

static IFF_Chunk *createHelloChunk(void)
{
    TEST_Hello *hello = TEST_createHello(TEST_HELO_DEFAULT_SIZE);

    hello->a = 'a';
    hello->b = 'b';
    hello->c = 4096;

    return (IFF_Chunk*)hello;
}

static IFF_Chunk *createByeChunk(void)
{
    TEST_Bye *bye = TEST_createBye(TEST_BYE_DEFAULT_SIZE);

    bye->one = 1;
    bye->two = 2;

    return (IFF_Chunk*)bye;
}

TEST_Conversation *IFF_createTestConversation(void)
{
    IFF_Chunk *hello = createHelloChunk();
    IFF_Chunk *bye = createByeChunk();
    IFF_Chunk *message1 = (IFF_Chunk*)IFF_createTextChunkFromText(TEST_ID_MESG, "A random message!");
    IFF_Chunk *message2 = (IFF_Chunk*)IFF_createTextChunkFromText(TEST_ID_MESG, "Another message!");
    TEST_Conversation *conversation = TEST_createConversation();

    TEST_addChunkToConversation(conversation, hello);
    TEST_addChunkToConversation(conversation, message1);
    TEST_addChunkToConversation(conversation, message2);
    TEST_addChunkToConversation(conversation, bye);

    return conversation;
}
