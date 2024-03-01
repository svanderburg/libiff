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

#include "form.h"
#include <string.h>
#include <stdlib.h>
#include "id.h"
#include "util.h"
#include "list.h"
#include "cat.h"
#include "error.h"

#define FORM_GROUPTYPENAME "formType"

#define ID_FOR1 IFF_MAKEID('F', 'O', 'R', '1')
#define ID_FOR2 IFF_MAKEID('F', 'O', 'R', '2')
#define ID_FOR3 IFF_MAKEID('F', 'O', 'R', '3')
#define ID_FOR4 IFF_MAKEID('F', 'O', 'R', '4')
#define ID_FOR5 IFF_MAKEID('F', 'O', 'R', '5')
#define ID_FOR6 IFF_MAKEID('F', 'O', 'R', '6')
#define ID_FOR7 IFF_MAKEID('F', 'O', 'R', '7')
#define ID_FOR8 IFF_MAKEID('F', 'O', 'R', '8')
#define ID_FOR9 IFF_MAKEID('F', 'O', 'R', '9')

#define ID_CAT1 IFF_MAKEID('C', 'A', 'T', '1')
#define ID_CAT2 IFF_MAKEID('C', 'A', 'T', '2')
#define ID_CAT3 IFF_MAKEID('C', 'A', 'T', '3')
#define ID_CAT4 IFF_MAKEID('C', 'A', 'T', '4')
#define ID_CAT5 IFF_MAKEID('C', 'A', 'T', '5')
#define ID_CAT6 IFF_MAKEID('C', 'A', 'T', '6')
#define ID_CAT7 IFF_MAKEID('C', 'A', 'T', '7')
#define ID_CAT8 IFF_MAKEID('C', 'A', 'T', '8')
#define ID_CAT9 IFF_MAKEID('C', 'A', 'T', '9')

#define ID_LIS1 IFF_MAKEID('L', 'I', 'S', '1')
#define ID_LIS2 IFF_MAKEID('L', 'I', 'S', '2')
#define ID_LIS3 IFF_MAKEID('L', 'I', 'S', '3')
#define ID_LIS4 IFF_MAKEID('L', 'I', 'S', '4')
#define ID_LIS5 IFF_MAKEID('L', 'I', 'S', '5')
#define ID_LIS6 IFF_MAKEID('L', 'I', 'S', '6')
#define ID_LIS7 IFF_MAKEID('L', 'I', 'S', '7')
#define ID_LIS8 IFF_MAKEID('L', 'I', 'S', '8')
#define ID_LIS9 IFF_MAKEID('L', 'I', 'S', '9')

IFF_Form *IFF_createForm(const IFF_Long chunkSize, const IFF_ID formType)
{
    return (IFF_Form*)IFF_createGroup(IFF_ID_FORM, chunkSize, formType);
}

IFF_Form *IFF_createEmptyForm(const IFF_ID formType)
{
    return (IFF_Form*)IFF_createEmptyGroup(IFF_ID_FORM, formType);
}

IFF_Chunk *IFF_createUnparsedForm(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    return IFF_createUnparsedGroup(chunkId, chunkSize);
}

void IFF_addToForm(IFF_Form *form, IFF_Chunk *chunk)
{
    IFF_addToGroup((IFF_Group*)form, chunk);
}

IFF_Bool IFF_readForm(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    return IFF_readGroup(file, chunk, FORM_GROUPTYPENAME, chunkRegistry, bytesProcessed);
}

IFF_Bool IFF_writeForm(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    return IFF_writeGroup(file, chunk, FORM_GROUPTYPENAME, chunkRegistry, bytesProcessed);
}

IFF_Bool IFF_checkFormType(const IFF_ID formType)
{
    unsigned int i;
    IFF_ID2 formType2;

    /* A form type must be a valid ID */
    if(!IFF_checkId(formType))
        return FALSE;

    IFF_idToString(formType, formType2);

    /* A form type is not allowed to have lowercase or puntuaction marks */
    for(i = 0; i < IFF_ID_SIZE; i++)
    {
        if((formType2[i] >= 0x61 && formType2[i] <= 0x7a) || formType2[i] == '.')
        {
            IFF_error("No lowercase characters or punctuation marks allowed in a form type ID!\n");
            return FALSE;
        }
    }

    /* A form ID is not allowed to be equal to a group chunk ID */

    if(formType == IFF_ID_LIST ||
       formType == IFF_ID_FORM ||
       formType == IFF_ID_PROP ||
       formType == IFF_ID_CAT ||
       formType == IFF_ID_JJJJ ||
       formType == ID_LIS1 ||
       formType == ID_LIS2 ||
       formType == ID_LIS3 ||
       formType == ID_LIS4 ||
       formType == ID_LIS5 ||
       formType == ID_LIS6 ||
       formType == ID_LIS7 ||
       formType == ID_LIS8 ||
       formType == ID_LIS9 ||
       formType == ID_FOR1 ||
       formType == ID_FOR1 ||
       formType == ID_FOR2 ||
       formType == ID_FOR3 ||
       formType == ID_FOR4 ||
       formType == ID_FOR5 ||
       formType == ID_FOR6 ||
       formType == ID_FOR7 ||
       formType == ID_FOR8 ||
       formType == ID_FOR9 ||
       formType == ID_CAT1 ||
       formType == ID_CAT2 ||
       formType == ID_CAT3 ||
       formType == ID_CAT4 ||
       formType == ID_CAT5 ||
       formType == ID_CAT6 ||
       formType == ID_CAT7 ||
       formType == ID_CAT8 ||
       formType == ID_CAT9)
    {
        IFF_error("Form type: '");
        IFF_errorId(formType);
        IFF_error("' not allowed!\n");

        return FALSE;
    }

    return TRUE;
}

