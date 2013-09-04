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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a 4 character ID from a string.
 *
 * @param id A 4 character IFF id
 * @param idString String containing a 4 character ID
 */
void IFF_createId(IFF_ID id, const char *idString);

/**
 * Compares two IFF ids
 *
 * @param id1 An IFF ID to compare
 * @param id2 An IFF ID to compare
 * @return 0 if the IDs are equal, a value lower than 0 if id1 is lower than id2, a value higher than 1 if id1 is higher than id2
 */
int IFF_compareId(const IFF_ID id1, const char* id2);

/**
 * Reads an IFF id from a file
 *
 * @param file File descriptor of the file
 * @param id A 4 character IFF id
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the ID is succesfully read, else FALSE
 */
int IFF_readId(FILE *file, IFF_ID id, const IFF_ID chunkId, const char *attributeName);

/**
 * Writes an IFF id to a file
 *
 * @param file File descriptor of the file
 * @param id A 4 character IFF id
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the ID is succesfully written, else FALSE
 */
int IFF_writeId(FILE *file, const IFF_ID id, const IFF_ID chunkId, const char *attributeName);

/**
 * Checks whether an IFF id is valid
 *
 * @param id A 4 character IFF id
 * @return TRUE if the IFF id is valid, else FALSE
 */
int IFF_checkId(const IFF_ID id);

/**
 * Prints an IFF id
 *
 * @param id A 4 character IFF id
 */
void IFF_printId(const IFF_ID id);

#ifdef __cplusplus
}
#endif

#endif
