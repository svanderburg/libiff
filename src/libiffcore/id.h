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

#ifndef __IFF_ID_H
#define __IFF_ID_H

#include <stdio.h>
#include "ifftypes.h"
#include "attributepath.h"
#include "error.h"

#define IFF_MAKEID(a, b, c, d) ( (a) << 24 | (b) << 16 | (c) << 8 | (d) )

typedef char IFF_ID2[4];

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an IFF id from a file
 *
 * @param file File descriptor of the file
 * @param id A 4 character IFF id
 * @return TRUE if the ID is succesfully read, else FALSE
 */
IFF_Bool IFF_readId(FILE *file, void *value);

/**
 * Writes an IFF id to a file
 *
 * @param file File descriptor of the file
 * @param id A 4 character IFF id
 * @return TRUE if the ID is succesfully written, else FALSE
 */
IFF_Bool IFF_writeId(FILE *file, const void *value);

/**
 * Converts a given ID to a string representation
 *
 * @param id ID to convert
 * @param id2 A 4-character array containing its string representation
 */
void IFF_idToString(const IFF_ID id, IFF_ID2 id2);

/**
 * Checks whether an IFF id is valid
 *
 * @param id A 4 character IFF id
 * @return TRUE if the IFF id is valid, else FALSE
 */
IFF_QualityLevel IFF_checkId(const IFF_ID id, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId);

void IFF_printIdValue(FILE *file, const void *value, const unsigned int indentLevel);

#ifdef __cplusplus
}
#endif

#endif
