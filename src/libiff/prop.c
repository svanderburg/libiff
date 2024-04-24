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

#define PROP_GROUPTYPENAME "formType"

IFF_ChunkInterface IFF_propInterface = {&IFF_createUnparsedGroup, &IFF_readFormContents, &IFF_writeFormContents, &IFF_checkPropContents, &IFF_clearGroupContents, &IFF_printFormContents, &IFF_compareGroupContents};

IFF_Prop *IFF_createProp(const IFF_Long chunkSize, const IFF_ID formType)
{
    return (IFF_Prop*)IFF_createGroup(IFF_ID_PROP, chunkSize, formType);
}

IFF_Prop *IFF_createEmptyProp(const IFF_ID formType)
{
    return (IFF_Prop*)IFF_createEmptyGroup(IFF_ID_PROP, formType);
}

void IFF_addToProp(IFF_Prop *prop, IFF_Chunk *chunk)
{
    IFF_addToForm((IFF_Form*)prop, chunk);
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
    return IFF_checkGroupContents((const IFF_Group*)chunk, "formType", &IFF_checkFormType, &subChunkCheck, chunkRegistry, attributePath, printCheckMessage, data);
}

void IFF_updatePropChunkSizes(IFF_Prop *prop)
{
    IFF_updateFormChunkSizes((IFF_Form*)prop);
}

IFF_Chunk *IFF_getChunkFromProp(const IFF_Prop *prop, const IFF_ID chunkId)
{
    return IFF_getDataChunkFromForm((IFF_Form*)prop, chunkId);
}