static IFF_Bool subChunkCheck(const IFF_Group *group, const IFF_Chunk *subChunk)
{
    if(subChunk->chunkId == IFF_ID_PROP)
    {
        IFF_error("ERROR: Element with chunk Id: '");
        IFF_errorId(subChunk->chunkId);
        IFF_error("' not allowed in FORM chunk!\n");

        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_checkForm(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_checkGroup((IFF_Group*)chunk, &IFF_checkFormType, &subChunkCheck, chunkRegistry);
}

void IFF_freeForm(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_freeGroup((IFF_Group*)chunk, chunkRegistry);
}

void IFF_printForm(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroup((const IFF_Group*)chunk, indentLevel, FORM_GROUPTYPENAME, chunkRegistry);
}

IFF_Bool IFF_compareForm(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_compareGroup((const IFF_Group*)chunk1, (const IFF_Group*)chunk2, chunkRegistry);
}

IFF_Form **IFF_mergeFormArray(IFF_Form **target, unsigned int *targetLength, IFF_Form **source, const unsigned int sourceLength)
{
    unsigned int i;
    unsigned int newLength = *targetLength + sourceLength;

    target = (IFF_Form**)realloc(target, newLength * sizeof(IFF_Form*));

    for(i = 0; i < sourceLength; i++)
        target[i + *targetLength] = source[i];

    *targetLength = newLength;

    return target;
}

IFF_Form **IFF_searchFormsInForm(IFF_Form *form, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    unsigned int i;

    /* If the given form is what we look for, return it */
    for(i = 0; i < formTypesLength; i++)
    {
        const IFF_ID formType = formTypes[i];

        if(form->formType == formType)
        {
            IFF_Form **forms = (IFF_Form**)malloc(sizeof(IFF_Form*));
            forms[0] = form;
            *formsLength = 1;

            return forms;
        }
    }

    return IFF_searchFormsInGroup((IFF_Group*)form, formTypes, formTypesLength, formsLength); /* Search into the nested forms in this form */
}

void IFF_updateFormChunkSizes(IFF_Form *form)
{
    IFF_updateGroupChunkSizes((IFF_Group*)form);
}

/**
 * Searches the list in which the given chunk is (indirectly) a member from.
 *
 * @param chunk An arbitrary chunk, which could be (indirectly) a member of a list
 * @return List instance of which the given chunk is (indirectly) a member, or NULL if the chunk is not a member of a list
 */
static IFF_List *searchList(const IFF_Chunk *chunk)
{
    IFF_Group *parent = chunk->parent;

    if(parent == NULL)
        return NULL;
    else
    {
        if(parent->chunkId == IFF_ID_LIST)
            return (IFF_List*)parent;
        else
            return searchList((IFF_Chunk*)parent);
    }
}

/**
 * Recursively searches for a shared list property with the given chunk ID.
 *
 * @param chunk An arbitrary chunk, which could be (indirectly) a member of a list
 * @param formType A 4 character form id
 * @param chunkId A 4 character chunk id
 * @return The chunk with the given chunk id, or NULL if the chunk can't be found
 */
static IFF_Chunk *searchProperty(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_ID chunkId)
{
    IFF_List *list = searchList(chunk);

    if(list == NULL)
        return NULL; /* If the chunk is not (indirectly) in a list, we have no shared properties at all */
    else
    {
        /* Try requesting the PROP chunk for the given form type */
        IFF_Prop *prop = IFF_getPropFromList(list, formType);

        if(prop == NULL)
            return searchProperty((IFF_Chunk*)list, formType, chunkId); /* If we can't find a shared property chunk with the given form type, try searching for a list higher in the hierarchy */
        else
        {
            /* Try requesting the chunk from the shared property chunk */
            IFF_Chunk *chunk = IFF_getChunkFromProp(prop, chunkId);

            if(chunk == NULL)
                return searchProperty((IFF_Chunk*)list, formType, chunkId); /* If the requested chunk is not in the PROP chunk, try searching for a list higher in the hierarchy */
            else
                return chunk; /* We have found the requested shared property chunk */
        }
    }
}

IFF_Chunk *IFF_getDataChunkFromForm(const IFF_Form *form, const IFF_ID chunkId)
{
    unsigned int i;

    for(i = 0; i < form->chunkLength; i++)
    {
        if(form->chunk[i]->chunkId == chunkId)
            return form->chunk[i];
    }

    return NULL;
}

IFF_Chunk *IFF_getChunkFromForm(const IFF_Form *form, const IFF_ID chunkId)
{
    /* Retrieve the chunk with the given id from the given form */
    IFF_Chunk *chunk = IFF_getDataChunkFromForm(form, chunkId);

    /* If the chunk is not in the form, try to find it in a higher located PROP */
    if(chunk == NULL)
        return searchProperty((IFF_Chunk*)form, form->formType, chunkId);
    else
        return chunk;
}

IFF_Chunk **IFF_getChunksFromForm(const IFF_Form *form, const IFF_ID chunkId, unsigned int *chunksLength)
{
    IFF_Chunk **result = NULL;
    unsigned int i;

    *chunksLength = 0;

    for(i = 0; i < form->chunkLength; i++)
    {
        if(form->chunk[i]->chunkId == chunkId)
        {
            result = (IFF_Chunk**)realloc(result, (*chunksLength + 1) * sizeof(IFF_Chunk*));
            result[*chunksLength] = form->chunk[i];
            *chunksLength = *chunksLength + 1;
        }
    }

    return result;
}
