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
#include "test.h"
#include <list.h>

static TEST_Hello *createSharedHello(void)
{
    TEST_Hello *hello = TEST_createHello(TEST_HELO_DEFAULT_SIZE);

    hello->a = 'a';
    hello->b = 'b';
    hello->c = 1;

    return hello;
}

static TEST_Conversation *createSharedConversation(TEST_Hello *hello)
{
    TEST_Conversation *conversation = TEST_createSharedConversation();
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)hello);
    return conversation;
}

static TEST_Bye *createBye(void)
{
    TEST_Bye *bye = TEST_createBye(TEST_BYE_DEFAULT_SIZE);

    bye->one = 1;
    bye->two = 2;

    return bye;
}

static TEST_Conversation *createConversation(TEST_Bye *bye)
{
    TEST_Conversation *conversation = TEST_createConversation();
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)bye);
    return conversation;
}

static IFF_List *createListWithSharedProperty(TEST_Conversation *conversation, TEST_Conversation *sharedConversation)
{
    IFF_List *list = IFF_createEmptyList();
    IFF_addChunkToListAndUpdateContentsType(list, (IFF_Chunk*)conversation);
    IFF_addChunkToListAndUpdateContentsType(list, (IFF_Chunk*)sharedConversation);
    return list;
}

static IFF_Bool testListWithSharedProperty(void)
{
    TEST_Bye *bye = createBye();
    TEST_Conversation *conversation = createConversation(bye);
    TEST_Hello *sharedHello = createSharedHello();
    TEST_Conversation *sharedConversation = createSharedConversation(sharedHello);
    IFF_List *list = createListWithSharedProperty(conversation, sharedConversation);

    if(TEST_getHello(conversation) != sharedHello)
    {
        fprintf(stderr, "The getHello() function should return a shared hello chunk!\n");
        return FALSE;
    }

    if(TEST_getBye(conversation) != bye)
    {
        fprintf(stderr, "The getBye() function should return a bye chunk!\n");
        return FALSE;
    }

    TEST_free((IFF_Chunk*)list);

    return TRUE;
}

static TEST_Hello *createHello(void)
{
    TEST_Hello *hello = TEST_createHello(TEST_HELO_DEFAULT_SIZE);

    hello->a = 'A';
    hello->b = 'B';
    hello->c = 2;

    return hello;
}

static TEST_Conversation *createConversation2(TEST_Hello *hello, TEST_Bye *bye)
{
    TEST_Conversation *conversation = TEST_createConversation();
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)hello);
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)bye);
    return conversation;
}

static IFF_Bool testListWithOverriddenProperty(void)
{
    TEST_Hello *hello = createHello();
    TEST_Bye *bye = createBye();
    TEST_Conversation *conversation = createConversation2(hello, bye);
    TEST_Hello *sharedHello = createSharedHello();
    TEST_Conversation *sharedConversation = createSharedConversation(sharedHello);
    IFF_List *list = createListWithSharedProperty(conversation, sharedConversation);

    if(TEST_getHello(conversation) != hello)
    {
        fprintf(stderr, "The getHello() function should return the non-shared hello chunk!\n");
        return FALSE;
    }

    if(TEST_getBye(conversation) != bye)
    {
        fprintf(stderr, "The getBye() function should return a bye chunk!\n");
        return FALSE;
    }

    TEST_free((IFF_Chunk*)list);

    return TRUE;
}

static TEST_Conversation *createSharedConversatonWithMessages(IFF_TextChunk *sharedMessage1, IFF_TextChunk *sharedMessage2)
{
    TEST_Conversation *conversation = TEST_createSharedConversation();

    TEST_addChunkToConversation(conversation, (IFF_Chunk*)sharedMessage1);
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)sharedMessage2);

    return conversation;
}

static TEST_Conversation *createConversatonWithMessages(IFF_TextChunk *message1, IFF_TextChunk *message2)
{
    TEST_Conversation *conversation = TEST_createConversation();

    TEST_addChunkToConversation(conversation, (IFF_Chunk*)message1);
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)message2);

    return conversation;
}

static IFF_List *createListWithMessages(TEST_Conversation *sharedConversation, TEST_Conversation *conversation)
{
    IFF_List *list = IFF_createEmptyList();

    IFF_addChunkToListAndUpdateContentsType(list, (IFF_Chunk*)sharedConversation);
    IFF_addChunkToListAndUpdateContentsType(list, (IFF_Chunk*)conversation);

    return list;
}

static IFF_Bool testListWithSharedMessages(void)
{
    IFF_TextChunk *sharedMessage1 = IFF_createTextChunkFromText(TEST_ID_MESG, "Hi");
    IFF_TextChunk *sharedMessage2 = IFF_createTextChunkFromText(TEST_ID_MESG, "everybody");

    IFF_TextChunk *message1 = IFF_createTextChunkFromText(TEST_ID_MESG, "hello");
    IFF_TextChunk *message2 = IFF_createTextChunkFromText(TEST_ID_MESG, "world!");

    TEST_Conversation *sharedConversation = createSharedConversatonWithMessages(sharedMessage1, sharedMessage2);
    TEST_Conversation *conversation = createConversatonWithMessages(message1, message2);

    IFF_List *list = createListWithMessages(sharedConversation, conversation);

    IFF_TextChunk **allMessages;
    unsigned int allMessagesLength;

    allMessages = TEST_getMessages(conversation, &allMessagesLength);

    if(allMessagesLength != 4)
    {
        fprintf(stderr, "We should have retrieved 4 messages, instead we have: %u\n", allMessagesLength);
        return FALSE;
    }

    if(allMessages[0] != sharedMessage1)
    {
        fprintf(stderr, "message[0] should be sharedMessage1!\n");
        return FALSE;
    }

    if(allMessages[1] != sharedMessage2)
    {
        fprintf(stderr, "message[1] should be sharedMessage2!\n");
        return FALSE;
    }

    if(allMessages[2] != message1)
    {
        fprintf(stderr, "message[2] should be message1!\n");
        return FALSE;
    }

    if(allMessages[3] != message2)
    {
        fprintf(stderr, "message[3] should be message2!\n");
        return FALSE;
    }

    free(allMessages);
    TEST_free((IFF_Chunk*)list);

    return TRUE;
}

int main(int argc, char *argv[])
{
    if(testListWithSharedProperty() &&
        testListWithOverriddenProperty() &&
        testListWithSharedMessages())
        return 0;
    else
        return 1;
}
