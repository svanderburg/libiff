#include <iff.h>
#include <chunk.h>
#include <cat.h>
#include <form.h>
#include <rawchunk.h>
#include <error.h>
#include "catdata.h"

#define HI_BYTES_SIZE 2
#define HEY_BYTES_SIZE 3

#define ID_HI IFF_MAKEID('H', 'I', ' ', ' ')
#define ID_HEY IFF_MAKEID('H', 'E', 'Y', ' ')

IFF_UByte hiData[] = {'H', 'i'};
IFF_UByte heyData[] = {'H', 'e', 'y'};

static IFF_Chunk *createTestDataChunk(const IFF_ID chunkId, const IFF_Long chunkSize, IFF_UByte *data)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createRawChunk(chunkId, chunkSize);
    IFF_copyDataToRawChunkData(rawChunk, data);

    return (IFF_Chunk*)rawChunk;
}

static IFF_Chunk *createHiChunk(void)
{
    return createTestDataChunk(ID_HI, HI_BYTES_SIZE, hiData);
}

static IFF_Chunk *createHeyChunk(void)
{
    return createTestDataChunk(ID_HEY, HEY_BYTES_SIZE, heyData);
}

static IFF_Chunk *addChunkAndCheck(const IFF_CAT *cat, IFF_Form *test2Form)
{
    IFF_QualityLevel qualityLevel;

    /* Create and attach a data chunk */
    IFF_Chunk *hiChunk = createHiChunk();
    IFF_addChunkToForm(test2Form, hiChunk);

    /* Check if the Hi chunk is the last chunk */
    if(test2Form->chunks[2] != hiChunk)
    {
        fprintf(stderr, "The last chunk should be the 'HI  ' chunk!\n");
        IFF_free((IFF_Chunk*)hiChunk);
        return NULL;
    }

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)cat);

    if(qualityLevel == IFF_QUALITY_PERFECT)
        return hiChunk;
    else
    {
        IFF_free((IFF_Chunk*)hiChunk);
        return NULL;
    }
}

static IFF_Bool updateChunkAndCheck(const IFF_CAT *cat, IFF_Form *test2Form, const IFF_Chunk *hiChunk)
{
    IFF_QualityLevel qualityLevel;
    IFF_Chunk *heyChunk = createHeyChunk();
    IFF_Chunk *obsoleteChunk = IFF_updateChunkInForm(test2Form, 2, heyChunk);

    if(obsoleteChunk != hiChunk)
    {
        fprintf(stderr, "The obsolete chunk is not the 'HI  ' chunk!\n");
        return FALSE;
    }

    if(test2Form->chunks[2] != heyChunk)
    {
        fprintf(stderr, "The last chunk should be the 'HEY ' chunk!\n");
        return FALSE;
    }

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)cat);

    IFF_free((IFF_Chunk*)obsoleteChunk);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Bool removeMiddleChunkAndCheck(const IFF_CAT *cat, IFF_Form *test2Form)
{
    IFF_QualityLevel qualityLevel;
    IFF_Chunk *byeChunk = test2Form->chunks[1];
    IFF_Chunk *obsoleteChunk = IFF_removeChunkFromForm(test2Form, 1);
    IFF_Bool result = TRUE;

    if(obsoleteChunk != byeChunk)
    {
        fprintf(stderr, "The removed chunk should be the 'BYE ' chunk!\n");
        result = FALSE;
    }

    if(test2Form->chunksLength != 2)
    {
        fprintf(stderr, "The FORM should contain 2 sub chunks, but it has: %d\n", test2Form->chunksLength);
        result = FALSE;
    }

    IFF_free((IFF_Chunk*)obsoleteChunk);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)cat);
    if(qualityLevel != IFF_QUALITY_PERFECT)
        result = FALSE;

    return result;
}

int main(int argc, char *argv[])
{
    IFF_Chunk *hiChunk;
    int status;

    /* Create existing test CAT structure */
    IFF_CAT *cat = IFF_createTestCAT();

    /* Request the second FORM */
    IFF_Form *test2Form = (IFF_Form*)cat->chunks[1];

    /* Execute CRUD operations and check */
    if((hiChunk = addChunkAndCheck(cat, test2Form)) != NULL &&
      updateChunkAndCheck(cat, test2Form, hiChunk) &&
      removeMiddleChunkAndCheck(cat, test2Form))
        status = 0;
    else
        status = 1;

    /* Cleanup and return */
    IFF_free((IFF_Chunk*)cat);
    return status;
}
