/**
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

#include "util.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "cat.h"
#include "id.h"
#include "error.h"

void IFF_printIndent(FILE *file, const unsigned int indentLevel, const char *formatString, ...)
{
    va_list ap;
    unsigned int i;
    
    va_start(ap, formatString);
    
    for(i = 0; i < indentLevel; i++)
      fprintf(file, "  ");
    
    vfprintf(file, formatString, ap);
}

static IFF_Form **appendToFormArray(IFF_Form **target, unsigned int *targetLength, IFF_Form **source, const unsigned int sourceLength)
{
    unsigned int i;
    unsigned int newLength = *targetLength + sourceLength;
    
    target = (IFF_Form**)realloc(target, newLength * sizeof(IFF_Form*));
    
    for(i = 0; i < sourceLength; i++)
	target[i + *targetLength] = source[i];
    
    *targetLength = newLength;
    
    return target;
}

IFF_Form **IFF_searchForms(IFF_Chunk *chunk, const char *formType, unsigned int *formsLength)
{
    IFF_Form **forms = NULL;
    *formsLength = 0;

    if(IFF_compareId(chunk->chunkId, "FORM") == 0)
    {
	IFF_Form *form = (IFF_Form*)chunk;
	
	if(IFF_compareId(form->formType, formType) == 0)
	{
	    forms = (IFF_Form**)realloc(forms, (*formsLength + 1) * sizeof(IFF_Form*));
	    forms[*formsLength] = form;
	    *formsLength = *formsLength + 1;
	}
	else
	{
	    unsigned int i;
	    
	    for(i = 0; i < form->chunkLength; i++)
	    {
		unsigned int resultLength;
		IFF_Form **result = IFF_searchForms(form->chunk[i], formType, &resultLength);
	    
		forms = appendToFormArray(forms, formsLength, result, resultLength);
	    }
	}
    }
    else if(IFF_compareId(chunk->chunkId, "LIST") == 0)
    {
	unsigned int i;
	IFF_List *list = (IFF_List*)chunk;
	
	for(i = 0; i < list->chunkLength; i++)
	{
	    unsigned int resultLength;
	    IFF_Form **result = IFF_searchForms(list->chunk[i], formType, &resultLength);
	    
	    forms = appendToFormArray(forms, formsLength, result, resultLength);
	}
    }
    else if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
    {
	unsigned int i;
	IFF_CAT *cat = (IFF_CAT*)chunk;
	
	for(i = 0; i < cat->chunkLength; i++)
	{
	    unsigned int resultLength;
	    IFF_Form **result = IFF_searchForms(cat->chunk[i], formType, &resultLength);
	    
	    forms = appendToFormArray(forms, formsLength, result, resultLength);
	}
    }
    
    return forms;
}
