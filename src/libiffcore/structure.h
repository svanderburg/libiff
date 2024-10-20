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

#ifndef __IFF_STRUCTURE_H
#define __IFF_STRUCTURE_H

#include "ifftypes.h"
#include "attributepath.h"
#include "error.h"
#include "field.h"

typedef void *(*IFF_getFieldPointerFunction) (void *object, const unsigned int index);

typedef void **(*IFF_getArrayFieldPointerFunction) (void *object, const unsigned int index, unsigned int *arrayLength);

typedef struct IFF_Structure IFF_Structure;

struct IFF_Structure
{
    unsigned int fieldsLength;
    IFF_Field *fields;

    IFF_getFieldPointerFunction getFieldPointer;
    IFF_getArrayFieldPointerFunction getArrayFieldPointer;
};

#ifdef __cplusplus
extern "C" {
#endif

IFF_Bool IFF_readStructure(FILE *file, const IFF_Structure *structure, void *object, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_Bool IFF_writeStructure(FILE *file, const IFF_Structure *structure, void *object, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_clearStructure(const IFF_Structure *structure, void *object);

IFF_Bool IFF_compareStructure(const IFF_Structure *structure, void *object1, void *object2);

void IFF_printStructureContents(FILE *file, const unsigned int indentLevel, const IFF_Structure *structure, void *object);

#ifdef __cplusplus
}
#endif

#endif
