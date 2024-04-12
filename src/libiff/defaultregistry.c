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
    {IFF_ID_CAT, &IFF_createUnparsedCAT, &IFF_readCATContents, &IFF_writeCATContents, &IFF_checkCATContents, &IFF_clearCATContents, &IFF_printCATContents, &IFF_compareCATContents},
    {IFF_ID_FORM, &IFF_createUnparsedForm, &IFF_readFormContents, &IFF_writeFormContents, &IFF_checkFormContents, &IFF_clearFormContents, &IFF_printFormContents, &IFF_compareFormContents},
    {IFF_ID_LIST, &IFF_createUnparsedList, &IFF_readListContents, &IFF_writeListContents, &IFF_checkListContents, &IFF_clearListContents, &IFF_printListContents, &IFF_compareListContents},
    {IFF_ID_PROP, &IFF_createUnparsedProp, &IFF_readPropContents, &IFF_writePropContents, &IFF_checkPropContents, &IFF_clearPropContents, &IFF_printPropContents, &IFF_comparePropContents},
    {IFF_ID_TEXT, &IFF_createRawChunk, &IFF_readRawChunkContents, &IFF_writeRawChunkContents, &IFF_checkRawChunkContents, &IFF_clearRawChunkContents, &IFF_printTextChunkContents, &IFF_compareRawChunkContents},
};

IFF_ChunkTypesNode IFF_globalChunkTypesNode = {
    IFF_NUM_OF_CHUNK_TYPES, IFF_globalChunkTypes, NULL
};

IFF_ChunkType IFF_defaultChunkType = {0, &IFF_createRawChunk, &IFF_readRawChunkContents, &IFF_writeRawChunkContents, &IFF_checkRawChunkContents, &IFF_clearRawChunkContents, &IFF_printRawChunkContents, &IFF_compareRawChunkContents};

const IFF_ChunkRegistry IFF_defaultChunkRegistry = {
    0, NULL, &IFF_globalChunkTypesNode, &IFF_defaultChunkType
};
