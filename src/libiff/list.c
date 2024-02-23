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
#include "util.h"
#include "cat.h"
#include "error.h"

IFF_List *IFF_createList(const IFF_Long chunkSize, const IFF_ID contentsType)
{
    IFF_List *list = (IFF_List*)IFF_allocateChunk(IFF_ID_LIST, chunkSize, sizeof(IFF_List));

    if(list != NULL)
    {
        IFF_initGroup((IFF_Group*)list, contentsType);

        list->prop = NULL;
        list->propLength = 0;
    }

    return list;
}

IFF_List *IFF_createEmptyList(const IFF_ID contentsType)
{
    return IFF_createList(IFF_ID_SIZE /* We have a contentsType field so it is not 0 */, contentsType);
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

static IFF_Bool readListSubChunks(FILE *file, IFF_List *list, const IFF_ID contentsType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_Long bytesProcessed = IFF_ID_SIZE; /* The groupType field was already processed */

    while(bytesProcessed < list->chunkSize)
    {
        /* Read sub chunk */
        IFF_Chunk *chunk = IFF_readChunk(file, contentsType, extension, extensionLength);

        if(chunk == NULL)
            return FALSE;

        /* Add the PROP chunk or arbitrary sub chunk */
        if(chunk->chunkId == IFF_ID_PROP)
            addPropToList(list, (IFF_Prop*)chunk);
        else
            IFF_attachToGroup((IFF_Group*)list, chunk);

        /* Increase the bytes processed counter */
        bytesProcessed = IFF_incrementChunkSize(bytesProcessed, chunk);
    }

    if(bytesProcessed > list->chunkSize)
        IFF_error("WARNING: truncated LIST chunk! The size specifies: %d but the total amount of its sub chunks is: %d bytes. The parser may get confused!\n", list->chunkSize, bytesProcessed);

    return TRUE;
}

IFF_List *IFF_readList(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_ID contentsType;
    IFF_List *list;

    /* Read the contentsType id */
    if(!IFF_readId(file, &contentsType, IFF_ID_LIST, "contentsType"))
        return NULL;

    /* Create new list */
    list = IFF_createList(chunkSize, contentsType);

    if(list == NULL)
        return NULL;

    /* Read the remaining nested sub chunks */
    if(!readListSubChunks(file, list, contentsType, extension, extensionLength))
    {
        IFF_error("Error reading chunk in list!\n");
        IFF_freeChunk((IFF_Chunk*)list, 0, extension, extensionLength);
        return NULL;
    }

    /* Return the resulting list */
    return list;
}

static IFF_Bool writeListPropChunks(FILE *file, const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;

    for(i = 0; i < list->propLength; i++)
    {
        if(!IFF_writeChunk(file, (IFF_Chunk*)list->prop[i], 0, extension, extensionLength))
        {
            IFF_error("Error writing PROP!\n");
            return FALSE;
        }
    }

    return TRUE;
}

IFF_Bool IFF_writeList(FILE *file, const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(!IFF_writeId(file, list->contentsType, IFF_ID_LIST, "contentsType"))
    {
        IFF_error("Error writing contentsType!\n");
        return FALSE;
    }

    if(!writeListPropChunks(file, list, extension, extensionLength))
        return FALSE;

    if(!IFF_writeGroupSubChunks(file, (IFF_Group*)list, 0, extension, extensionLength))
        return FALSE;

    return TRUE;
}

static IFF_Long checkListPropChunks(const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_Long chunkSize = 0;
    unsigned int i;

    for(i = 0; i < list->propLength; i++)
    {
        IFF_Chunk *propChunk = (IFF_Chunk*)list->prop[i];

        if(!IFF_checkChunk(propChunk, 0, extension, extensionLength))
            return -1;

        chunkSize = IFF_incrementChunkSize(chunkSize, propChunk);
    }

    return chunkSize;
}

IFF_Bool IFF_checkList(const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_Long chunkSize = IFF_ID_SIZE;
    IFF_Long subChunkSize;

    if(!IFF_checkId(list->contentsType))
        return FALSE;

    /* Check validity of PROP chunks */
    if((subChunkSize = checkListPropChunks(list, extension, extensionLength)) == -1)
        return FALSE;

    chunkSize += subChunkSize;

    /* Check validity of other sub chunks */
    if((subChunkSize = IFF_checkGroupSubChunks((IFF_Group*)list, &IFF_checkCATSubChunk, 0, extension, extensionLength)) == -1)
        return FALSE;

    chunkSize += subChunkSize;

    /* Check whether the calculated chunk size matches the chunks' chunk size */
    if(!IFF_checkGroupChunkSize((IFF_Group*)list, chunkSize))
        return FALSE;

    return TRUE;
}

static void freeListPropChunks(IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;

    for(i = 0; i < list->propLength; i++)
        IFF_freeChunk((IFF_Chunk*)list->prop[i], 0, extension, extensionLength);
}

void IFF_freeList(IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_freeCAT((IFF_CAT*)list, extension, extensionLength);
    freeListPropChunks(list, extension, extensionLength);
    free(list->prop);
}

static void printListPropChunks(const IFF_List *list, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;

    IFF_printIndent(stdout, indentLevel, "prop = [\n");

    for(i = 0; i < list->propLength; i++)
        IFF_printChunk((IFF_Chunk*)list->prop[i], indentLevel + 1, 0, extension, extensionLength);

    IFF_printIndent(stdout, indentLevel, "];\n");
}

void IFF_printList(const IFF_List *list, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printGroupType("contentsType", list->contentsType, indentLevel);
    printListPropChunks(list, indentLevel, extension, extensionLength);
    IFF_printGroupSubChunks((const IFF_Group *)list, indentLevel, 0, extension, extensionLength);
}

static IFF_Bool compareListPropChunks(const IFF_List *list1, const IFF_List *list2, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(list1->propLength == list2->propLength)
    {
        unsigned int i;

        for(i = 0; i < list1->propLength; i++)
        {
            if(!IFF_compareProp(list1->prop[i], list2->prop[i], extension, extensionLength))
               return FALSE;
        }

        return TRUE;
    }
    else
        return FALSE;
}

IFF_Bool IFF_compareList(const IFF_List *list1, const IFF_List *list2, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(!compareListPropChunks(list1, list2, extension, extensionLength))
        return FALSE;

    if(!IFF_compareCAT((const IFF_CAT*)list1, (const IFF_CAT*)list2, extension, extensionLength))
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
