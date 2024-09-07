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
#include "form.h"

static void initConversationContents(IFF_Group *group)
{
    TEST_Conversation *conversation = (TEST_Conversation*)group;
    conversation->hello = NULL;
    conversation->bye = NULL;
}

static IFF_Bool attachChunkToConversation(IFF_Group *group, IFF_Chunk *chunk)
{
    TEST_Conversation *conversation = (TEST_Conversation*)group;

    switch(chunk->chunkId)
    {
        case TEST_ID_HELO:
            conversation->hello = (TEST_Hello*)chunk;
            return TRUE;
        case TEST_ID_BYE:
            conversation->bye = (TEST_Bye*)chunk;
            return TRUE;
        default:
            return FALSE;
    }
}

static IFF_Chunk *getChunkFromConversation(const IFF_Group *group, const unsigned int index)
{
    const TEST_Conversation *conversation = (const TEST_Conversation*)group;

    switch(index)
    {
        case 0:
            return (IFF_Chunk*)conversation->hello;
        case 1:
            return (IFF_Chunk*)conversation->bye;
        default:
            return NULL;
    }
}

static IFF_Chunk **getChunksFromConversation(const IFF_Group *group, const unsigned int index, unsigned int *chunksLength)
{
    return NULL;
}

static IFF_GroupMember groupMembers[] = {
    { TEST_ID_HELO, "hello", IFF_GROUP_MEMBER_SINGLE },
    { TEST_ID_BYE, "bye", IFF_GROUP_MEMBER_SINGLE }
};

IFF_GroupStructure TEST_conversationStructure = {
    sizeof(TEST_Conversation),
    2,
    groupMembers,
    initConversationContents,
    attachChunkToConversation,
    getChunkFromConversation,
    getChunksFromConversation
};

TEST_Conversation *TEST_createConversation(void)
{
    return (TEST_Conversation*)IFF_createEmptyForm(TEST_ID_CONV, &TEST_conversationStructure);
}

void TEST_addChunkToConversation(TEST_Conversation *conversation, IFF_Chunk *chunk)
{
    IFF_addChunkToForm((IFF_Form*)conversation, &TEST_conversationStructure, chunk);
}
