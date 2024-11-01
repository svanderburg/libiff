#include "conversation.h"
#include <stdio.h>
#include "test.h"

static IFF_Chunk *createHello1Chunk(void)
{
    TEST_Hello *hello = TEST_createHello(TEST_HELO_DEFAULT_SIZE);

    hello->a = 'a';
    hello->b = 'b';
    hello->c = 512;

    return (IFF_Chunk*)hello;
}

static IFF_Bool addHelloChunkAndCheck(TEST_Conversation *conversation)
{
    IFF_QualityLevel qualityLevel;
    IFF_Chunk *hello1 = createHello1Chunk();

    TEST_addChunkToConversation(conversation, hello1);

    if(conversation->chunksLength != 0)
    {
        fprintf(stderr, "There should not be any arbitrary chunks attached, instead we have: %u\n", conversation->chunksLength);
        return FALSE;
    }

    if((IFF_Chunk*)conversation->hello != hello1)
    {
        fprintf(stderr, "conversation->hello should point to the hello1 chunk!\n");
        return FALSE;
    }

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = TEST_check((IFF_Chunk*)conversation);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Chunk *createHello2Chunk(void)
{
    TEST_Hello *hello = TEST_createHello(TEST_HELO_DEFAULT_SIZE);

    hello->a = 'A';
    hello->b = 'B';
    hello->c = 128;

    return (IFF_Chunk*)hello;
}

static IFF_Bool updateHelloChunkAndCheck(TEST_Conversation *conversation)
{
    IFF_QualityLevel qualityLevel;
    IFF_Chunk *hello2 = createHello2Chunk();
    IFF_Chunk *obsoleteChunk = TEST_updateChunkInConversation(conversation, hello2);

    if(conversation->chunksLength != 0)
    {
        fprintf(stderr, "There should not be any arbitrary chunks attached, instead we have: %u\n", conversation->chunksLength);
        return FALSE;
    }

    if((IFF_Chunk*)conversation->hello != hello2)
    {
        fprintf(stderr, "conversation->hello should point to the hello2 chunk!\n");
        return FALSE;
    }

    IFF_free(obsoleteChunk);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = TEST_check((IFF_Chunk*)conversation);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Bool removeHelloChunkAndCheck(TEST_Conversation *conversation)
{
    IFF_QualityLevel qualityLevel;
    IFF_Chunk *obsoleteChunk = TEST_removeChunkFromConversation(conversation, TEST_ID_HELO);

    if(conversation->chunksLength != 0)
    {
        fprintf(stderr, "There should not be any arbitrary chunks attached, instead we have: %u\n", conversation->chunksLength);
        return FALSE;
    }

    if(conversation->hello != NULL)
    {
        fprintf(stderr, "conversation->hello should be NULL!\n");
        return FALSE;
    }

    IFF_free(obsoleteChunk);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = TEST_check((IFF_Chunk*)conversation);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Bool addTwoHelloChunksAndCheck(TEST_Conversation *conversation)
{
    IFF_QualityLevel qualityLevel;
    IFF_Chunk *hello1 = createHello1Chunk();
    IFF_Chunk *hello2 = createHello2Chunk();

    TEST_addChunkToConversation(conversation, hello1);
    TEST_addChunkToConversation(conversation, hello2);

    if(conversation->chunksLength != 1)
    {
        fprintf(stderr, "There should be one arbitrary chunk attached, instead we have: %u\n", conversation->chunksLength);
        return FALSE;
    }

    if((IFF_Chunk*)conversation->hello != hello2)
    {
        fprintf(stderr, "conversation->hello should point to the hello2 chunk!\n");
        return FALSE;
    }

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = TEST_check((IFF_Chunk*)conversation);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Bool removeTwoHelloChunksAndCheck(TEST_Conversation *conversation)
{
    IFF_QualityLevel qualityLevel;
    TEST_Hello *obsoleteChunk = (TEST_Hello*)TEST_removeChunkFromConversation(conversation, TEST_ID_HELO);

    /* Check if the first removed chunk is the last added */
    if(obsoleteChunk->c != 128)
    {
        fprintf(stderr, "The first removed hello object should be the second added!\n");
        return FALSE;
    }

    IFF_free((IFF_Chunk*)obsoleteChunk);

    /* Check if the second removed chunk is the first added */
    obsoleteChunk = (TEST_Hello*)TEST_removeChunkFromConversation(conversation, TEST_ID_HELO);

    if(obsoleteChunk->c != 512)
    {
        fprintf(stderr, "The second removed hello object should be the first added!\n");
        return FALSE;
    }

    IFF_free((IFF_Chunk*)obsoleteChunk);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = TEST_check((IFF_Chunk*)conversation);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Bool addMessageAndCheck(TEST_Conversation *conversation)
{
    IFF_TextChunk *message = IFF_createTextChunkFromText(TEST_ID_MESG, "A random message!");

    TEST_addChunkToConversation(conversation, (IFF_Chunk*)message);

    if(conversation->messagesLength != 1)
    {
        fprintf(stderr, "There should be one message attached, instead we have: %u\n", conversation->messagesLength);
        return FALSE;
    }

    if(conversation->messages[0] != message)
    {
        fprintf(stderr, "The first chunk should be a message chunk!\n");
        return FALSE;
    }

    return TRUE;
}

static IFF_Bool updateMessageAndCheck(TEST_Conversation *conversation)
{
    IFF_TextChunk *message = IFF_createTextChunkFromText(TEST_ID_MESG, "Another message!");
    IFF_Chunk *obsoleteChunk = TEST_updateChunkInConversationByIndex(conversation, 0, (IFF_Chunk*)message);

    if(conversation->messagesLength != 1)
    {
        fprintf(stderr, "There should be one message attached, instead we have: %u\n", conversation->messagesLength);
        return FALSE;
    }

    if(conversation->messages[0] != message)
    {
        fprintf(stderr, "The first chunk should be the new message chunk!\n");
        return FALSE;
    }

    IFF_free(obsoleteChunk);

    return TRUE;
}

static IFF_Bool removeMessageAndCheck(TEST_Conversation *conversation)
{
    IFF_Chunk *obsoleteChunk = TEST_removeChunkFromConversationByIndex(conversation, TEST_ID_MESG, 0);

    if(conversation->messagesLength != 0)
    {
        fprintf(stderr, "There should not be any message attached, instead we have: %u\n", conversation->messagesLength);
        return FALSE;
    }

    IFF_free(obsoleteChunk);

    return TRUE;
}

static TEST_Greetings *createGreetingsChunk(void)
{
    TEST_Greet *greet1, *greet2, *greet3;
    TEST_Greetings *greetings = TEST_createGreetings();

    greet1 = TEST_addGreetToGreetings(greetings);
    greet1->opening = TEST_OPENING_HELLO;
    greet1->closure = TEST_CLOSURE_BYE;

    greet2 = TEST_addGreetToGreetings(greetings);
    greet2->opening = TEST_OPENING_HI;
    greet2->closure = TEST_CLOSURE_SEEYA;

    greet3 = TEST_addGreetToGreetings(greetings);
    greet3->opening = TEST_OPENING_GREETINGS;
    greet3->closure = TEST_CLOSURE_GOOD_DAY;

    return greetings;
}

static IFF_Bool addGreetingsAndCheck(TEST_Conversation *conversation)
{
    TEST_Greetings *greetings = createGreetingsChunk();
    TEST_addChunkToConversation(conversation, (IFF_Chunk*)greetings);

    if(greetings->greetsLength != 3)
    {
        fprintf(stderr, "There should be 3 greets attached, instead we have: %u\n", greetings->greetsLength);
        return FALSE;
    }

    if(greetings->greets[0].opening != TEST_OPENING_HELLO)
    {
        fprintf(stderr, "The first opening greet should be: %u, instead it is: %u\n", TEST_OPENING_HELLO, greetings->greets[0].opening);
        return FALSE;
    }

    if(greetings->greets[1].opening != TEST_OPENING_HI)
    {
        fprintf(stderr, "The second opening greet should be: %u, instead it is: %u\n", TEST_OPENING_HI, greetings->greets[1].opening);
        return FALSE;
    }

    if(greetings->greets[2].opening != TEST_OPENING_GREETINGS)
    {
        fprintf(stderr, "The third opening greet should be: %u, instead it is: %u\n", TEST_OPENING_GREETINGS, greetings->greets[2].opening);
        return FALSE;
    }

    return TRUE;
}

static IFF_Bool removeGreetAndCheck(TEST_Conversation *conversation)
{
    TEST_Greetings *greetings = conversation->greetings;

    TEST_removeGreetFromGreetings(greetings, 1); /* Remove the middle greet */

    if(greetings->greetsLength != 2)
    {
        fprintf(stderr, "There should be 2 greets attached, instead we have: %u\n", greetings->greetsLength);
        return FALSE;
    }

    if(greetings->greets[0].opening != TEST_OPENING_HELLO)
    {
        fprintf(stderr, "The first opening greet should be: %u, instead it is: %u\n", TEST_OPENING_HELLO, greetings->greets[0].opening);
        return FALSE;
    }

    if(greetings->greets[1].opening != TEST_OPENING_GREETINGS)
    {
        fprintf(stderr, "The second opening greet should be: %u, instead it is: %u\n", TEST_OPENING_GREETINGS, greetings->greets[1].opening);
        return FALSE;
    }

    return TRUE;
}

int main(int argc, char *argv[])
{
    int status;
    TEST_Conversation *conversation = TEST_createConversation();

    if(addHelloChunkAndCheck(conversation) &&
        updateHelloChunkAndCheck(conversation) &&
        removeHelloChunkAndCheck(conversation) &&
        addTwoHelloChunksAndCheck(conversation) &&
        removeTwoHelloChunksAndCheck(conversation) &&
        addMessageAndCheck(conversation) &&
        updateMessageAndCheck(conversation) &&
        removeMessageAndCheck(conversation) &&
        addGreetingsAndCheck(conversation) &&
        removeGreetAndCheck(conversation))
        status = 0;
    else
        status = 1;

    IFF_free((IFF_Chunk*)conversation);

    return status;
}
