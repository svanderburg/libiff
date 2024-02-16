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

#include "cat.h"
#include "id.h"
#include "form.h"
#include "list.h"
#include "error.h"

#define CAT_GROUPTYPENAME "contentsType"

IFF_CAT *IFF_createCAT(const IFF_Long chunkSize, const IFF_ID contentsType)
{
    return (IFF_CAT*)IFF_createGroup(IFF_ID_CAT, chunkSize, contentsType);
}

IFF_CAT *IFF_createEmptyCAT(const IFF_ID contentsType)
{
    return (IFF_CAT*)IFF_createEmptyGroup(IFF_ID_CAT, contentsType);
}

void IFF_addToCAT(IFF_CAT *cat, IFF_Chunk *chunk)
{
    IFF_addToGroup((IFF_Group*)cat, chunk);
}

IFF_CAT *IFF_readCAT(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return (IFF_CAT*)IFF_readGroup(file, IFF_ID_CAT, chunkSize, CAT_GROUPTYPENAME, FALSE, extension, extensionLength);
}

IFF_Bool IFF_writeCAT(FILE *file, const IFF_CAT *cat, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_writeGroup(file, (IFF_Group*)cat, 0, CAT_GROUPTYPENAME, extension, extensionLength);
}

IFF_Bool IFF_checkCATSubChunk(const IFF_Group *group, const IFF_Chunk *subChunk)
{
    IFF_CAT *cat = (IFF_CAT*)group;

    /* A concatenation chunk may only contain other group chunks (except a PROP) */

    if(subChunk->chunkId != IFF_ID_FORM &&
       subChunk->chunkId != IFF_ID_LIST &&
       subChunk->chunkId != IFF_ID_CAT)
    {
        IFF_error("ERROR: Element with chunk Id: '");
        IFF_errorId(subChunk->chunkId);
        IFF_error("' not allowed in CAT chunk!\n");
        return FALSE;
    }

    if(cat->contentsType != IFF_ID_JJJJ)
    {
        /* Check whether a group type matches the contents type of the CAT */

        if(subChunk->chunkId == IFF_ID_FORM || subChunk->chunkId == IFF_ID_LIST || subChunk->chunkId == IFF_ID_CAT)
        {
            IFF_Group *group = (IFF_Group*)subChunk;

            if(group->groupType != cat->contentsType)
            {
                IFF_error("Sub form does not match contentsType of the CAT!\n");
                return FALSE;
            }
        }
    }

    return TRUE;
}

IFF_Bool IFF_checkCAT(const IFF_CAT *cat, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_checkGroup((IFF_Group*)cat, &IFF_checkId, &IFF_checkCATSubChunk, 0, extension, extensionLength);
}

void IFF_freeCAT(IFF_CAT *cat, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_freeGroup((IFF_Group*)cat, 0, extension, extensionLength);
}

void IFF_printCAT(const IFF_CAT *cat, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printGroup((const IFF_Group*)cat, indentLevel, 0, CAT_GROUPTYPENAME, extension, extensionLength);
}

IFF_Bool IFF_compareCAT(const IFF_CAT *cat1, const IFF_CAT *cat2, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_compareGroup((const IFF_Group*)cat1, (const IFF_Group*)cat2, 0, extension, extensionLength);
}

IFF_Form **IFF_searchFormsInCAT(IFF_CAT *cat, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    return IFF_searchFormsInGroup((IFF_Group*)cat, formTypes, formTypesLength, formsLength);
}

void IFF_updateCATChunkSizes(IFF_CAT *cat)
{
    IFF_updateGroupChunkSizes((IFF_Group*)cat);
}
