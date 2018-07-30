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
#include "error.h"

#define FORM_CHUNKID "FORM"
#define FORM_GROUPTYPENAME "formType"

IFF_Form *IFF_createForm(const char *formType)
{
    return (IFF_Form*)IFF_createGroup(FORM_CHUNKID, formType);
}

void IFF_addToForm(IFF_Form *form, IFF_Chunk *chunk)
{
    IFF_addToGroup((IFF_Group*)form, chunk);
}

IFF_Form *IFF_readForm(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return (IFF_Form*)IFF_readGroup(file, FORM_CHUNKID, chunkSize, FORM_GROUPTYPENAME, TRUE, extension, extensionLength);
}

int IFF_writeForm(FILE *file, const IFF_Form *form, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_writeGroup(file, (IFF_Group*)form, form->formType, FORM_GROUPTYPENAME, extension, extensionLength);
}

int IFF_checkFormType(const IFF_ID formType)
{
    unsigned int i;
    
    /* A form type must be a valid ID */
    if(!IFF_checkId(formType))
	return FALSE;

    /* A form type is not allowed to have lowercase or puntuaction marks */
    for(i = 0; i < IFF_ID_SIZE; i++)
    {
	if((formType[i] >= 0x61 && formType[i] <= 0x7a) || formType[i] == '.')
	{
	    IFF_error("No lowercase characters or punctuation marks allowed in a form type ID!\n");
	    return FALSE;
	}
    }
    
    /* A form ID is not allowed to be equal to a group chunk ID */
    
    if(IFF_compareId(formType, "LIST") == 0 ||
       IFF_compareId(formType, "FORM") == 0 ||
       IFF_compareId(formType, "PROP") == 0 ||
       IFF_compareId(formType, "CAT ") == 0 ||
       IFF_compareId(formType, "JJJJ") == 0 ||
       IFF_compareId(formType, "LIS1") == 0 ||
       IFF_compareId(formType, "LIS2") == 0 ||
       IFF_compareId(formType, "LIS3") == 0 ||
       IFF_compareId(formType, "LIS4") == 0 ||
       IFF_compareId(formType, "LIS5") == 0 ||
       IFF_compareId(formType, "LIS6") == 0 ||
       IFF_compareId(formType, "LIS7") == 0 ||
       IFF_compareId(formType, "LIS8") == 0 ||
       IFF_compareId(formType, "LIS9") == 0 ||
       IFF_compareId(formType, "FOR1") == 0 ||
       IFF_compareId(formType, "FOR1") == 0 ||
       IFF_compareId(formType, "FOR2") == 0 ||
       IFF_compareId(formType, "FOR3") == 0 ||
       IFF_compareId(formType, "FOR4") == 0 ||
       IFF_compareId(formType, "FOR5") == 0 ||
       IFF_compareId(formType, "FOR6") == 0 ||
       IFF_compareId(formType, "FOR7") == 0 ||
       IFF_compareId(formType, "FOR8") == 0 ||
       IFF_compareId(formType, "FOR9") == 0 ||
       IFF_compareId(formType, "CAT1") == 0 ||
       IFF_compareId(formType, "CAT2") == 0 ||
       IFF_compareId(formType, "CAT3") == 0 ||
       IFF_compareId(formType, "CAT4") == 0 ||
       IFF_compareId(formType, "CAT5") == 0 ||
       IFF_compareId(formType, "CAT6") == 0 ||
       IFF_compareId(formType, "CAT7") == 0 ||
       IFF_compareId(formType, "CAT8") == 0 ||
       IFF_compareId(formType, "CAT9") == 0)
    {
	IFF_error("Form type: '");
	IFF_errorId(formType);
	IFF_error("' not allowed!\n");
	
	return FALSE;
    }
    
    return TRUE;
}

static int subChunkCheck(const IFF_Group *group, const IFF_Chunk *subChunk)
{
    if(IFF_compareId(subChunk->chunkId, "PROP") == 0)
    {
        IFF_error("ERROR: Element with chunk Id: '");
        IFF_errorId(subChunk->chunkId);
        IFF_error("' not allowed in FORM chunk!\n");
	
        return FALSE;
    }
    else
	return TRUE;
}

int IFF_checkForm(const IFF_Form *form, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_checkGroup((IFF_Group*)form, &IFF_checkFormType, &subChunkCheck, form->formType, extension, extensionLength);
}

void IFF_freeForm(IFF_Form *form, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_freeGroup((IFF_Group*)form, form->formType, extension, extensionLength);
}

void IFF_printForm(const IFF_Form *form, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printGroup((const IFF_Group*)form, indentLevel, form->formType, FORM_GROUPTYPENAME, extension, extensionLength);
}

int IFF_compareForm(const IFF_Form *form1, const IFF_Form *form2, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_compareGroup((const IFF_Group*)form1, (const IFF_Group*)form2, form1->formType, extension, extensionLength);
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

IFF_Form **IFF_searchFormsInForm(IFF_Form *form, const char **formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    unsigned int i;

    /* If the given form is what we look for, return it */
    for(i = 0; i < formTypesLength; i++)
    {
        const char *formType = formTypes[i];

        if(IFF_compareId(form->formType, formType) == 0)
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
	if(IFF_compareId(parent->chunkId, "LIST") == 0)
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
static IFF_Chunk *searchProperty(const IFF_Chunk *chunk, const char *formType, const char *chunkId)
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

IFF_Chunk *IFF_getDataChunkFromForm(const IFF_Form *form, const char *chunkId)
{
    unsigned int i;
    
    for(i = 0; i < form->chunkLength; i++)
    {
	if(IFF_compareId(form->chunk[i]->chunkId, chunkId) == 0)
	    return form->chunk[i];
    }
    
    return NULL;
}

IFF_Chunk *IFF_getChunkFromForm(const IFF_Form *form, const char *chunkId)
{
    /* Retrieve the chunk with the given id from the given form */
    IFF_Chunk *chunk = IFF_getDataChunkFromForm(form, chunkId);
    
    /* If the chunk is not in the form, try to find it in a higher located PROP */
    if(chunk == NULL)
	return searchProperty((IFF_Chunk*)form, form->formType, chunkId);
    else
	return chunk;
}

IFF_Chunk **IFF_getChunksFromForm(const IFF_Form *form, const char *chunkId, unsigned int *chunksLength)
{
    IFF_Chunk **result = NULL;
    unsigned int i;
    
    *chunksLength = 0;
    
    for(i = 0; i < form->chunkLength; i++)
    {
	if(IFF_compareId(form->chunk[i]->chunkId, chunkId) == 0)
	{
	    result = (IFF_Chunk**)realloc(result, (*chunksLength + 1) * sizeof(IFF_Chunk*));
	    result[*chunksLength] = form->chunk[i];
	    *chunksLength = *chunksLength + 1;
	}
    }
    
    return result;
}
