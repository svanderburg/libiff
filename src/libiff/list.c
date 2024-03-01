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

#include "list.h"
#include <stdlib.h>
#include "id.h"
#include "field.h"
#include "util.h"
#include "cat.h"
#include "error.h"

IFF_List *IFF_createList(const IFF_Long chunkSize, const IFF_ID contentsType)
{
    IFF_List *list = (IFF_List*)IFF_createChunk(IFF_ID_LIST, chunkSize, sizeof(IFF_List));

    if(list != NULL)
    {
        IFF_initGroup((IFF_Group*)list, contentsType);

        list->prop = NULL;
        list->propLength = 0;
    }

    return list;
}

IFF_List *IFF_createEmptyListWithContentsType(const IFF_ID contentsType)
{
    return IFF_createList(IFF_ID_SIZE /* We have a contentsType field so it is not 0 */, contentsType);
}

IFF_List *IFF_createEmptyList(void)
{
    return IFF_createEmptyListWithContentsType(IFF_ID_JJJJ);
}

IFF_Chunk *IFF_createUnparsedList(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    return (IFF_Chunk*)IFF_createList(chunkSize, 0);
}

static void addPropToList(IFF_List *list, IFF_Prop *prop)
{
    list->prop = (IFF_Prop**)realloc(list->prop, (list->propLength + 1) * sizeof(IFF_Prop*));
    list->prop[list->propLength] = prop;
    list->propLength++;
    prop->parent = (IFF_Group*)list;
}

void IFF_addPropToList(IFF_List *list, IFF_Prop *prop)
{
    addPropToList(list, prop);
    list->chunkSize = IFF_incrementChunkSize(list->chunkSize, (IFF_Chunk*)prop);
}

void IFF_addToList(IFF_List *list, IFF_Chunk *chunk)
{
    IFF_addToCAT((IFF_CAT*)list, chunk);
}

void IFF_addToListAndUpdateContentsType(IFF_List *list, IFF_Chunk *chunk)
{
    IFF_addToCATAndUpdateContentsType((IFF_CAT*)list, chunk);
}

static IFF_Bool readListSubChunks(FILE *file, IFF_List *list, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    while(*bytesProcessed < list->chunkSize)
    {
        /* Read sub chunk */
        IFF_Chunk *chunk = IFF_readChunk(file, list->contentsType, chunkRegistry);

        if(chunk == NULL)
            return FALSE;

        /* Add the PROP chunk or arbitrary sub chunk */
        if(chunk->chunkId == IFF_ID_PROP)
            addPropToList(list, (IFF_Prop*)chunk);
        else
            IFF_attachToGroup((IFF_Group*)list, chunk);

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, chunk);
    }

    if(*bytesProcessed > list->chunkSize)
        IFF_error("WARNING: truncated LIST chunk! The size specifies: %d but the total amount of its sub chunks is: %d bytes. The parser may get confused!\n", list->chunkSize, bytesProcessed);

    return TRUE;
}

