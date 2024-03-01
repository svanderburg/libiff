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
#include "rawchunk.h"

static IFF_ChunkType IFF_globalChunkTypes[] = {
    {IFF_ID_CAT, &IFF_createUnparsedCAT, &IFF_readCAT, &IFF_writeCAT, &IFF_checkCAT, &IFF_freeCAT, &IFF_printCAT, &IFF_compareCAT},
    {IFF_ID_FORM, &IFF_createUnparsedForm, &IFF_readForm, &IFF_writeForm, &IFF_checkForm, &IFF_freeForm, &IFF_printForm, &IFF_compareForm},
    {IFF_ID_LIST, &IFF_createUnparsedList, &IFF_readList, &IFF_writeList, &IFF_checkList, &IFF_freeList, &IFF_printList, &IFF_compareList},
    {IFF_ID_PROP, &IFF_createUnparsedProp, &IFF_readProp, &IFF_writeProp, &IFF_checkProp, &IFF_freeProp, &IFF_printProp, &IFF_compareProp}
};

IFF_ChunkTypesNode IFF_globalChunkTypesNode = {
    IFF_NUM_OF_CHUNK_TYPES, IFF_globalChunkTypes, NULL
};

IFF_ChunkType IFF_defaultChunkType = {0, &IFF_createRawChunk, &IFF_readRawChunk, &IFF_writeRawChunk, &IFF_checkRawChunk, &IFF_freeRawChunk, &IFF_printRawChunk, &IFF_compareRawChunk};

const IFF_ChunkRegistry IFF_defaultChunkRegistry = {
    0, NULL, &IFF_globalChunkTypesNode, &IFF_defaultChunkType
};
