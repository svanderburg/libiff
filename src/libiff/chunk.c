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

#include "chunk.h"
#include <string.h>
#include <stdlib.h>
#include "io.h"
#include "id.h"
#include "util.h"
#include "form.h"
#include "cat.h"
#include "list.h"
#include "prop.h"
#include "datachunk.h"
#include "util.h"
#include "error.h"

#define ID_EMPTY IFF_MAKEID(' ', ' ', ' ', ' ')

IFF_Chunk *IFF_allocateChunk(const IFF_ID chunkId, const IFF_Long chunkSize, size_t structSize)
{
    IFF_Chunk *chunk = (IFF_Chunk*)malloc(structSize);

    if(chunk != NULL)
    {
        chunk->parent = NULL;
        chunk->chunkId = chunkId;
        chunk->chunkSize = chunkSize;
    }

    return chunk;
}

IFF_Chunk *IFF_readChunk(FILE *file, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_ID chunkId;
    IFF_Long chunkSize;
    IFF_Chunk *chunk;

    /* Read chunk id */
    if(!IFF_readId(file, &chunkId, ID_EMPTY, ""))
        return NULL;

    /* Read chunk size */
    if(!IFF_readLong(file, &chunkSize, chunkId, "chunkSize"))
        return NULL;

    /* Read remaining bytes (procedure depends on chunk id type) */

    if(chunkId == IFF_ID_FORM)
        chunk = (IFF_Chunk*)IFF_readForm(file, chunkSize, extension, extensionLength);
    else if(chunkId == IFF_ID_CAT)
        chunk = (IFF_Chunk*)IFF_readCAT(file, chunkSize, extension, extensionLength);
    else if(chunkId == IFF_ID_LIST)
        chunk = (IFF_Chunk*)IFF_readList(file, chunkSize, extension, extensionLength);
    else if(chunkId == IFF_ID_PROP)
        chunk = (IFF_Chunk*)IFF_readProp(file, chunkSize, extension, extensionLength);
    else
        chunk = IFF_readDataChunk(file, chunkId, chunkSize, formType, extension, extensionLength);

    if(chunk == NULL)
        return NULL;

    /* If the chunk size is odd, we have to read the padding byte */
    if(!IFF_readPaddingByte(file, chunkSize, chunk->chunkId))
    {
        IFF_freeChunk(chunk, formType, extension, extensionLength);
        return NULL;
    }

    return chunk;
}

IFF_Bool IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(!IFF_writeId(file, chunk->chunkId, chunk->chunkId, "chunkId"))
        return FALSE;

    if(!IFF_writeLong(file, chunk->chunkSize, chunk->chunkId, "chunkSize"))
        return FALSE;

    if(chunk->chunkId == IFF_ID_FORM)
    {
        if(!IFF_writeForm(file, (IFF_Form*)chunk, extension, extensionLength))
            return FALSE;
    }
    else if(chunk->chunkId == IFF_ID_CAT)
    {
        if(!IFF_writeCAT(file, (IFF_CAT*)chunk, extension, extensionLength))
            return FALSE;
    }
    else if(chunk->chunkId == IFF_ID_LIST)
    {
        if(!IFF_writeList(file, (IFF_List*)chunk, extension, extensionLength))
            return FALSE;
    }
    else if(chunk->chunkId == IFF_ID_PROP)
    {
        if(!IFF_writeProp(file, (IFF_Prop*)chunk, extension, extensionLength))
            return FALSE;
    }
    else
    {
        if(!IFF_writeDataChunk(file, chunk, formType, extension, extensionLength))
            return FALSE;
    }

    /* If the chunk size is odd, we have to write the padding byte */
    if(!IFF_writePaddingByte(file, chunk->chunkSize, chunk->chunkId))
        return FALSE;

    return TRUE;
}

IFF_Bool IFF_checkChunk(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(!IFF_checkId(chunk->chunkId))
        return FALSE;
    else
    {
        if(chunk->chunkId == IFF_ID_FORM)
            return IFF_checkForm((const IFF_Form*)chunk, extension, extensionLength);
        else if(chunk->chunkId == IFF_ID_CAT)
            return IFF_checkCAT((const IFF_CAT*)chunk, extension, extensionLength);
        else if(chunk->chunkId == IFF_ID_LIST)
            return IFF_checkList((const IFF_List*)chunk, extension, extensionLength);
        else if(chunk->chunkId == IFF_ID_PROP)
            return IFF_checkProp((const IFF_Prop*)chunk, extension, extensionLength);
        else
            return IFF_checkDataChunk(chunk, formType, extension, extensionLength);
    }
}

