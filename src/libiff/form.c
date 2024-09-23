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
#include "io.h"
#include "iff.h"
#include "pointerarray.h"
#include "formregistry.h"

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

IFF_ChunkInterface IFF_formInterface = {&IFF_parseFormContents, &IFF_writeFormContents, &IFF_checkFormContents, &IFF_clearFormContents, &IFF_printFormContents, &IFF_compareFormContents, &IFF_traverseFormChunkHierarchy, &IFF_recalculateFormChunkSize};

IFF_Form *IFF_createForm(const IFF_Long chunkSize, const IFF_ID formType, IFF_GroupStructure *formStructure)
{
    return (IFF_Form*)IFF_createGroup(IFF_ID_FORM, chunkSize, formType, formStructure, &IFF_formInterface);
}

IFF_Form *IFF_createEmptyForm(const IFF_ID formType, IFF_GroupStructure *formStructure)
{
    return (IFF_Form*)IFF_createEmptyGroup(IFF_ID_FORM, formType, formStructure, &IFF_formInterface);
}

void IFF_addChunkToForm(IFF_Form *form, IFF_Chunk *chunk)
{
    IFF_addChunkToGroup((IFF_Group*)form, chunk);
}

IFF_Chunk *IFF_updateChunkInFormByIndex(IFF_Form *form, const unsigned int index, IFF_Chunk *chunk)
{
    return IFF_updateChunkInGroupByIndex((IFF_Group*)form, index, chunk);
}

IFF_Chunk *IFF_removeChunkFromFormByIndex(IFF_Form *form, const unsigned int index)
{
    return IFF_removeChunkFromGroupByIndex((IFF_Group*)form, index);
}

IFF_Chunk *IFF_parseFormContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return (IFF_Chunk*)IFF_parseGroupContents(file, IFF_findGroupStructure, chunkId, chunkSize, FORM_GROUPTYPENAME, registry, chunkInterface, attributePath, bytesProcessed, error);
}

IFF_Bool IFF_writeFormContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_writeGroupContents(file, (const IFF_Group*)chunk, FORM_GROUPTYPENAME, attributePath, bytesProcessed, error);
}

static IFF_QualityLevel checkValidFormChars(const IFF_ID formType, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId)
{
    IFF_QualityLevel currentLevel = IFF_QUALITY_PERFECT;
    IFF_ID2 formType2;
    unsigned int i;

    IFF_idToString(formType, formType2);

    /* A form type is not allowed to have lowercase or puntuaction marks */
    for(i = 0; i < IFF_ID_SIZE; i++)
    {
        if((formType2[i] >= 0x61 && formType2[i] <= 0x7a) || formType2[i] == '.')
        {
            printCheckMessage(attributePath, attributeName, chunkId, data, "contains lowercase characters or punctuation marks: \"%.4s\"", formType2);
            currentLevel = IFF_degradeQualityLevel(currentLevel, IFF_QUALITY_OK);
            break;
        }
    }

    return currentLevel;
}

static IFF_QualityLevel checkNonGroupChunkId(const IFF_ID formType, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId)
{
    IFF_ID2 formType2;
    IFF_idToString(formType, formType2);

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
        printCheckMessage(attributePath, attributeName, chunkId, data, "contains an identifier that is not allowed: \"%.4s\"", formType2);
        return IFF_QUALITY_OK;
    }
    else
        return IFF_QUALITY_PERFECT;
}

IFF_QualityLevel IFF_checkFormType(const IFF_ID formType, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId)
{
    IFF_QualityLevel currentLevel = IFF_QUALITY_PERFECT;

    currentLevel = IFF_degradeQualityLevel(currentLevel, IFF_checkId(formType, attributePath, attributeName, printCheckMessage, data, chunkId));
    currentLevel = IFF_degradeQualityLevel(currentLevel, checkValidFormChars(formType, attributePath, attributeName, printCheckMessage, data, chunkId));
    currentLevel = IFF_degradeQualityLevel(currentLevel, checkNonGroupChunkId(formType, attributePath, attributeName, printCheckMessage, data, chunkId));

    return currentLevel;
}

static IFF_QualityLevel subChunkCheck(const IFF_Group *group, const IFF_Chunk *subChunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    if(subChunk->chunkId == IFF_ID_PROP)
    {
        IFF_ID2 subChunkId;
        IFF_idToString(subChunk->chunkId, subChunkId);
        printCheckMessage(attributePath, NULL, group->chunkId, data, "is a sub chunk with chunkId: \"%.4s\" that is not allowed", subChunkId);
        return IFF_QUALITY_OK;
    }
    else
        return IFF_QUALITY_PERFECT;
}

IFF_QualityLevel IFF_checkFormContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_checkGroupContents((const IFF_Group*)chunk, FORM_GROUPTYPENAME, &IFF_checkFormType, &subChunkCheck, attributePath, printCheckMessage, data);
}

void IFF_clearFormContents(IFF_Chunk *chunk)
{
    IFF_clearGroupContents((IFF_Group*)chunk);
}

void IFF_printFormContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printGroupContents(file, (const IFF_Group*)chunk, indentLevel, FORM_GROUPTYPENAME);
}

IFF_Bool IFF_compareFormContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareGroupContents((const IFF_Group*)chunk1, (const IFF_Group*)chunk2);
}

IFF_Bool IFF_traverseFormChunkHierarchy(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk)
{
    return IFF_traverseGroupChunkHierarchy((IFF_Group*)chunk, data, visitChunk);
}

void IFF_recalculateFormChunkSize(IFF_Chunk *chunk)
{
    IFF_recalculateGroupChunkSize((IFF_Group*)chunk);
}

typedef struct
{
    const IFF_ID *formTypes;
    unsigned int formTypesLength;
    IFF_Form **forms;
    unsigned int formsLength;
}
SearchFormsResult;

static IFF_Bool visitFormChunkFunction(IFF_Chunk *chunk, void *data)
{
    if(chunk->chunkId == IFF_ID_FORM)
    {
        SearchFormsResult *result = (SearchFormsResult*)data;
        IFF_Form *form = (IFF_Form*)chunk;
        unsigned int i;

        for(i = 0; i < result->formTypesLength; i++)
        {
            const IFF_ID formType = result->formTypes[i];

            if(form->formType == formType)
            {
                result->forms = (IFF_Form**)IFF_addElementToPointerArray((void**)result->forms, (void*)form, &result->formsLength);
                break;
            }
        }
    }

    return TRUE;
}

IFF_Form **IFF_searchFormsFromArray(IFF_Chunk *chunk, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    SearchFormsResult result;
    result.formTypes = formTypes;
    result.formTypesLength = formTypesLength;
    result.forms = NULL;
    result.formsLength = 0;

    IFF_traverse(chunk, &result, visitFormChunkFunction);

    *formsLength = result.formsLength;
    return result.forms;
}

IFF_Form **IFF_searchForms(IFF_Chunk *chunk, const IFF_ID formType, unsigned int *formsLength)
{
    IFF_ID formTypes[1];
    formTypes[0] = formType;
    return IFF_searchFormsFromArray(chunk, formTypes, 1, formsLength);
}
