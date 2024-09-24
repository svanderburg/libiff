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
#include "value.h"

typedef IFF_Bool (*IFF_readArrayFunction) (FILE *file, void *array, size_t arrayLength);
typedef IFF_Bool (*IFF_writeArrayFunction) (FILE *file, void *array, size_t arrayLength);
typedef void (*IFF_printArrayFunction) (FILE *file, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow);

#ifdef __cplusplus
extern "C" {
#endif

IFF_Bool IFF_readUByteArray(FILE *file, void *array, size_t arrayLength);

IFF_Bool IFF_writeUByteArray(FILE *file, void *array, size_t arrayLength);

void IFF_printUByteValueArray(FILE *file, const unsigned int indentLevel, IFF_UByte *array, const unsigned int arrayLength, const unsigned int elementsPerRow, IFF_printValueFunction printByteValue);

void IFF_printUByteHexArray(FILE *file, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow);

void IFF_printText(FILE *file, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow);

#ifdef __cplusplus
}
#endif

#endif
