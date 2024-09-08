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
#include "array.h"

#define LIST_GROUPTYPENAME "contentsType"

IFF_ChunkInterface IFF_listInterface = {&IFF_parseListContents, &IFF_writeListContents, &IFF_checkListContents, &IFF_clearListContents, &IFF_printListContents, &IFF_compareListContents, &IFF_traverseCATChunkHierarchy, &IFF_recalculateListChunkSize};

static IFF_GroupMember listStructureMembers[] = {
    { IFF_ID_PROP, "props", IFF_GROUP_MEMBER_MULTIPLE }
};

static void initListContents(IFF_Group *group)
{
    IFF_List *list = (IFF_List*)group;
    list->props = NULL;
    list->propsLength = 0;
}

static IFF_GroupMember *getGroupMemberByChunkId(const IFF_GroupStructure *groupStructure, const IFF_ID chunkId)
{
    if(chunkId == IFF_ID_PROP)
        return &groupStructure->groupMembers[0];
    else
        return NULL;
}

static IFF_Chunk **getFieldPointerByChunkId(const IFF_Group *group, const IFF_ID chunkId)
{
    return NULL;
}

static IFF_Chunk ***getArrayFieldPointerByChunkId(IFF_Group *group, const IFF_ID chunkId, unsigned int **chunksLength)
{
    if(chunkId == IFF_ID_PROP)
    {
        IFF_List *list = (IFF_List*)group;
        *chunksLength = &list->propsLength;
        return (IFF_Chunk***)&list->props;
    }
    else
        return NULL;
}

static IFF_Chunk *getChunkFromList(const IFF_Group *group, const unsigned int index)
{
    return NULL;
}

static IFF_Chunk **getChunksFromList(const IFF_Group *group, const unsigned int index, unsigned int *chunksLength)
{
    if(index == 0)
    {
        const IFF_List *list = (const IFF_List*)group;
        *chunksLength = list->propsLength;
        return (IFF_Chunk**)list->props;
    }
    else
        return NULL;
}

IFF_GroupStructure listStructure = {
    sizeof(IFF_List),
    1,
    listStructureMembers,
    initListContents,
    getGroupMemberByChunkId,
    getFieldPointerByChunkId,
    getArrayFieldPointerByChunkId,
    getChunkFromList,
    getChunksFromList
};

IFF_List *IFF_createList(const IFF_Long chunkSize, const IFF_ID contentsType)
{
    return (IFF_List*)IFF_createGroup(IFF_ID_LIST, chunkSize, contentsType, &listStructure);
}

IFF_List *IFF_createEmptyListWithContentsType(const IFF_ID contentsType)
{
    return IFF_createList(IFF_ID_SIZE /* We have a contentsType field so it is not 0 */, contentsType);
}

IFF_List *IFF_createEmptyList(void)
{
    return IFF_createEmptyListWithContentsType(IFF_ID_JJJJ);
}

static IFF_Prop *detachPropFromList(IFF_List *list, const unsigned int index)
{
    IFF_Prop *obsoleteProp = NULL;
    IFF_removeElementFromPointerArrayByIndex((void**)list->props, index, &list->propsLength, (void**)&obsoleteProp);

    if(obsoleteProp != NULL)
        obsoleteProp->parent = NULL;

    return obsoleteProp;
}

IFF_Prop *IFF_removePropFromListByIndex(IFF_List *list, const unsigned int index)
{
    IFF_Prop *obsoleteProp = detachPropFromList(list, index);

    if(obsoleteProp != NULL)
        IFF_decreaseChunkSize((IFF_Chunk*)list, (IFF_Chunk*)obsoleteProp);

    return obsoleteProp;
}

static IFF_Prop *replacePropInList(IFF_List *list, const unsigned int index, IFF_Prop *prop)
{
    IFF_Prop *obsoleteProp = (IFF_Prop*)IFF_replaceElementInPointerArrayByIndex((void**)list->props, list->propsLength, index, (void*)prop);

    if(obsoleteProp != NULL)
        obsoleteProp->parent = NULL;

    return obsoleteProp;
}

IFF_Prop *IFF_updatePropInListByIndex(IFF_List *list, unsigned int index, IFF_Prop *prop)
{
    IFF_Prop *obsoleteProp = replacePropInList(list, index, prop);

    if(obsoleteProp != NULL)
        IFF_updateChunkSize((IFF_Chunk*)list, (IFF_Chunk*)obsoleteProp, (IFF_Chunk*)prop);

    return obsoleteProp;
}

void IFF_addChunkToList(IFF_List *list, IFF_Chunk *chunk)
{
    IFF_addChunkToGroup((IFF_Group*)list, &listStructure, chunk);
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

static IFF_GroupStructure *lookupListStructure(const IFF_ChunkRegistry *chunkRegistry, const IFF_ID groupType)
{
    return &listStructure;
}

IFF_Chunk *IFF_parseListContents(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return (IFF_Chunk*)IFF_parseGroupContents(file, lookupListStructure, chunkId, chunkSize, LIST_GROUPTYPENAME, chunkRegistry, attributePath, bytesProcessed, error);
}

IFF_Bool IFF_writeListContents(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_writeGroupContents(file, (const IFF_Group*)chunk, &listStructure, LIST_GROUPTYPENAME, chunkRegistry, attributePath, bytesProcessed, error);
}

IFF_QualityLevel IFF_checkListContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_checkGroupContents((const IFF_Group*)chunk, &listStructure, LIST_GROUPTYPENAME, &IFF_checkId, &IFF_checkCATSubChunk, chunkRegistry, attributePath, printCheckMessage, data);
}

void IFF_clearListContents(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_clearGroupContents((IFF_Group*)chunk, &listStructure, chunkRegistry);
}

void IFF_printListContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_printGroupContents(file, (const IFF_Group*)chunk, &listStructure, indentLevel, LIST_GROUPTYPENAME, chunkRegistry);
}

IFF_Bool IFF_compareListContents(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    return IFF_compareGroupContents((const IFF_Group*)chunk1, (const IFF_Group*)chunk2, &listStructure, chunkRegistry);
}

void IFF_recalculateListChunkSize(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    IFF_recalculateGroupChunkSize((IFF_Group*)chunk, &listStructure);
}

IFF_Prop *IFF_searchPropInList(const IFF_List *list, const IFF_ID formType)
{
    unsigned int i;

    for(i = list->propsLength; i-- > 0; )
    {
        if(list->props[i]->formType == formType)
            return list->props[i];
    }

    return NULL;
}
