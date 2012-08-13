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

#include "prop.h"
#include <stdlib.h>
#include <string.h>
#include "id.h"
#include "group.h"
#include "util.h"
#include "error.h"

#define PROP_CHUNKID "PROP"
#define PROP_GROUPTYPENAME "formType"

IFF_Prop *IFF_createProp(const char *formType)
{
    return (IFF_Prop*)IFF_createGroup(PROP_CHUNKID, formType);
}

void IFF_addToProp(IFF_Prop *prop, IFF_Chunk *chunk)
{
    IFF_addToForm((IFF_Form*)prop, chunk);
}

IFF_Prop *IFF_readProp(FILE *file, const IFF_Long chunkSize, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return (IFF_Prop*)IFF_readGroup(file, PROP_CHUNKID, chunkSize, PROP_GROUPTYPENAME, TRUE, extension, extensionLength);
}

int IFF_writeProp(FILE *file, const IFF_Prop *prop, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_writeForm(file, (IFF_Form*)prop, extension, extensionLength);
}

static int subChunkCheck(const IFF_Group *group, const IFF_Chunk *subChunk)
{
    if(IFF_compareId(subChunk->chunkId, "FORM") == 0 ||
       IFF_compareId(subChunk->chunkId, "LIST") == 0 ||
       IFF_compareId(subChunk->chunkId, "CAT ") == 0 ||
       IFF_compareId(subChunk->chunkId, "PROP") == 0)
    {
	IFF_error("ERROR: Element with chunk Id: '");
	IFF_errorId(subChunk->chunkId);
	IFF_error("' not allowed in PROP chunk!\n");

	return FALSE;
    }
    else
	return TRUE;
}

int IFF_checkProp(const IFF_Prop *prop, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_checkGroup((IFF_Group*)prop, &IFF_checkFormType, &subChunkCheck, prop->formType, extension, extensionLength);
}

void IFF_freeProp(IFF_Prop *prop, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_freeForm((IFF_Form*)prop, extension, extensionLength);
}

void IFF_printProp(const IFF_Prop *prop, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength)
{
    IFF_printForm((const IFF_Form *)prop, indentLevel, extension, extensionLength);
}

int IFF_compareProp(const IFF_Prop *prop1, const IFF_Prop *prop2, const IFF_Extension *extension, const unsigned int extensionLength)
{
    return IFF_compareForm((const IFF_Form*)prop1, (const IFF_Form*)prop2, extension, extensionLength);
}

void IFF_updatePropChunkSizes(IFF_Prop *prop)
{
    IFF_updateFormChunkSizes((IFF_Form*)prop);
}

IFF_Chunk *IFF_getChunkFromProp(const IFF_Prop *prop, const char *chunkId)
{
    return IFF_getDataChunkFromForm((IFF_Form*)prop, chunkId);
}
