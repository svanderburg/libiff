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
#include "extension.h"
#include "io.h"
#include "id.h"
#include "util.h"
#include "form.h"
#include "cat.h"
#include "list.h"
#include "prop.h"
#include "rawchunk.h"
#include "util.h"
#include "error.h"

IFF_Chunk *IFF_allocateChunk(const char *chunkId, const size_t chunkSize)
{
    IFF_Chunk *chunk = (IFF_Chunk*)malloc(chunkSize);
    
    chunk->parent = NULL;
    IFF_createId(chunk->chunkId, chunkId);
    chunk->chunkSize = 0;
    
    return chunk;
}

IFF_Chunk *IFF_readChunk(FILE *file, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_ID chunkId;
    IFF_Long chunkSize;
    
    /* Read chunk id */
    if(!IFF_readId(file, chunkId, "", chunkId))
	return NULL;
    
    /* Read chunk size */
    if(!IFF_readLong(file, &chunkSize, chunkId, "chunkSize"))
	return NULL;

    /* Read remaining bytes (procedure depends on chunk id type) */
    
    if(IFF_compareId(chunkId, "FORM") == 0)
	return (IFF_Chunk*)IFF_readForm(file, chunkSize, extension, extensionLength);
    else if(IFF_compareId(chunkId, "CAT ") == 0)
	return (IFF_Chunk*)IFF_readCAT(file, chunkSize, extension, extensionLength);
    else if(IFF_compareId(chunkId, "LIST") == 0)
	return (IFF_Chunk*)IFF_readList(file, chunkSize, extension, extensionLength);
    else if(IFF_compareId(chunkId, "PROP") == 0)
	return (IFF_Chunk*)IFF_readProp(file, chunkSize, extension, extensionLength);
    else
    {
	const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunkId, extension, extensionLength);
	
	if(formExtension == NULL)
	    return (IFF_Chunk*)IFF_readRawChunk(file, chunkId, chunkSize);
	else
	    return formExtension->readChunk(file, chunkSize);
    }
}

int IFF_writeChunk(FILE *file, const IFF_Chunk *chunk, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(!IFF_writeId(file, chunk->chunkId, chunk->chunkId, "chunkId"))
	return FALSE;
    
    if(!IFF_writeLong(file, chunk->chunkSize, chunk->chunkId, "chunkSize"))
	return FALSE;
    
    if(IFF_compareId(chunk->chunkId, "FORM") == 0)
    {
	if(!IFF_writeForm(file, (IFF_Form*)chunk, extension, extensionLength))
	    return FALSE;
    }
    else if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
    {
	if(!IFF_writeCAT(file, (IFF_CAT*)chunk, extension, extensionLength))
	    return FALSE;
    }
    else if(IFF_compareId(chunk->chunkId, "LIST") == 0)
    {
	if(!IFF_writeList(file, (IFF_List*)chunk, extension, extensionLength))
	    return FALSE;
    }
    else if(IFF_compareId(chunk->chunkId, "PROP") == 0)
    {
	if(!IFF_writeProp(file, (IFF_Prop*)chunk, extension, extensionLength))
	    return FALSE;
    }
    else
    {
	const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);
	    
	if(formExtension == NULL)
	    return IFF_writeRawChunk(file, (IFF_RawChunk*)chunk);
	else
	    return formExtension->writeChunk(file, chunk);
    }
    
    return TRUE;
}

int IFF_checkChunk(const IFF_Chunk *chunk, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(!IFF_checkId(chunk->chunkId))
	return FALSE;
    else
    {
	if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	    return IFF_checkForm((const IFF_Form*)chunk, extension, extensionLength);
	else if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
	    return IFF_checkCAT((const IFF_CAT*)chunk, extension, extensionLength);
	else if(IFF_compareId(chunk->chunkId, "LIST") == 0)
	    return IFF_checkList((const IFF_List*)chunk, extension, extensionLength);
	else if(IFF_compareId(chunk->chunkId, "PROP") == 0)
	    return IFF_checkProp((const IFF_Prop*)chunk, extension, extensionLength);
	else
	{
	    const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);
		
	    if(formExtension == NULL)
		return TRUE;
	    else
	        return formExtension->checkChunk(chunk);
	}
    }
}

void IFF_freeChunk(IFF_Chunk *chunk, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    /* Free nested sub chunks */
    if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	IFF_freeForm((IFF_Form*)chunk, extension, extensionLength);
    else if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
	IFF_freeCAT((IFF_CAT*)chunk, extension, extensionLength);
    else if(IFF_compareId(chunk->chunkId, "LIST") == 0)
	IFF_freeList((IFF_List*)chunk, extension, extensionLength);
    else if(IFF_compareId(chunk->chunkId, "PROP") == 0)
	IFF_freeProp((IFF_Prop*)chunk, extension, extensionLength);
    else
    {
	const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);
	    
	if(formExtension == NULL)
	    IFF_freeRawChunk((IFF_RawChunk*)chunk);
	else
	    formExtension->freeChunk(chunk);
    }
    
    /* Free the chunk itself */
    free(chunk);
}

