#include <iff.h>
#include "listdata.h"

#define ID_TEST IFF_MAKEID('T', 'E', 'S', 'T')
#define ID_NEW IFF_MAKEID('N', 'E', 'W', ' ')

static IFF_Bool addFormAndCheck(IFF_List *list)
{
    IFF_QualityLevel qualityLevel;
    IFF_Form *form = IFF_createEmptyForm(ID_TEST);
    IFF_addChunkToList(list, (IFF_Chunk*)form);

    if(list->chunksLength != 3)
    {
        fprintf(stderr, "Chunks length of the LIST should be 3, instead it is: %d\n", list->chunksLength);
        return FALSE;
    }

    if(list->chunks[2] != (IFF_Chunk*)form)
    {
        fprintf(stderr, "The last form should be a FORM with formType: 'TEST'\n");
        return FALSE;
    }

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)list);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Form *updateCATAndCheck(IFF_List *list)
{
    IFF_QualityLevel qualityLevel;
    IFF_Form *newForm = IFF_createEmptyForm(ID_NEW);
    IFF_Chunk *previousMiddleChunk = list->chunks[1];
    IFF_Chunk *obsoleteChunk = IFF_updateChunkInListAndUpdateContentsTypeByIndex(list, 1, (IFF_Chunk*)newForm);

    if(obsoleteChunk != previousMiddleChunk)
    {
         fprintf(stderr, "The obsolete chunk is not the previous middle chunk!\n");
         return NULL;
    }

    if(list->chunks[1] != (IFF_Chunk*)newForm)
    {
        fprintf(stderr, "The middle chunk should be a FORM with formType 'NEW '!\n");
        return NULL;
    }

    IFF_free((IFF_Chunk*)obsoleteChunk);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)list);

    if(qualityLevel == IFF_QUALITY_PERFECT)
        return newForm;
    else
        return NULL;
}

static IFF_Bool removeForm(IFF_List *list, const IFF_Form *newForm)
{
    IFF_QualityLevel qualityLevel;
    IFF_Bool result = TRUE;
    IFF_Chunk *obsoleteChunk = IFF_removeChunkFromListByIndex(list, 1);

    if(obsoleteChunk != (IFF_Chunk*)newForm)
    {
        fprintf(stderr, "The removed chunk should be the 'NEW ' chunk!\n");
        result = FALSE;
    }

    if(list->chunksLength != 2)
    {
        fprintf(stderr, "The LIST should contain 2 sub chunks, but it has: %d\n", list->chunksLength);
        result = FALSE;
    }

    IFF_free((IFF_Chunk*)obsoleteChunk);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)list);

    if(qualityLevel != IFF_QUALITY_PERFECT)
        result = FALSE;

    return result;
}

static IFF_Bool addPropAndCheck(IFF_List *list)
{
    IFF_QualityLevel qualityLevel;
    IFF_Prop *prop = IFF_createEmptyProp(ID_NEW);
    IFF_addChunkToList(list, (IFF_Chunk*)prop);

    if(list->propsLength != 2)
    {
        fprintf(stderr, "Props length of the LIST should be 2, instead it is: %d\n", list->propsLength);
        return FALSE;
    }

    if(list->props[1] != prop)
    {
        fprintf(stderr, "The last form should be a PROP with formType: 'NEW '\n");
        return FALSE;
    }

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)list);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Bool updatePropAndCheck(IFF_List *list)
{
    IFF_QualityLevel qualityLevel;
    IFF_Prop *newProp = IFF_createEmptyProp(ID_NEW);
    IFF_Prop *previousMiddleProp = list->props[1];
    IFF_Prop *obsoleteProp = IFF_updatePropInListByIndex(list, 1, newProp);

    if(obsoleteProp != previousMiddleProp)
    {
         fprintf(stderr, "The obsolete PROP is not the previous middle PROP!\n");
         return FALSE;
    }

    if(list->props[1] != newProp)
    {
        fprintf(stderr, "The middle PROP should be a PROP with formType 'NEW '!\n");
        return FALSE;
    }

    IFF_free((IFF_Chunk*)obsoleteProp);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)list);

    return (qualityLevel == IFF_QUALITY_PERFECT);
}

static IFF_Bool removeFirstPropAndCheck(IFF_List *list)
{
    IFF_QualityLevel qualityLevel;
    IFF_Bool result = TRUE;
    IFF_Prop *firstProp = list->props[0];
    IFF_Prop *obsoleteProp = IFF_removePropFromListByIndex(list, 0);

    if(obsoleteProp != firstProp)
    {
        fprintf(stderr, "The removed chunk should be a PROP with formType: 'TEST'\n");
        result = FALSE;
    }

    if(list->propsLength != 1)
    {
        fprintf(stderr, "The LIST should contain 1 PROP chunks, but it has: %d\n", list->propsLength);
        result = FALSE;
    }

    IFF_free((IFF_Chunk*)obsoleteProp);

    /* Check if the quality is still perfect, because the parents' chunk sizes should all have been updated */
    qualityLevel = IFF_check((IFF_Chunk*)list);

    if(qualityLevel != IFF_QUALITY_PERFECT)
        result = FALSE;

    return result;
}

int main(int argc, char *argv[])
{
    IFF_Form *newForm;
    int status = 0;

    /* Create existing test LIST structure */
    IFF_List *list = IFF_createTestList();

    /* Execute CRUD operations and check */
    if(addFormAndCheck(list) &&
        (newForm = updateCATAndCheck(list)) != NULL &&
        removeForm(list, newForm) &&
        addPropAndCheck(list) &&
        updatePropAndCheck(list) &&
        removeFirstPropAndCheck(list))
        status = 0;
    else
        status = 1;

    /* Cleanup and return */
    IFF_free((IFF_Chunk*)list);
    return status;
}
