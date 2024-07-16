#include <iff.h>
#include "catdata.h"

#define ID_TEST IFF_MAKEID('T', 'E', 'S', 'T')
#define ID_NEW IFF_MAKEID('N', 'E', 'W', ' ')

static IFF_Bool addFormAndCheck(IFF_CAT *cat)
{
    IFF_QualityLevel qualityLevel;
    IFF_Form *form = IFF_createEmptyForm(ID_TEST);
    IFF_addChunkToCAT(cat, (IFF_Chunk*)form);

    if(cat->chunksLength != 3)
    {
        fprintf(stderr, "Chunks length of the CAT should be 3, instead it is: %d\n", cat->chunksLength);
        return FALSE;
    }

    if(cat->chunks[2] != (IFF_Chunk*)form)
    {
        fprintf(stderr, "The last form should be a FORM with formType: 'TEST'\n");
        return FALSE;
    }

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)cat);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Form *updateFormAndCheck(IFF_CAT *cat)
{
    IFF_QualityLevel qualityLevel;
    IFF_Form *newForm = IFF_createEmptyForm(ID_NEW);
    IFF_Chunk *previousMiddleChunk = cat->chunks[1];
    IFF_Chunk *obsoleteChunk = IFF_updateChunkInCATAndUpdateContentsTypeByIndex(cat, 1, (IFF_Chunk*)newForm);

    if(obsoleteChunk != previousMiddleChunk)
    {
         fprintf(stderr, "The obsolete chunk is not the previous middle chunk!\n");
         return NULL;
    }

    if(cat->chunks[1] != (IFF_Chunk*)newForm)
    {
        fprintf(stderr, "The middle chunk should be a FORM with formType 'NEW '!\n");
        return NULL;
    }

    IFF_free((IFF_Chunk*)obsoleteChunk);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)cat);

    if(qualityLevel == IFF_QUALITY_PERFECT)
        return newForm;
    else
        return NULL;
}

static IFF_Bool removeForm(IFF_CAT *cat, const IFF_Form *newForm)
{
    IFF_QualityLevel qualityLevel;
    IFF_Bool result = TRUE;
    IFF_Chunk *obsoleteChunk = IFF_removeChunkFromCATByIndex(cat, 1);

    if(obsoleteChunk != (IFF_Chunk*)newForm)
    {
        fprintf(stderr, "The removed chunk should be the 'NEW ' chunk!\n");
        result = FALSE;
    }

    if(cat->chunksLength != 2)
    {
        fprintf(stderr, "The CAT should contain 2 sub chunks, but it has: %d\n", cat->chunksLength);
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
    IFF_Form *newForm;
    int status = 0;

    /* Create existing test CAT structure */
    IFF_CAT *cat = IFF_createTestCAT();

    /* Execute CRUD operations and check */
    if(addFormAndCheck(cat) &&
        (newForm = updateFormAndCheck(cat)) != NULL &&
        removeForm(cat, newForm))
        status = 0;
    else
        status = 1;

    /* Cleanup and return */
    IFF_free((IFF_Chunk*)cat);
    return status;
}
