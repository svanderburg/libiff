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

#ifndef __TEST_CONVERSATION_H
#define __TEST_CONVERSATION_H

typedef struct TEST_Conversation TEST_Conversation;

#include "chunk.h"
#include "id.h"
#include "groupstructure.h"
#include "textchunk.h"
#include "hello.h"
#include "bye.h"

#define TEST_ID_CONV IFF_MAKEID('C', 'O', 'N', 'V')
#define TEST_ID_MESG IFF_MAKEID('M', 'E', 'S', 'G')

#define TEST_NUM_OF_CONVERSATION_GROUP_MEMBERS 3

extern IFF_GroupStructure TEST_conversationStructure;

struct TEST_Conversation
{
    IFF_Chunk *parent;
    IFF_ChunkInterface *chunkInterface;
    IFF_ID chunkId;
    IFF_Long chunkSize;
    IFF_ID formType;
    unsigned int chunksLength;
    IFF_Chunk **chunks;
    IFF_GroupStructure *groupStructure;

    TEST_Hello *hello;
    TEST_Bye *bye;
    unsigned int messagesLength;
    IFF_TextChunk **messages;
};

TEST_Conversation *TEST_createConversation(void);

TEST_Conversation *TEST_createSharedConversation(void);

TEST_Conversation *TEST_evaluateConversation(TEST_Conversation *conversation);

void TEST_freeEvaluatedConversation(TEST_Conversation *evaluatedConversation);

void TEST_addChunkToConversation(TEST_Conversation *conversation, IFF_Chunk *chunk);

IFF_Chunk *TEST_updateChunkInConversation(TEST_Conversation *conversation, IFF_Chunk *chunk);

IFF_Chunk *TEST_removeChunkFromConversation(TEST_Conversation *conversation, const IFF_ID chunkId);

IFF_Chunk *TEST_updateChunkInConversationByIndex(TEST_Conversation *conversation, const unsigned int index, IFF_Chunk *chunk);

IFF_Chunk *TEST_removeChunkFromConversationByIndex(TEST_Conversation *conversation, const IFF_ID chunkId, const unsigned int index);

TEST_Hello *TEST_getHello(const TEST_Conversation *conversation);

TEST_Bye *TEST_getBye(const TEST_Conversation *conversation);

IFF_TextChunk **TEST_getMessages(const TEST_Conversation *conversation, unsigned int *messagesLength);

#endif
