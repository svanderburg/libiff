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

#include "formregistry.h"
#include <stdlib.h>

static int compareFormTypes(const void *a, const void *b)
{
    const IFF_FormType *l = (const IFF_FormType*)a;
    const IFF_FormType *r = (const IFF_FormType*)b;

    if(l->formType < r->formType)
        return -1;
    else if(l->formType > r->formType)
        return 1;
    else
        return 0;
}

static IFF_FormType *getFormType(const IFF_ID formType, const IFF_FormTypesNode *formTypesNode)
{
    if(formTypesNode == NULL)
        return NULL;
    else
    {
        IFF_FormType *result;
        IFF_FormType key;
        key.formType = formType;

        result = (IFF_FormType*)bsearch(&key, formTypesNode->formTypes, formTypesNode->formTypesLength, sizeof(IFF_FormType), &compareFormTypes);

        if(result == NULL)
            return getFormType(formType, formTypesNode->parent);
        else
            return result;
    }
}

IFF_GroupStructure *IFF_findGroupStructure(const IFF_ChunkRegistry *chunkRegistry, const IFF_ID formType)
{
    IFF_FormTypesNode *formTypesNode = (IFF_FormTypesNode*)chunkRegistry->extension;
    IFF_FormType *result = getFormType(formType, formTypesNode);

    if(result == NULL)
        return NULL;
    else
        return result->formStructure;
}
