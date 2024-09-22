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

#ifndef __IFF_ARRAY_H
#define __IFF_ARRAY_H

#include <stdio.h>
#include "ifftypes.h"
#include "id.h"
#include "attributepath.h"
#include "error.h"
#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

IFF_Bool IFF_readValueArray(FILE *file, void *pointer, size_t elementSize, size_t length, IFF_AttributePath *attributePath, char *attributeName, char *description, const IFF_ID chunkId, IFF_IOError **error);

IFF_Bool IFF_readUByteArray(FILE *file, IFF_UByte *ubyteArray, size_t length, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

IFF_Bool IFF_writeValueArray(FILE *file, void *pointer, size_t elementSize, size_t length, IFF_AttributePath *attributePath, char *attributeName, char *description, const IFF_ID chunkId, IFF_IOError **error);

IFF_Bool IFF_writeUByteArray(FILE *file, IFF_UByte *ubyteArray, size_t length, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

void IFF_printUByteValueArray(FILE *file, const unsigned int indentLevel, IFF_UByte *array, const unsigned int arrayLength, unsigned int elementsPerRow, IFF_printValueFunction printByteValue);

void IFF_printText(FILE *file, const unsigned int indentLevel, IFF_UByte *array, const unsigned int arrayLength);

#ifdef __cplusplus
}
#endif

#endif
