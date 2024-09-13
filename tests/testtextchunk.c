#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <form.h>
#include <textchunk.h>
#include <iff.h>

static IFF_Bool checkInitialTextChunk(IFF_Form *form, IFF_TextChunk *textChunk, const char *initialText)
{
    if(memcmp(initialText, textChunk->chunkData, textChunk->chunkSize) != 0)
    {
        fprintf(stderr, "The initial text is not correctly set!\n");
        return FALSE;
    }

    if(IFF_check((const IFF_Chunk*)form) != IFF_QUALITY_PERFECT)
    {
        fprintf(stderr, "The form should be of perfect quality!\n");
        return FALSE;
    }

    return TRUE;
}

static IFF_Bool updateWithNewTextAndCheck(IFF_Form *form, IFF_TextChunk *textChunk)
{
    char *newText = "New text";
    IFF_Long obsoleteTextLength;
    char *oldText = IFF_updateTextData(textChunk, newText, &obsoleteTextLength);

    if(memcmp(newText, textChunk->chunkData, textChunk->chunkSize) != 0)
    {
        fprintf(stderr, "The updated text is not correctly set!\n");
        IFF_printFd(stderr, (IFF_Chunk*)form, 0);
        return FALSE;
    }

    if(IFF_check((const IFF_Chunk*)form) != IFF_QUALITY_PERFECT)
    {
        fprintf(stderr, "The form should be of perfect quality!\n");
        return FALSE;
    }

    free(oldText);

    return TRUE;
}

int main(int argc, char *argv[])
{
    int status;
    char *initialText = "Initial text";
    IFF_TextChunk *textChunk = IFF_createTextChunkFromText(IFF_ID_TEXT, initialText);
    IFF_Form *form = IFF_createEmptyForm(IFF_ID_TEXT, NULL);

    IFF_addChunkToForm(form, (IFF_Chunk*)textChunk);

    if(checkInitialTextChunk(form, textChunk, initialText) &&
        updateWithNewTextAndCheck(form, textChunk))
        status = 0;
    else
        status = 1;

    IFF_free((IFF_Chunk*)form);
    return status;
}
