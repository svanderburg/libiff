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

IFF_CAT *IFF_createEmptyCATWithContentsType(const IFF_ID contentsType)
{
    return (IFF_CAT*)IFF_createEmptyGroup(IFF_ID_CAT, contentsType);
}

IFF_CAT *IFF_createEmptyCAT(void)
{
    return (IFF_CAT*)IFF_createEmptyCATWithContentsType(IFF_ID_JJJJ);
}

IFF_Chunk *IFF_createUnparsedCAT(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    return IFF_createUnparsedGroup(chunkId, chunkSize);
}

static IFF_Bool checkValidCATSubChunkId(IFF_ID chunkId)
{
    return chunkId == IFF_ID_FORM || chunkId == IFF_ID_CAT || chunkId == IFF_ID_LIST;
}

static void updateContentsType(IFF_CAT *cat, IFF_Chunk *chunk)
{
    if(checkValidCATSubChunkId(chunk->chunkId))
    {
        IFF_Group *group = (IFF_Group*)chunk;

        if(cat->chunkLength == 0)
            cat->contentsType = group->groupType;
        else if(cat->contentsType != IFF_ID_JJJJ && cat->contentsType != group->groupType)
            cat->contentsType = IFF_ID_JJJJ;
    }
}

void IFF_addToCAT(IFF_CAT *cat, IFF_Chunk *chunk)
{
    IFF_addToGroup((IFF_Group*)cat, chunk);
}

void IFF_addToCATAndUpdateContentsType(IFF_CAT *cat, IFF_Chunk *chunk)
{
    updateContentsType(cat, chunk);
    IFF_addToCAT(cat, chunk);
}

IFF_Bool IFF_readCAT(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    return IFF_readGroup(file, chunk, CAT_GROUPTYPENAME, chunkRegistry, bytesProcessed);
}

IFF_Bool IFF_writeCAT(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    return IFF_writeGroup(file, chunk, CAT_GROUPTYPENAME, chunkRegistry, bytesProcessed);
}

IFF_Bool IFF_checkCATSubChunk(const IFF_Group *group, const IFF_Chunk *subChunk)
{
    IFF_CAT *cat = (IFF_CAT*)group;

    /* A concatenation chunk may only contain other group chunks (except a PROP) */

    if(!checkValidCATSubChunkId(subChunk->chunkId))
    {
        IFF_error("ERROR: Element with chunk Id: '");
        IFF_errorId(subChunk->chunkId);
        IFF_error("' not allowed in CAT chunk!\n");
        return FALSE;
    }

    if(cat->contentsType != IFF_ID_JJJJ)
    {
        /* Check whether a group type matches the contents type of the CAT */

        if(checkValidCATSubChunkId(subChunk->chunkId))
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

IFF_Bool IFF_checkCAT(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_checkGroup((IFF_Group*)chunk, &IFF_checkId, &IFF_checkCATSubChunk, chunkRegistry);
}

void IFF_freeCAT(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_freeGroup((IFF_Group*)chunk, chunkRegistry);
}

void IFF_printCAT(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroup((const IFF_Group*)chunk, indentLevel, CAT_GROUPTYPENAME, chunkRegistry);
}

IFF_Bool IFF_compareCAT(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_compareGroup((const IFF_Group*)chunk1, (const IFF_Group*)chunk2, chunkRegistry);
}

IFF_Form **IFF_searchFormsInCAT(IFF_CAT *cat, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    return IFF_searchFormsInGroup((IFF_Group*)cat, formTypes, formTypesLength, formsLength);
}

void IFF_updateCATChunkSizes(IFF_CAT *cat)
{
    IFF_updateGroupChunkSizes((IFF_Group*)cat);
}