IFF_Bool IFF_readList(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    IFF_List *list = (IFF_List*)chunk;
    IFF_FieldStatus status;

    /* Read the contentsType id */
    if((status = IFF_readIdField(file, &list->contentsType, chunk, "contentsType", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    /* Read the remaining nested sub chunks */
    if(!readListSubChunks(file, list, chunkRegistry, bytesProcessed))
    {
        IFF_error("Error reading chunk in list!\n");
        return FALSE;
    }

    /* Return the result */
    return TRUE;
}

static IFF_Bool writeListPropChunks(FILE *file, const IFF_List *list, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    unsigned int i;

    for(i = 0; i < list->propLength; i++)
    {
        if(!IFF_writeChunk(file, (IFF_Chunk*)list->prop[i], 0, chunkRegistry))
        {
            IFF_error("Error writing PROP!\n");
            return FALSE;
        }

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, (IFF_Chunk*)list->prop[i]);
    }

    return TRUE;
}

IFF_Bool IFF_writeList(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const IFF_List *list = (const IFF_List*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeIdField(file, list->contentsType, chunk, "contentsType", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if(!writeListPropChunks(file, list, chunkRegistry, bytesProcessed))
        return FALSE;

    if(!IFF_writeGroupSubChunks(file, (const IFF_Group*)chunk, chunkRegistry, bytesProcessed))
        return FALSE;

    return TRUE;
}

static IFF_Long checkListPropChunks(const IFF_List *list, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_Long chunkSize = 0;
    unsigned int i;

    for(i = 0; i < list->propLength; i++)
    {
        IFF_Chunk *propChunk = (IFF_Chunk*)list->prop[i];

        if(!IFF_checkChunk(propChunk, list->contentsType, chunkRegistry))
            return -1;

        chunkSize = IFF_incrementChunkSize(chunkSize, propChunk);
    }

    return chunkSize;
}

IFF_Bool IFF_checkList(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_List *list = (const IFF_List*)chunk;

    IFF_Long chunkSize = IFF_ID_SIZE;
    IFF_Long subChunkSize;

    if(!IFF_checkId(list->contentsType))
        return FALSE;

    /* Check validity of PROP chunks */
    if((subChunkSize = checkListPropChunks(list, chunkRegistry)) == -1)
        return FALSE;

    chunkSize += subChunkSize;

    /* Check validity of other sub chunks */
    if((subChunkSize = IFF_checkGroupSubChunks((IFF_Group*)list, &IFF_checkCATSubChunk, chunkRegistry)) == -1)
        return FALSE;

    chunkSize += subChunkSize;

    /* Check whether the calculated chunk size matches the chunks' chunk size */
    if(!IFF_checkGroupChunkSize((IFF_Group*)list, chunkSize))
        return FALSE;

    return TRUE;
}

static void freeListPropChunks(IFF_List *list, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    for(i = 0; i < list->propLength; i++)
        IFF_freeChunk((IFF_Chunk*)list->prop[i], list->contentsType, chunkRegistry);
}

void IFF_freeList(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_List *list = (IFF_List*)chunk;

    IFF_freeCAT(chunk, chunkRegistry);
    freeListPropChunks(list, chunkRegistry);
    free(list->prop);
}

static void printListPropChunks(const IFF_List *list, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    IFF_printIndent(stdout, indentLevel, "prop = [\n");

    for(i = 0; i < list->propLength; i++)
        IFF_printChunk((IFF_Chunk*)list->prop[i], indentLevel + 1, list->contentsType, chunkRegistry);

    IFF_printIndent(stdout, indentLevel, "];\n");
}

void IFF_printList(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_List *list = (const IFF_List*)chunk;

    IFF_printGroupType("contentsType", list->contentsType, indentLevel);
    printListPropChunks(list, indentLevel, chunkRegistry);
    IFF_printGroupSubChunks((const IFF_Group*)list, indentLevel, chunkRegistry);
}

static IFF_Bool compareListPropChunks(const IFF_List *list1, const IFF_List *list2, const IFF_ChunkRegistry *chunkRegistry)
{
    if(list1->propLength == list2->propLength)
    {
        unsigned int i;

        for(i = 0; i < list1->propLength; i++)
        {
            if(!IFF_compareProp((IFF_Chunk*)list1->prop[i], (IFF_Chunk*)list2->prop[i], chunkRegistry))
               return FALSE;
        }

        return TRUE;
    }
    else
        return FALSE;
}

IFF_Bool IFF_compareList(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    if(!compareListPropChunks((const IFF_List*)chunk1, (const IFF_List*)chunk2, chunkRegistry))
        return FALSE;

    if(!IFF_compareCAT(chunk1, chunk2, chunkRegistry))
        return FALSE;

    return TRUE;
}

IFF_Form **IFF_searchFormsInList(IFF_List *list, const IFF_ID *formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
{
    return IFF_searchFormsInCAT((IFF_CAT*)list, formTypes, formTypesLength, formsLength);
}

void IFF_updateListChunkSizes(IFF_List *list)
{
    unsigned int i;

    IFF_updateCATChunkSizes((IFF_CAT*)list);

    for(i = 0; i < list->propLength; i++)
        list->chunkSize = IFF_incrementChunkSize(list->chunkSize, (IFF_Chunk*)list->prop[i]);
}

IFF_Prop *IFF_getPropFromList(const IFF_List *list, const IFF_ID formType)
{
    unsigned int i;

    for(i = 0; i < list->propLength; i++)
    {
        if(list->prop[i]->formType == formType)
            return list->prop[i];
    }

    return NULL;
}
