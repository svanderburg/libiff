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

#include "extension.h"
#include <stdlib.h>
#include "id.h"

static int compareExtension(const void *a, const void *b)
{
    const IFF_Extension *l = (IFF_Extension*)a;
    const IFF_Extension *r = (IFF_Extension*)b;
    
    return IFF_compareId(l->formType, r->formType);
}

const static IFF_FormExtension *getFormExtensions(const char *formType, const IFF_Extension *extension, const unsigned int extensionLength, unsigned int *formExtensionsLength)
{
    IFF_Extension key;
    IFF_Extension *result;
    
    key.formType = formType;
    
    result = bsearch(&key, extension, extensionLength, sizeof(IFF_Extension), &compareExtension);

    if(result == NULL)
    {
	*formExtensionsLength = 0;
	return NULL;
    }
    else
    {
	*formExtensionsLength = result->formExtensionsLength;
	return result->formExtensions;
    }
}

static int compareFormExtension(const void *a, const void *b)
{
    const IFF_FormExtension *l = (IFF_FormExtension*)a;
    const IFF_FormExtension *r = (IFF_FormExtension*)b;
    
    return IFF_compareId(l->chunkId, r->chunkId);
}

const static IFF_FormExtension *getFormExtension(const char *chunkId, const IFF_FormExtension *formExtension, const unsigned int formExtensionLength)
{
    IFF_FormExtension key;
    key.chunkId = chunkId;
    
    return bsearch(&key, formExtension, formExtensionLength, sizeof(IFF_FormExtension), &compareFormExtension);
}

const IFF_FormExtension *IFF_findFormExtension(const char *formType, const char *chunkId, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(formType == NULL)
	return NULL;
    else
    {
	unsigned int formExtensionsLength;
	
	/* Search the given form extensions array */
	const IFF_FormExtension *formExtensions = getFormExtensions(formType, extension, extensionLength, &formExtensionsLength);
	
	/* Search for the extension that handles the a chunk with the given chunk id */
	const IFF_FormExtension *formExtension = getFormExtension(chunkId, formExtensions, formExtensionsLength);
    
	/* Return the form extension we have found */
	return formExtension;
    }
}
