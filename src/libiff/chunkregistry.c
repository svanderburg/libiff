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

#include "chunkregistry.h"
#include <stdlib.h>
#include "id.h"
#include "rawchunk.h"

static int compareFormChunkTypes(const void *a, const void *b)
{
    const IFF_FormChunkTypes *l = (IFF_FormChunkTypes*)a;
    const IFF_FormChunkTypes *r = (IFF_FormChunkTypes*)b;

    if(l->formType < r->formType)
        return -1;
    else if(l->formType > r->formType)
        return 1;
    else
        return 0;
}

const static IFF_FormChunkTypes *getFormChunkTypes(const IFF_ID formType, const IFF_ChunkRegistry *chunkRegistry)
{
    if(chunkRegistry == NULL)
        return NULL;
    else
    {
        IFF_FormChunkTypes key;
        key.formType = formType;

        return (IFF_FormChunkTypes*)bsearch(&key, chunkRegistry->formChunkTypes, chunkRegistry->formChunkTypesLength, sizeof(IFF_FormChunkTypes), &compareFormChunkTypes);
    }
}

static int compareChunkTypes(const void *a, const void *b)
{
    const IFF_ChunkType *l = (IFF_ChunkType*)a;
    const IFF_ChunkType *r = (IFF_ChunkType*)b;

    if(l->chunkId < r->chunkId)
        return -1;
    else if(l->chunkId > r->chunkId)
        return 1;
    else
        return 0;
}

static IFF_ChunkType *getChunkType(const IFF_ID chunkId, const IFF_ChunkTypesNode *chunkTypesNode)
{
    if(chunkTypesNode == NULL)
        return NULL;
    else
    {
        IFF_ChunkType *result;
        IFF_ChunkType key;
        key.chunkId = chunkId;

        result = (IFF_ChunkType*)bsearch(&key, chunkTypesNode->chunkTypes, chunkTypesNode->chunkTypesLength, sizeof(IFF_ChunkType), &compareChunkTypes);

        if(result == NULL)
            return getChunkType(chunkId, chunkTypesNode->parent);
        else
            return result;
    }
}

IFF_ChunkType *IFF_findChunkType(const IFF_ChunkRegistry *chunkRegistry, const IFF_ID formType, const IFF_ID chunkId)
{
    if(chunkRegistry == NULL)
        return NULL;
    else
    {
        /* Search for the requested FORM chunk types */
        const IFF_FormChunkTypes *formChunkTypes = getFormChunkTypes(formType, chunkRegistry);
        IFF_ChunkType *result;

        if(formChunkTypes == NULL)
            result = getChunkType(chunkId, chunkRegistry->globalChunkTypesNode); /* Search for the chunk type that handles the a chunk with the given chunk id */
        else
            result = getChunkType(chunkId, formChunkTypes->chunkTypesNode); /* Search for the chunk type that handles the a chunk with the given chunk id */

        if(result == NULL)
            return chunkRegistry->defaultChunkType;
        else
            return result;
    }
}
