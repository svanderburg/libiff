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
#include "error.h"

#define PROP_GROUPTYPENAME "formType"

IFF_Prop *IFF_createProp(const IFF_Long chunkSize, const IFF_ID formType)
{
    return (IFF_Prop*)IFF_createGroup(IFF_ID_PROP, chunkSize, formType);
}

IFF_Prop *IFF_createEmptyProp(const IFF_ID formType)
{
    return (IFF_Prop*)IFF_createEmptyGroup(IFF_ID_PROP, formType);
}

IFF_Chunk *IFF_createUnparsedProp(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    return IFF_createUnparsedGroup(chunkId, chunkSize);
}

void IFF_addToProp(IFF_Prop *prop, IFF_Chunk *chunk)
{
    IFF_addToForm((IFF_Form*)prop, chunk);
}

IFF_Bool IFF_readProp(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    return IFF_readGroup(file, chunk, PROP_GROUPTYPENAME, chunkRegistry, bytesProcessed);
}

IFF_Bool IFF_writeProp(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    return IFF_writeForm(file, chunk, chunkRegistry, bytesProcessed);
}

static IFF_Bool subChunkCheck(const IFF_Group *group, const IFF_Chunk *subChunk)
{
    if(subChunk->chunkId == IFF_ID_FORM ||
       subChunk->chunkId == IFF_ID_LIST ||
       subChunk->chunkId == IFF_ID_CAT ||
       subChunk->chunkId == IFF_ID_PROP)
    {
        IFF_error("ERROR: Element with chunk Id: '");
        IFF_errorId(subChunk->chunkId);
        IFF_error("' not allowed in PROP chunk!\n");

        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_checkProp(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_checkGroup((IFF_Group*)chunk, &IFF_checkFormType, &subChunkCheck, chunkRegistry);
}

void IFF_freeProp(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_freeForm(chunk, chunkRegistry);
}

void IFF_printProp(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printForm(chunk, indentLevel, chunkRegistry);
}

IFF_Bool IFF_compareProp(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_compareForm(chunk1, chunk2, chunkRegistry);
}

void IFF_updatePropChunkSizes(IFF_Prop *prop)
{
    IFF_updateFormChunkSizes((IFF_Form*)prop);
}

IFF_Chunk *IFF_getChunkFromProp(const IFF_Prop *prop, const IFF_ID chunkId)
{
    return IFF_getDataChunkFromForm((IFF_Form*)prop, chunkId);
}
