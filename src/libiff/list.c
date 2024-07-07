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

IFF_ChunkInterface IFF_listInterface = {&IFF_createUnparsedList, &IFF_readListContents, &IFF_writeListContents, &IFF_checkListContents, &IFF_clearListContents, &IFF_printListContents, &IFF_compareListContents, &IFF_traverseGroupChunkHierarchy};

IFF_List *IFF_createList(const IFF_Long chunkSize, const IFF_ID contentsType)
{
    IFF_List *list = (IFF_List*)IFF_createChunk(IFF_ID_LIST, chunkSize, sizeof(IFF_List));

    if(list != NULL)
    {
        IFF_initGroup((IFF_Group*)list, contentsType);

        list->props = NULL;
        list->propsLength = 0;
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
    list->props = (IFF_Prop**)realloc(list->props, (list->propsLength + 1) * sizeof(IFF_Prop*));
    list->props[list->propsLength] = prop;
    list->propsLength++;
    prop->parent = (IFF_Chunk*)list;
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

static IFF_Bool readListSubChunks(FILE *file, IFF_List *list, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int index = 0;

    IFF_visitAttributeByName(attributePath, "props");

    while(*bytesProcessed < list->chunkSize)
    {
        /* Read sub chunk */
        IFF_Chunk *chunk;

        IFF_visitAttributeByIndex(attributePath, index);

        chunk = IFF_readChunk(file, list->contentsType, chunkRegistry, attributePath, error);

        if(chunk == NULL)
            return FALSE;

        /* Add the PROP chunk or arbitrary sub chunk */
        if(chunk->chunkId == IFF_ID_PROP)
            addPropToList(list, (IFF_Prop*)chunk);
        else
            IFF_attachToGroup((IFF_Group*)list, chunk);

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, chunk);
        index++;

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return TRUE;
}

IFF_Bool IFF_readListContents(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_List *list = (IFF_List*)chunk;
    IFF_FieldStatus status;

    /* Read the contentsType id */
    if((status = IFF_readIdField(file, &list->contentsType, chunk, attributePath, "contentsType", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    /* Read the remaining nested sub chunks */
    if(!readListSubChunks(file, list, chunkRegistry, attributePath, bytesProcessed, error))
        return FALSE;

    /* Return the result */
    return TRUE;
}

static IFF_Bool writeListPropChunks(FILE *file, const IFF_List *list, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int i;

    IFF_visitAttributeByName(attributePath, "props");

    for(i = 0; i < list->propsLength; i++)
    {
        IFF_visitAttributeByIndex(attributePath, i);

        if(!IFF_writeChunk(file, (IFF_Chunk*)list->props[i], 0, chunkRegistry, attributePath, error))
            return FALSE;

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_incrementChunkSize(*bytesProcessed, (IFF_Chunk*)list->props[i]);

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return TRUE;
}

IFF_Bool IFF_writeListContents(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    const IFF_List *list = (const IFF_List*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeIdField(file, list->contentsType, chunk, attributePath, "contentsType", bytesProcessed, error)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if(!writeListPropChunks(file, list, chunkRegistry, attributePath, bytesProcessed, error))
        return FALSE;

    if(!IFF_writeGroupSubChunks(file, (const IFF_Group*)chunk, chunkRegistry, attributePath, bytesProcessed, error))
        return FALSE;

    return TRUE;
}

IFF_Long IFF_computeActualListChunkSize(const IFF_List *list)
{
    IFF_Long chunkSize = IFF_computeActualGroupChunkSize((const IFF_Group*)list);
    unsigned int i;

    for(i = 0; i < list->propsLength; i++)
    {
        IFF_Chunk *propChunk = (IFF_Chunk*)list->props[i];
        chunkSize = IFF_incrementChunkSize(chunkSize, propChunk);
    }

    return chunkSize;
}

static IFF_QualityLevel checkListPropChunks(const IFF_List *list, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    unsigned int i;
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;

    IFF_visitAttributeByName(attributePath, "props");

    for(i = 0; i < list->propsLength; i++)
    {
        IFF_Chunk *propChunk = (IFF_Chunk*)list->props[i];

        IFF_visitAttributeByIndex(attributePath, i);

        qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(propChunk, list->contentsType, chunkRegistry, attributePath, printCheckMessage, data));

        IFF_unvisitAttribute(attributePath);
    }

    IFF_unvisitAttribute(attributePath);

    return qualityLevel;
}

IFF_QualityLevel IFF_checkListContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    const IFF_List *list = (const IFF_List*)chunk;
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    IFF_Long actualChunkSize;

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkId(list->contentsType, attributePath, "contentsType", printCheckMessage, data, list->chunkId));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, checkListPropChunks(list, chunkRegistry, attributePath, printCheckMessage, data));
    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkGroupSubChunks((const IFF_Group*)list, &IFF_checkCATSubChunk, chunkRegistry, attributePath, printCheckMessage, data));

    actualChunkSize = IFF_computeActualListChunkSize(list);

    qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkGroupChunkSize((const IFF_Group*)list, actualChunkSize, attributePath, printCheckMessage, data));

    return qualityLevel;
}

static void freeListPropChunks(IFF_List *list, const IFF_ChunkRegistry *chunkRegistry)
{
    unsigned int i;

    for(i = 0; i < list->propsLength; i++)
        IFF_freeChunk((IFF_Chunk*)list->props[i], list->contentsType, chunkRegistry);
}

void IFF_clearListContents(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_List *list = (IFF_List*)chunk;

    IFF_clearGroupContents(chunk, chunkRegistry);
    freeListPropChunks(list, chunkRegistry);
    free(list->props);
}

static void printListPropChunks(FILE *file, const IFF_List *list, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_PrintChunksArrayParameter param;
    param.groupType = list->contentsType;
    param.chunksLength = list->propsLength;
    param.chunks = (IFF_Chunk**)list->props;
    param.chunkRegistry = chunkRegistry;

    IFF_printField(file, indentLevel, "props", &param, IFF_printChunksArray);
}

void IFF_printListContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const IFF_List *list = (const IFF_List*)chunk;

    IFF_printGroupType(file, "contentsType", list->contentsType, indentLevel);
    printListPropChunks(file, list, indentLevel, chunkRegistry);
    IFF_printGroupSubChunks(file, (const IFF_Group*)list, indentLevel, chunkRegistry);
}

