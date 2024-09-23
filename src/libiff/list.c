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
#include "pointerarray.h"

#define LIST_GROUPTYPENAME "contentsType"

IFF_ChunkInterface IFF_listInterface = {&IFF_parseListContents, &IFF_writeListContents, &IFF_checkListContents, &IFF_clearListContents, &IFF_printListContents, &IFF_compareListContents, &IFF_traverseCATChunkHierarchy, &IFF_recalculateListChunkSize};

static IFF_GroupMember listStructureMembers[] = {
    { IFF_ID_PROP, "props", IFF_GROUP_MEMBER_MULTIPLE }
};

typedef enum
{
    FIELD_INDEX_PROPS = 0
}
FieldIndex;

static IFF_Bool mapChunkIdToFieldIndex(const IFF_ID chunkId, unsigned int *index)
{
    if(chunkId == IFF_ID_PROP)
    {
        *index = FIELD_INDEX_PROPS;
        return TRUE;
    }
    else
        return FALSE;
}

static IFF_Chunk **getFieldPointer(const IFF_Group *group, const unsigned index)
{
    return NULL;
}

static IFF_Chunk ***getArrayFieldPointer(const IFF_Group *group, const unsigned index, unsigned int **chunksLength)
{
    if(index == 0)
    {
        IFF_List *list = (IFF_List*)group;
        *chunksLength = &list->propsLength;
        return (IFF_Chunk***)&list->props;
    }
    else
    {
        *chunksLength = 0;
        return NULL;
    }
}

IFF_GroupStructure listStructure = {
    sizeof(IFF_List),
    1,
    listStructureMembers,
    mapChunkIdToFieldIndex,
    getFieldPointer,
    getArrayFieldPointer
};

IFF_List *IFF_createList(const IFF_Long chunkSize, const IFF_ID contentsType)
{
    return (IFF_List*)IFF_createGroup(IFF_ID_LIST, chunkSize, contentsType, &listStructure, &IFF_listInterface);
}

IFF_List *IFF_createEmptyListWithContentsType(const IFF_ID contentsType)
{
    return IFF_createList(IFF_ID_SIZE /* We have a contentsType field so it is not 0 */, contentsType);
}

IFF_List *IFF_createEmptyList(void)
{
    return IFF_createEmptyListWithContentsType(IFF_ID_JJJJ);
}

IFF_Prop *IFF_removePropFromListByIndex(IFF_List *list, const unsigned int index)
{
    return (IFF_Prop*)IFF_removeChunkFromGroupStructureByIndex((IFF_Group*)list, IFF_ID_PROP, index);
}

IFF_Prop *IFF_updatePropInListByIndex(IFF_List *list, const unsigned int index, IFF_Prop *prop)
{
    return (IFF_Prop*)IFF_updateChunkInGroupStructureByIndex((IFF_Group*)list, index, (IFF_Chunk*)prop);
}

void IFF_addChunkToList(IFF_List *list, IFF_Chunk *chunk)
{
    IFF_addChunkToGroup((IFF_Group*)list, chunk);
}

void IFF_addChunkToListAndUpdateContentsType(IFF_List *list, IFF_Chunk *chunk)
{
    IFF_addChunkToCATAndUpdateContentsType((IFF_CAT*)list, chunk);
}

IFF_Chunk *IFF_removeChunkFromListByIndex(IFF_List *list, const unsigned int index)
{
    return IFF_removeChunkFromCATByIndex((IFF_CAT*)list, index);
}

IFF_Chunk *IFF_updateChunkInListByIndex(IFF_List *list, const unsigned int index, IFF_Chunk *chunk)
{
    return IFF_updateChunkInCATByIndex((IFF_CAT*)list, index, chunk);
}

IFF_Chunk *IFF_updateChunkInListAndUpdateContentsTypeByIndex(IFF_List *list, const unsigned int index, IFF_Chunk *chunk)
{
    return IFF_updateChunkInCATAndUpdateContentsTypeByIndex((IFF_CAT*)list, index, chunk);
}

static IFF_GroupStructure *lookupListStructure(const IFF_Registry *registry, const IFF_ID groupType)
{
    return &listStructure;
}

IFF_Chunk *IFF_parseListContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Registry *registry, IFF_ChunkInterface *chunkInterface, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return (IFF_Chunk*)IFF_parseGroupContents(file, lookupListStructure, chunkId, chunkSize, LIST_GROUPTYPENAME, registry, chunkInterface, attributePath, bytesProcessed, error);
}

IFF_Bool IFF_writeListContents(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_writeGroupContents(file, (const IFF_Group*)chunk, LIST_GROUPTYPENAME, attributePath, bytesProcessed, error);
}

IFF_QualityLevel IFF_checkListContents(const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_checkGroupContents((const IFF_Group*)chunk, LIST_GROUPTYPENAME, &IFF_checkId, &IFF_checkCATSubChunk, attributePath, printCheckMessage, data);
}

void IFF_clearListContents(IFF_Chunk *chunk)
{
    IFF_clearGroupContents((IFF_Group*)chunk);
}

void IFF_printListContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printGroupContents(file, (const IFF_Group*)chunk, indentLevel, LIST_GROUPTYPENAME);
}

IFF_Bool IFF_compareListContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareGroupContents((const IFF_Group*)chunk1, (const IFF_Group*)chunk2);
}

void IFF_recalculateListChunkSize(IFF_Chunk *chunk)
{
    IFF_recalculateGroupChunkSize((IFF_Group*)chunk);
}

static IFF_Prop *searchPropInList(const IFF_List *list, const IFF_ID formType)
{
    unsigned int i;

    for(i = list->propsLength; i-- > 0; )
    {
        if(list->props[i]->formType == formType)
            return list->props[i];
    }

    return NULL;
}

static IFF_List *searchEnclosingList(const IFF_Chunk *chunk)
{
    if(chunk == NULL)
        return NULL;
    else if(chunk->chunkId == IFF_ID_LIST)
        return (IFF_List*)chunk;
    else if(chunk->chunkId == IFF_ID_PROP)
    {
        if(chunk->parent == NULL || chunk->parent->parent == NULL)
            return NULL;
        else
            return searchEnclosingList(chunk->parent->parent);
    }
    else
        return searchEnclosingList(chunk->parent);
}

IFF_Prop *IFF_searchEnclosingProp(const IFF_Chunk *chunk, const IFF_ID formType)
{
    IFF_List *list = searchEnclosingList(chunk);

    if(list == NULL)
        return NULL; /* If the chunk is not (indirectly) embedded in a list, we have no PROPs at all */
    else
    {
        /* Try requesting the PROP chunk for the given form type */
        IFF_Prop *prop = searchPropInList(list, formType);

        if(prop == NULL)
            return IFF_searchEnclosingProp((const IFF_Chunk*)list, formType); /* If we can't find a PROP in the given list, try the parent list */
        else
            return prop;
    }
}
