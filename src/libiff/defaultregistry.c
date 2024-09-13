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

#include "defaultregistry.h"
#include "id.h"
#include "chunk.h"
#include "form.h"
#include "cat.h"
#include "list.h"
#include "prop.h"
#include "textchunk.h"

static IFF_ChunkType IFF_globalChunkTypes[] = {
    {IFF_ID_CAT, &IFF_catInterface},
    {IFF_ID_FORM, &IFF_formInterface},
    {IFF_ID_LIST, &IFF_listInterface},
    {IFF_ID_PROP, &IFF_propInterface},
    {IFF_ID_TEXT, &IFF_textChunkInterface},
};

IFF_ChunkTypesNode IFF_globalChunkTypesNode = {
    IFF_NUM_OF_CHUNK_TYPES, IFF_globalChunkTypes, NULL
};

IFF_QualityLevel IFF_checkMainIFFChunk(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    if(chunk->chunkId == IFF_ID_FORM || /* The main chunk must have one of the following IDs: FORM, CAT or LIST */
       chunk->chunkId == IFF_ID_CAT ||
       chunk->chunkId == IFF_ID_LIST)
        return IFF_QUALITY_PERFECT;
    else
    {
        IFF_ID2 chunkId;
        IFF_idToString(chunk->chunkId, chunkId);
        printCheckMessage(attributePath, "chunkId", chunk->chunkId, data, "is invalid: the main chunkId should be: \"FORM\", \"CAT \" or \"LIST\", value is: \"%.4s\"", chunkId);
        return IFF_QUALITY_INCONSISTENT;
    }
}

const IFF_Registry IFF_defaultRegistry = {
    0, NULL, &IFF_globalChunkTypesNode, &IFF_rawChunkInterface, &IFF_checkMainIFFChunk, NULL
};
