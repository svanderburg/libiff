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

#ifndef __IFF_PROP_H
#define __IFF_PROP_H

#define IFF_ID_PROP IFF_MAKEID('P', 'R', 'O', 'P')

typedef struct IFF_Form IFF_Prop;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "form.h"
#include "error.h"

extern IFF_ChunkInterface IFF_propInterface;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new PROP chunk instance with a given chunk size and form type.
 * The resulting chunk must be freed by using IFF_free().
 *
 * @param chunkSize Size of the chunk data
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Prop *IFF_createProp(const IFF_Long chunkSize, const IFF_ID formType);

/**
 * Creates a new empty PROP chunk instance with a given form type.
 * The resulting chunk must be freed by using IFF_free().
 * Sub chunks can be added with the IFF_addToProp() function.
 *
 * @param formType Form type describing the purpose of the sub chunks.
 * @return FORM chunk or NULL, if the memory for the struct can't be allocated
 */
IFF_Prop *IFF_createEmptyProp(const IFF_ID formType);

/**
 * Adds a chunk to the body of the given PROP. This function also increments the
 * chunk size and chunk length counter.
 *
 * @param prop An instance of a PROP chunk
 * @param chunk A data chunk
 */
void IFF_addToProp(IFF_Prop *prop, IFF_Chunk *chunk);

/**
 * Checks whether the PROP chunk and its sub chunks conform to the IFF specification.
 *
 * @param chunk An instance of a PROP chunk
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 * @return TRUE if the PROP is valid, else FALSE.
 */
IFF_QualityLevel IFF_checkPropContents(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Recalculates the chunk size of the given PROP chunk.
 *
 * @param prop An instance of a PROP chunk
 */
void IFF_updatePropChunkSizes(IFF_Prop *prop);

/**
 * Retrieves the chunk with the given chunk ID from the given PROP chunk. 
 *
 * @param prop An instance of a PROP chunk
 * @param chunkId An arbitrary chunk ID
 * @return The chunk with the given chunk ID, or NULL if the chunk can't be found
 */
IFF_Chunk *IFF_getChunkFromProp(const IFF_Prop *prop, const IFF_ID chunkId);

#ifdef __cplusplus
}
#endif

#endif