void IFF_printChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printIndent(stdout, indentLevel, "'");
    IFF_printId(chunk->chunkId);
    printf("' = {\n");
    
    IFF_printIndent(stdout, indentLevel + 1, "chunkSize = %d;\n", chunk->chunkSize);
    
    if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	IFF_printForm((const IFF_Form*)chunk, indentLevel + 1, extension, extensionLength);
    else if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
	IFF_printCAT((const IFF_CAT*)chunk, indentLevel + 1, extension, extensionLength);
    else if(IFF_compareId(chunk->chunkId, "LIST") == 0)
	IFF_printList((const IFF_List*)chunk, indentLevel + 1, extension, extensionLength);
    else if(IFF_compareId(chunk->chunkId, "PROP") == 0)
	IFF_printProp((const IFF_Prop*)chunk, indentLevel + 1, extension, extensionLength);
    else
    {
	const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);
	
	if(formExtension == NULL)
	    IFF_printRawChunk((IFF_RawChunk*)chunk, indentLevel + 1);
	else
	    formExtension->printChunk(chunk, indentLevel + 1);
    }
    
    IFF_printIndent(stdout, indentLevel, "}\n\n");
}

int IFF_compareChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const char *formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    if(IFF_compareId(chunk1->chunkId, chunk2->chunkId) == 0)
    {
	if(chunk1->chunkSize == chunk2->chunkSize)
	{
	    if(IFF_compareId(chunk1->chunkId, "FORM") == 0)
		return IFF_compareForm((const IFF_Form*)chunk1, (const IFF_Form*)chunk2, extension, extensionLength);
	    else if(IFF_compareId(chunk1->chunkId, "CAT ") == 0)
		return IFF_compareCAT((const IFF_CAT*)chunk1, (const IFF_CAT*)chunk2, extension, extensionLength);
	    else if(IFF_compareId(chunk1->chunkId, "LIST") == 0)
		return IFF_compareList((const IFF_List*)chunk1, (const IFF_List*)chunk2, extension, extensionLength);
	    else if(IFF_compareId(chunk1->chunkId, "PROP") == 0)
		return IFF_compareProp((const IFF_Prop*)chunk1, (const IFF_Prop*)chunk2, extension, extensionLength);
	    else
	    {
		const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk1->chunkId, extension, extensionLength);
		
		if(formExtension == NULL)
		    return IFF_compareRawChunk((const IFF_RawChunk*)chunk1, (const IFF_RawChunk*)chunk2);
		else
		    return formExtension->compareChunk(chunk1, chunk2);
	    }
	}
	else
	    return FALSE;
    }
    else
	return FALSE;
}

IFF_Form **IFF_searchForms(IFF_Chunk *chunk, const char *formType, unsigned int *formsLength)
{
    if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	return IFF_searchFormsInForm((IFF_Form*)chunk, formType, formsLength);
    else if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
	return IFF_searchFormsInCAT((IFF_CAT*)chunk, formType, formsLength);
    else if(IFF_compareId(chunk->chunkId, "LIST") == 0)
	return IFF_searchFormsInList((IFF_List*)chunk, formType, formsLength);
    else
    {
	*formsLength = 0;
	return NULL;
    }
}

IFF_Long IFF_incrementChunkSize(const IFF_Long chunkSize, const IFF_Chunk *chunk)
{
    IFF_Long returnValue = chunkSize + IFF_ID_SIZE + sizeof(IFF_Long) + chunk->chunkSize;
    
    /* If the size of the nested chunk size is odd, we have to count the padding byte as well */
    if(chunk->chunkSize % 2 != 0)
        returnValue++;
        
    return returnValue;
}

void IFF_updateChunkSizes(IFF_Chunk *chunk)
{
    /* Check whether the given chunk is a group chunk and update the sizes */
    if(IFF_compareId(chunk->chunkId, "FORM") == 0)
	IFF_updateFormChunkSizes((IFF_Form*)chunk);
    else if(IFF_compareId(chunk->chunkId, "PROP") == 0)
	IFF_updatePropChunkSizes((IFF_Prop*)chunk);
    else if(IFF_compareId(chunk->chunkId, "CAT ") == 0)
	IFF_updateCATChunkSizes((IFF_CAT*)chunk);
    else if(IFF_compareId(chunk->chunkId, "LIST") == 0)
	IFF_updateListChunkSizes((IFF_List*)chunk);
    
    /* If the given type has a parent, recursively update these as well */
    if(chunk->parent != NULL)
	IFF_updateChunkSizes((IFF_Chunk*)chunk->parent);
}