static IFF_Bool compareListPropChunks(const IFF_List *list1, const IFF_List *list2, const IFF_ChunkRegistry *chunkRegistry)
{
    if(list1->propsLength == list2->propsLength)
    {
        unsigned int i;

        for(i = 0; i < list1->propsLength; i++)
        {
            if(!IFF_compareGroupContents((IFF_Chunk*)list1->props[i], (IFF_Chunk*)list2->props[i], chunkRegistry))
               return FALSE;
        }

        return TRUE;
    }
    else
        return FALSE;
}

IFF_Bool IFF_compareListContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    return compareListPropChunks((const IFF_List*)chunk1, (const IFF_List*)chunk2, chunkRegistry)
        && IFF_compareGroupContents(chunk1, chunk2, chunkRegistry);
}

void IFF_updateListChunkSizes(IFF_List *list)
{
    unsigned int i;

    IFF_updateCATChunkSizes((IFF_CAT*)list);

    for(i = 0; i < list->propsLength; i++)
        list->chunkSize = IFF_incrementChunkSize(list->chunkSize, (IFF_Chunk*)list->props[i]);
}

IFF_Prop *IFF_getPropFromList(const IFF_List *list, const IFF_ID formType)
{
    unsigned int i;

    for(i = 0; i < list->propsLength; i++)
    {
        if(list->props[i]->formType == formType)
            return list->props[i];
    }

    return NULL;
}
