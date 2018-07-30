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

#define CHUNKID "LIST"

IFF_List *IFF_createList(const char *contentsType)
{
    IFF_List *list = (IFF_List*)IFF_allocateChunk(CHUNKID, sizeof(IFF_List));
    
    if(list != NULL)
    {
	IFF_initGroup((IFF_Group*)list, contentsType);
	
	list->prop = NULL;
	list->propLength = 0;
    }
    
    return list;
}

void IFF_addPropToList(IFF_List *list, IFF_Prop *prop)
{
    list->prop = (IFF_Prop**)realloc(list->prop, (list->propLength + 1) * sizeof(IFF_Prop*));
    list->prop[list->propLength] = prop;
    list->propLength++;
    list->chunkSize = IFF_incrementChunkSize(list->chunkSize, (IFF_Chunk*)prop);
    
    prop->parent = (IFF_Group*)list;
}

void IFF_addToList(IFF_List *list, IFF_Chunk *chunk)
{
    IFF_addToCAT((IFF_CAT*)list, chunk);
}

IFF_List *IFF_readList(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_ID contentsType;
    IFF_List *list;
    
    /* Read the contentsType id */
    if(!IFF_readId(file, contentsType, CHUNKID, "contentsType"))
	return NULL;

    /* Create new list */
    list = IFF_createList(contentsType);
    
    /* Read the remaining nested sub chunks */
    
    while(list->chunkSize < chunkSize)
    {
	/* Read sub chunk */
	IFF_Chunk *chunk = IFF_readChunk(file, NULL, extension, extensionLength);
	
	if(chunk == NULL)
	{
	    IFF_error("Error reading chunk in list!\n");
	    IFF_freeChunk((IFF_Chunk*)list, NULL, extension, extensionLength);
	    return NULL;
	}
	
	/* Add the prop or chunk */
	if(IFF_compareId(chunk->chunkId, "PROP") == 0)
	    IFF_addPropToList(list, (IFF_Prop*)chunk);
	else
	    IFF_addToList(list, chunk);
    }
    
    /* Set the chunk size to what we have read */
    list->chunkSize = chunkSize;
    
    /* Return the resulting list */
    return list;
}

int IFF_writeList(FILE *file, const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;
    
    if(!IFF_writeId(file, list->contentsType, CHUNKID, "contentsType"))
    {
	IFF_error("Error writing contentsType!\n");
	return FALSE;
    }
    
    for(i = 0; i < list->propLength; i++)
    {
	if(!IFF_writeChunk(file, (IFF_Chunk*)list->prop[i], NULL, extension, extensionLength))
	{
	    IFF_error("Error writing PROP!\n");
	    return FALSE;
	}
    }
    
    if(!IFF_writeGroupSubChunks(file, (IFF_Group*)list, NULL, extension, extensionLength))
	return FALSE;
	
    return TRUE;
}

int IFF_checkList(const IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;
    IFF_Long chunkSize = IFF_ID_SIZE;
    IFF_Long subChunkSize;
    
    if(!IFF_checkId(list->contentsType))
	return FALSE;

    /* Check validity of PROP chunks */
    
    for(i = 0; i < list->propLength; i++)
    {
	IFF_Chunk *propChunk = (IFF_Chunk*)list->prop[i];
	
	if(!IFF_checkChunk(propChunk, NULL, extension, extensionLength))
	    return FALSE;
	
	chunkSize = IFF_incrementChunkSize(chunkSize, propChunk);
    }
    
    /* Check validity of other sub chunks */
    if((subChunkSize = IFF_checkGroupSubChunks((IFF_Group*)list, &IFF_checkCATSubChunk, NULL, extension, extensionLength)) == -1)
	return FALSE;
    
    chunkSize += subChunkSize;
    
    /* Check whether the calculated chunk size matches the chunks' chunk size */
    if(!IFF_checkGroupChunkSize((IFF_Group*)list, chunkSize))
	return FALSE;
    
    return TRUE;
}

void IFF_freeList(IFF_List *list, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;
    
    IFF_freeCAT((IFF_CAT*)list, extension, extensionLength);
    
    for(i = 0; i < list->propLength; i++)
	IFF_freeChunk((IFF_Chunk*)list->prop[i], NULL, extension, extensionLength);

    free(list->prop);
}

void IFF_printList(const IFF_List *list, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength)
{
    unsigned int i;
    
    IFF_printGroupType("contentsType", list->contentsType, indentLevel);
    
    IFF_printIndent(stdout, indentLevel, "prop = [\n");
    
    /* Print shared properties */
    for(i = 0; i < list->propLength; i++)
	IFF_printChunk((IFF_Chunk*)list->prop[i], indentLevel + 1, NULL, extension, extensionLength);
    
    IFF_printIndent(stdout, indentLevel, "];\n");
    
    /* Print sub chunks */
    IFF_printGroupSubChunks((const IFF_Group *)list, indentLevel, NULL, extension, extensionLength);
}

int IFF_compareList(const IFF_List *list1, const IFF_List *list2, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(list1->propLength == list2->propLength)
    {
	unsigned int i;
	
	for(i = 0; i < list1->propLength; i++)
	{
	    if(!IFF_compareProp(list1->prop[i], list2->prop[i], extension, extensionLength))
		return FALSE;
	}
	
	return IFF_compareCAT((const IFF_CAT*)list1, (const IFF_CAT*)list2, extension, extensionLength);
    }
    else
	return FALSE;
}

IFF_Form **IFF_searchFormsInList(IFF_List *list, const char **formTypes, const unsigned int formTypesLength, unsigned int *formsLength)
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

IFF_Prop *IFF_getPropFromList(const IFF_List *list, const char *formType)
{
    unsigned int i;
    
    for(i = 0; i < list->propLength; i++)
    {
	if(IFF_compareId(list->prop[i]->formType, formType) == 0)
	    return list->prop[i];
    }
    
    return NULL;
}