void IFF_freeChunk(IFF_Chunk *chunk, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    /* Free nested sub chunks */
    if(chunk->chunkId == IFF_ID_FORM)
        IFF_freeForm((IFF_Form*)chunk, extension, extensionLength);
    else if(chunk->chunkId == IFF_ID_CAT)
        IFF_freeCAT((IFF_CAT*)chunk, extension, extensionLength);
    else if(chunk->chunkId == IFF_ID_LIST)
        IFF_freeList((IFF_List*)chunk, extension, extensionLength);
    else if(chunk->chunkId == IFF_ID_PROP)
        IFF_freeProp((IFF_Prop*)chunk, extension, extensionLength);
    else
        IFF_freeDataChunk(chunk, formType, extension, extensionLength);

    /* Free the chunk itself */
    free(chunk);
}

void IFF_printChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printIndent(stdout, indentLevel, "'");
    IFF_printId(chunk->chunkId);
    printf("' = {\n");

    IFF_printIndent(stdout, indentLevel + 1, "chunkSize = %d;\n", chunk->chunkSize);

    if(chunk->chunkId == IFF_ID_FORM)
        IFF_printForm((const IFF_Form*)chunk, indentLevel + 1, extension, extensionLength);
    else if(chunk->chunkId == IFF_ID_CAT)
        IFF_printCAT((const IFF_CAT*)chunk, indentLevel + 1, extension, extensionLength);
    else if(chunk->chunkId == IFF_ID_LIST)
        IFF_printList((const IFF_List*)chunk, indentLevel + 1, extension, extensionLength);
    else if(chunk->chunkId == IFF_ID_PROP)
        IFF_printProp((const IFF_Prop*)chunk, indentLevel + 1, extension, extensionLength);
    else
        IFF_printDataChunk(chunk, indentLevel + 1, formType, extension, extensionLength);

    IFF_printIndent(stdout, indentLevel, "}\n\n");
}

IFF_Bool IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(chunk1->chunkId == chunk2->chunkId && chunk1->chunkSize == chunk2->chunkSize)
    {
        if(chunk1->chunkId == IFF_ID_FORM)
            return IFF_compareForm((const IFF_Form*)chunk1, (const IFF_Form*)chunk2, extension, extensionLength);
        else if(chunk1->chunkId == IFF_ID_CAT)
            return IFF_compareCAT((const IFF_CAT*)chunk1, (const IFF_CAT*)chunk2, extension, extensionLength);
        else if(chunk1->chunkId == IFF_ID_LIST)
            return IFF_compareList((const IFF_List*)chunk1, (const IFF_List*)chunk2, extension, extensionLength);
        else if(chunk1->chunkId == IFF_ID_PROP)
            return IFF_compareProp((const IFF_Prop*)chunk1, (const IFF_Prop*)chunk2, extension, extensionLength);
        else
            return IFF_compareDataChunk(chunk1, chunk2, formType, extension, extensionLength);
    }
    else
        return FALSE;
}

IFF_Form **IFF_searchFormsFromArray(IFF_Chunk *chunk, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    if(chunk->chunkId == IFF_ID_FORM)
        return IFF_searchFormsInForm((IFF_Form*)chunk, formTypes, formTypesLength, formsLength);
    else if(chunk->chunkId == IFF_ID_CAT)
        return IFF_searchFormsInCAT((IFF_CAT*)chunk, formTypes, formTypesLength, formsLength);
    else if(chunk->chunkId == IFF_ID_LIST)
        return IFF_searchFormsInList((IFF_List*)chunk, formTypes, formTypesLength, formsLength);
    else
    {
        *formsLength = 0;
        return NULL;
    }
}

IFF_Form **IFF_searchForms(IFF_Chunk *chunk, const IFF_ID formType, unsigned int *formsLength)
{
    IFF_ID formTypes[1];
    formTypes[0] = formType;
    return IFF_searchFormsFromArray(chunk, formTypes, 1, formsLength);
}

void IFF_updateChunkSizes(IFF_Chunk *chunk)
{
    /* Check whether the given chunk is a group chunk and update the sizes */
    if(chunk->chunkId == IFF_ID_FORM)
        IFF_updateFormChunkSizes((IFF_Form*)chunk);
    else if(chunk->chunkId == IFF_ID_PROP)
        IFF_updatePropChunkSizes((IFF_Prop*)chunk);
    else if(chunk->chunkId == IFF_ID_CAT)
        IFF_updateCATChunkSizes((IFF_CAT*)chunk);
    else if(chunk->chunkId == IFF_ID_LIST)
        IFF_updateListChunkSizes((IFF_List*)chunk);

    /* If the given type has a parent, recursively update these as well */
    if(chunk->parent != NULL)
        IFF_updateChunkSizes((IFF_Chunk*)chunk->parent);
}
