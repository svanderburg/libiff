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
#include "formregistry.h"
#include "hello.h"
#include "bye.h"
#include "conversation.h"

static IFF_ChunkType applicationChunkTypes[] = {
    {TEST_ID_BYE, &TEST_byeInterface},
    {TEST_ID_HELO, &TEST_helloInterface}
};

static IFF_ChunkTypesNode applicationChunkTypesNode = {
    TEST_NUM_OF_GLOBAL_CHUNK_TYPES, applicationChunkTypes, NULL
};

static IFF_ScopedChunkTypes scopedChunkTypes[] = {
    { TEST_ID_CONV, &applicationChunkTypesNode }
};

static IFF_FormType formTypes[] = {
    { TEST_ID_CONV, &TEST_conversationStructure }
};

static IFF_FormTypesNode formTypesNode = {
    TEST_NUM_OF_FORM_TYPES,
    formTypes,
    NULL
};

const IFF_ChunkRegistry TEST_chunkRegistry = IFF_EXTEND_DEFAULT_REGISTRY_WITH_APPLICATION_FORMS(TEST_NUM_OF_SCOPED_CHUNK_TYPES, scopedChunkTypes, formTypesNode);
