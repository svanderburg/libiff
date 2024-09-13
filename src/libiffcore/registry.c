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

#include "registry.h"
#include <stdlib.h>
#include "io.h"
#include "id.h"

static int compareScopedChunkTypes(const void *a, const void *b)
{
    const IFF_ScopedChunkTypes *l = (const IFF_ScopedChunkTypes*)a;
    const IFF_ScopedChunkTypes *r = (const IFF_ScopedChunkTypes*)b;

    if(l->scopeId < r->scopeId)
        return -1;
    else if(l->scopeId > r->scopeId)
        return 1;
    else
        return 0;
}

const static IFF_ScopedChunkTypes *getScopedChunkTypes(const IFF_ID scopeId, const IFF_Registry *registry)
{
    IFF_ScopedChunkTypes key;
    key.scopeId = scopeId;

    return (IFF_ScopedChunkTypes*)bsearch(&key, registry->scopedChunkTypes, registry->scopedChunkTypesLength, sizeof(IFF_ScopedChunkTypes), &compareScopedChunkTypes);
}

static int compareChunkTypes(const void *a, const void *b)
{
    const IFF_ChunkType *l = (const IFF_ChunkType*)a;
    const IFF_ChunkType *r = (const IFF_ChunkType*)b;

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

IFF_ChunkInterface *IFF_findChunkInterface(const IFF_Registry *registry, const IFF_ID scopeId, const IFF_ID chunkId)
{
    /* Search for the requested scoped chunk types */
    const IFF_ScopedChunkTypes *scopedChunkTypes = getScopedChunkTypes(scopeId, registry);
    IFF_ChunkType *result;

    /* Search for the chunk type that handles the chunk with the given chunk id */
    if(scopedChunkTypes == NULL)
        result = getChunkType(chunkId, registry->globalChunkTypesNode);
    else
        result = getChunkType(chunkId, scopedChunkTypes->chunkTypesNode);

    if(result == NULL)
        return registry->defaultChunkInterface;
    else
        return result->chunkInterface;
}
