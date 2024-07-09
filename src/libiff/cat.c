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

IFF_ChunkInterface IFF_catInterface = {&IFF_createUnparsedGroup, &IFF_readCATContents, &IFF_writeCATContents, &IFF_checkCATContents, &IFF_clearGroupContents, &IFF_printCATContents, &IFF_compareGroupContents, &IFF_traverseGroupChunkHierarchy, &IFF_recalculateGroupChunkSize};

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

static IFF_Bool checkValidCATSubChunkId(IFF_ID chunkId)
{
    return chunkId == IFF_ID_FORM || chunkId == IFF_ID_CAT || chunkId == IFF_ID_LIST;
}

static void updateContentsType(IFF_CAT *cat, IFF_Chunk *chunk)
{
    if(checkValidCATSubChunkId(chunk->chunkId))
    {
        IFF_Group *group = (IFF_Group*)chunk;

        if(cat->chunksLength == 0)
            cat->contentsType = group->groupType;
        else if(cat->contentsType != IFF_ID_JJJJ && cat->contentsType != group->groupType)
            cat->contentsType = IFF_ID_JJJJ;
    }
}

void IFF_addChunkToCAT(IFF_CAT *cat, IFF_Chunk *chunk)
{
    IFF_addChunkToGroup((IFF_Group*)cat, chunk);
}

void IFF_addChunkToCATAndUpdateContentsType(IFF_CAT *cat, IFF_Chunk *chunk)
{
    updateContentsType(cat, chunk);
    IFF_addChunkToCAT(cat, chunk);
}

IFF_Chunk *IFF_removeChunkFromCAT(IFF_CAT *cat, unsigned int index)
{
    return IFF_removeChunkFromGroup((IFF_Group*)cat, index);
}

IFF_Chunk *IFF_updateChunkInCATAndUpdateContentsType(IFF_CAT *cat, unsigned int index, IFF_Chunk *chunk)
{
    IFF_Chunk *obsoleteChunk = IFF_updateChunkInGroup((IFF_Group*)cat, index, chunk);
    updateContentsType(cat, chunk);
    return obsoleteChunk;
}

IFF_Chunk *IFF_updateChunkInCAT(IFF_CAT *cat, unsigned int index, IFF_Chunk *chunk)
{
    return IFF_updateChunkInGroup((IFF_Group*)cat, index, chunk);
}

IFF_Bool IFF_readCATContents(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_readGroupContents(file, chunk, CAT_GROUPTYPENAME, chunkRegistry, attributePath, bytesProcessed, error);
}

IFF_Bool IFF_writeCATContents(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_writeGroupContents(file, chunk, CAT_GROUPTYPENAME, chunkRegistry, attributePath, bytesProcessed, error);
}

static IFF_QualityLevel checkSubChunkGroupTypeMatchesContentType(const IFF_CAT *cat, const IFF_Chunk *subChunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    if(cat->contentsType == IFF_ID_JJJJ)
        return IFF_QUALITY_PERFECT;
    else
    {
        /* Check whether a group type matches the contents type of the CAT */

        const IFF_Group *group = (const IFF_Group*)subChunk;

        if(group->groupType == cat->contentsType)
            return IFF_QUALITY_PERFECT;
        else
        {
            IFF_ID2 groupType, contentsType;
            IFF_idToString(group->groupType, groupType);
            IFF_idToString(cat->contentsType, contentsType);

            printCheckMessage(attributePath, NULL, cat->chunkId, data, "is a group sub chunk with groupType: \"%.4s\" that does not match the parent's contentsType: \"%.4s\"", groupType, contentsType);
            return IFF_QUALITY_OK;
        }
    }
}

IFF_QualityLevel IFF_checkCATSubChunk(const IFF_Group *group, const IFF_Chunk *subChunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    const IFF_CAT *cat = (const IFF_CAT*)group;

    /* A concatenation chunk may only contain other group chunks (except a PROP) */

    if(checkValidCATSubChunkId(subChunk->chunkId))
        return checkSubChunkGroupTypeMatchesContentType(cat, subChunk, attributePath, printCheckMessage, data);
    else
    {
        IFF_ID2 subChunkId;
        IFF_idToString(subChunk->chunkId, subChunkId);
        printCheckMessage(attributePath, NULL, cat->chunkId, data, "is a sub chunk with chunkId: \"%.4s\" that is not allowed", subChunkId);
        return IFF_QUALITY_OK;
    }
}

IFF_QualityLevel IFF_checkCATContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_checkGroupContents((const IFF_Group*)chunk, CAT_GROUPTYPENAME, &IFF_checkId, &IFF_checkCATSubChunk, chunkRegistry, attributePath, printCheckMessage, data);
}

void IFF_printCATContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroupContents(file, (const IFF_Group*)chunk, indentLevel, CAT_GROUPTYPENAME, chunkRegistry);
}
