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

#include "conversation.h"
#include <stdlib.h>
#include <textchunk.h>
#include <iff.h>
#include "test.h"

static TEST_Hello *createHello(void)
{
    TEST_Hello *hello = TEST_createHello(TEST_HELO_DEFAULT_SIZE);

    hello->a = 'a';
    hello->b = 'b';
    hello->c = 1;

    return hello;
}

static TEST_Conversation *createTestConversation(TEST_Hello *hello, IFF_TextChunk *message)
{
    TEST_Conversation *conversation = TEST_createConversation();

    TEST_addChunkToConversation(conversation, (IFF_Chunk*)hello);
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)message);

    return conversation;
}

int main(int argc, char *argv[])
{
    int status = 0;
    IFF_QualityLevel qualityLevel;
    TEST_Hello *hello = createHello();
    IFF_TextChunk *message1 = IFF_createTextChunkFromText(TEST_ID_MESG, "Message 1");
    IFF_TextChunk *message2 = IFF_createTextChunkFromText(TEST_ID_MESG, "Message 2");
    TEST_Conversation *conversation = createTestConversation(hello, message1);

    /* Intentionally increase the size of the messages array without updating the chunk size */
    conversation->messages = (IFF_TextChunk**)realloc(conversation->messages, (conversation->messagesLength + 1) * sizeof(IFF_TextChunk*));
    conversation->messages[conversation->messagesLength] = message2;
    conversation->messagesLength++;

    /* The IFF file should be invalid now as the conversation chunk size is too small */
    if((qualityLevel = TEST_check((IFF_Chunk*)conversation)) == IFF_QUALITY_RECOVERED)
    {
        /* Recalculate the chunk sizes */
        IFF_recalculateChunkSizes((IFF_Chunk*)conversation);

        /* Now the IFF file should be valid */

        if((qualityLevel = TEST_check((IFF_Chunk*)conversation)) != IFF_QUALITY_PERFECT)
        {
            fprintf(stderr, "The IFF file should be in state: %d, but is in: %d!\n", IFF_QUALITY_PERFECT, qualityLevel);
            status = 1;
        }
    }
    else
    {
        fprintf(stderr, "The IFF file should be in state: %d, but is in: %d!\n", IFF_QUALITY_RECOVERED, qualityLevel);
        status = 1;
    }

    TEST_free((IFF_Chunk*)conversation);

    return status;
}
