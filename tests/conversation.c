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
#include "form.h"
#include "pointerarray.h"

static IFF_GroupMember groupMembers[] = {
    { TEST_ID_HELO, "hello", IFF_GROUP_MEMBER_SINGLE },
    { TEST_ID_BYE, "bye", IFF_GROUP_MEMBER_SINGLE },
    { TEST_ID_MESG, "messages", IFF_GROUP_MEMBER_MULTIPLE },
};

typedef enum
{
    FIELD_INDEX_HELLO = 0,
    FIELD_INDEX_BYE = 1,
    FIELD_INDEX_MESSAGES = 2
}
FieldIndex;

static IFF_Bool mapChunkIdToFieldIndex(const IFF_ID chunkId, unsigned int *index)
{
    switch(chunkId)
    {
        case TEST_ID_HELO:
            *index = FIELD_INDEX_HELLO;
            return TRUE;
        case TEST_ID_BYE:
            *index = FIELD_INDEX_BYE;
            return TRUE;
        case TEST_ID_MESG:
            *index = FIELD_INDEX_MESSAGES;
            return TRUE;
        default:
            return FALSE;
    }
}

static IFF_Chunk **getFieldPointer(const IFF_Group *group, const unsigned int index)
{
    const TEST_Conversation *conversation = (const TEST_Conversation*)group;

    switch(index)
    {
        case FIELD_INDEX_HELLO:
            return (IFF_Chunk**)&conversation->hello;
        case FIELD_INDEX_BYE:
            return (IFF_Chunk**)&conversation->bye;
        default:
            return NULL;
    }
}

static IFF_Chunk ***getArrayFieldPointer(const IFF_Group *group, const unsigned int index, unsigned int **chunksLength)
{
    TEST_Conversation *conversation = (TEST_Conversation*)group;

    switch(index)
    {
        case FIELD_INDEX_MESSAGES:
            *chunksLength = &conversation->messagesLength;
            return (IFF_Chunk***)&conversation->messages;
        default:
            *chunksLength = 0;
            return NULL;
    }
}

IFF_GroupStructure TEST_conversationStructure = {
    sizeof(TEST_Conversation),
    TEST_NUM_OF_CONVERSATION_GROUP_MEMBERS,
    groupMembers,
    mapChunkIdToFieldIndex,
    getFieldPointer,
    getArrayFieldPointer
};

TEST_Conversation *TEST_createConversation(void)
{
    return (TEST_Conversation*)IFF_createEmptyForm(TEST_ID_CONV, &TEST_conversationStructure);
}

TEST_Conversation *TEST_createSharedConversation(void)
{
    return (TEST_Conversation*)IFF_createEmptyProp(TEST_ID_CONV, &TEST_conversationStructure);
}

TEST_Conversation *TEST_evaluateConversation(TEST_Conversation *conversation)
{
    return (TEST_Conversation*)IFF_evaluateGroup((IFF_Group*)conversation);
}

void TEST_freeEvaluatedConversation(TEST_Conversation *evaluatedConversation)
{
    IFF_freeEvaluatedGroup((IFF_Group*)evaluatedConversation);
}

void TEST_addChunkToConversation(TEST_Conversation *conversation, IFF_Chunk *chunk)
{
    IFF_addChunkToForm((IFF_Form*)conversation, chunk);
}

IFF_Chunk *TEST_updateChunkInConversation(TEST_Conversation *conversation, IFF_Chunk *chunk)
{
    return IFF_updateChunkInGroupStructure((IFF_Group*)conversation, chunk);
}

IFF_Chunk *TEST_removeChunkFromConversation(TEST_Conversation *conversation, const IFF_ID chunkId)
{
    return IFF_removeChunkFromGroupStructure((IFF_Group*)conversation, chunkId);
}

IFF_Chunk *TEST_updateChunkInConversationByIndex(TEST_Conversation *conversation, const unsigned int index, IFF_Chunk *chunk)
{
    return IFF_updateChunkInGroupStructureByIndex((IFF_Group*)conversation, index, chunk);
}

IFF_Chunk *TEST_removeChunkFromConversationByIndex(TEST_Conversation *conversation, const IFF_ID chunkId, const unsigned int index)
{
    return IFF_removeChunkFromGroupStructureByIndex((IFF_Group*)conversation, chunkId, index);
}

TEST_Hello *TEST_getHello(const TEST_Conversation *conversation)
{
    return (TEST_Hello*)IFF_getPropertyFromGroupStructure((const IFF_Group*)conversation, FIELD_INDEX_HELLO);
}

TEST_Bye *TEST_getBye(const TEST_Conversation *conversation)
{
    return (TEST_Bye*)IFF_getPropertyFromGroupStructure((const IFF_Group*)conversation, FIELD_INDEX_BYE);
}

IFF_TextChunk **TEST_getMessages(const TEST_Conversation *conversation, unsigned int *messagesLength)
{
    return (IFF_TextChunk**)IFF_getPropertiesFromGroupStructure((const IFF_Group*)conversation, FIELD_INDEX_MESSAGES, messagesLength);
}
