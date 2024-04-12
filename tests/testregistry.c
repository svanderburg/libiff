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

#include "testregistry.h"
#include "defaultregistry.h"
#include "hello.h"
#include "bye.h"

static IFF_ChunkType applicationChunkTypes[] = {
    {TEST_ID_BYE, &TEST_createByeChunk, &TEST_readByeContents, &TEST_writeByeContents, &TEST_checkByeContents, &TEST_clearByeContents, &TEST_printByeContents, &TEST_compareByeContents},
    {TEST_ID_HELO, &TEST_createHelloChunk, &TEST_readHelloContents, &TEST_writeHelloContents, &TEST_checkHelloContents, &TEST_clearHelloContents, &TEST_printHelloContents, &TEST_compareHelloContents}
};

static IFF_ChunkTypesNode applicationChunkTypesNode = {
    TEST_NUM_OF_CHUNK_TYPES, applicationChunkTypes, NULL
};

static IFF_FormChunkTypes formChunkTypes[] = {
    { TEST_ID_TEST, &applicationChunkTypesNode }
};

const IFF_ChunkRegistry TEST_chunkRegistry = IFF_EXTEND_DEFAULT_REGISTRY_WITH_FORM_CHUNK_TYPES(TEST_NUM_OF_FORM_CHUNK_TYPES, formChunkTypes);
