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

#include "prop.h"
#include <stdlib.h>
#include <string.h>
#include "id.h"
#include "group.h"
#include "list.h"
#include "cat.h"
#include "util.h"
#include "formregistry.h"

#define PROP_GROUPTYPENAME "formType"

IFF_ChunkInterface IFF_propInterface = {&IFF_parseFormContents, &IFF_writeFormContents, &IFF_checkPropContents, &IFF_clearFormContents, &IFF_printFormContents, &IFF_compareFormContents, &IFF_traverseFormChunkHierarchy, &IFF_recalculateFormChunkSize};

IFF_Prop *IFF_createProp(const IFF_Long chunkSize, const IFF_ID formType, const IFF_GroupStructure *formStructure)
{
    return (IFF_Prop*)IFF_createGroup(IFF_ID_PROP, chunkSize, formType, formStructure);
}

IFF_Prop *IFF_createEmptyProp(const IFF_ID formType, const IFF_GroupStructure *formStructure)
{
    return (IFF_Prop*)IFF_createEmptyGroup(IFF_ID_PROP, formType, formStructure);
}

void IFF_addChunkToProp(IFF_Prop *prop, const IFF_GroupStructure *formStructure, IFF_Chunk *chunk)
{
    IFF_addChunkToForm((IFF_Form*)prop, formStructure, chunk);
}

IFF_Chunk *IFF_removeChunkFromPropByIndex(IFF_Prop *prop, const unsigned int index)
{
    return IFF_removeChunkFromFormByIndex((IFF_Form*)prop, index);
}

IFF_Chunk *IFF_updateInPropByIndex(IFF_Prop *prop, const unsigned int index, IFF_Chunk *chunk)
{
    return IFF_updateChunkInFormByIndex((IFF_Form*)prop, index, chunk);
}

static IFF_QualityLevel subChunkCheck(const IFF_Group *group, const IFF_Chunk *subChunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    if(subChunk->chunkId == IFF_ID_FORM ||
       subChunk->chunkId == IFF_ID_LIST ||
       subChunk->chunkId == IFF_ID_CAT ||
       subChunk->chunkId == IFF_ID_PROP)
    {
        IFF_ID2 subChunkId;
        IFF_idToString(subChunk->chunkId, subChunkId);
        printCheckMessage(attributePath, NULL, group->chunkId, data, "is a sub chunk with chunkId: \"%.4s\" that is not allowed", subChunkId);
        return IFF_QUALITY_OK;
    }
    else
        return IFF_QUALITY_PERFECT;
}

IFF_QualityLevel IFF_checkPropContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    const IFF_Prop *prop = (const IFF_Prop*)chunk;
    IFF_GroupStructure *formStructure = IFF_findGroupStructure(chunkRegistry, prop->formType);
    return IFF_checkGroupContents((const IFF_Group*)chunk, formStructure, PROP_GROUPTYPENAME, &IFF_checkFormType, &subChunkCheck, chunkRegistry, attributePath, printCheckMessage, data);
}

IFF_Chunk *IFF_searchChunkInProp(const IFF_Prop *prop, const IFF_ID chunkId)
{
    return IFF_searchChunkInForm((const IFF_Form*)prop, chunkId);
}

IFF_Chunk **IFF_searchChunksInProp(IFF_Chunk **chunks, const IFF_Prop *prop, const IFF_ID chunkId, unsigned int *chunksLength)
{
    return IFF_searchChunksInForm(chunks, (const IFF_Form*)prop, chunkId, chunksLength);
}
