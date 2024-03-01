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

#ifndef __IFF_FIELD_H
#define __IFF_FIELD_H

#include <stdio.h>
#include "chunk.h"

typedef enum
{
    IFF_FIELD_MORE = 0,
    IFF_FIELD_FAILURE = 1,
    IFF_FIELD_LAST = 2
}
IFF_FieldStatus;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Checks whether the given status is a success (not a failure)
 *
 * @param status Status field to check
 * @return TRUE if it is a success, else FALSE
 */
IFF_Bool IFF_deriveSuccess(const IFF_FieldStatus status);

IFF_FieldStatus IFF_readUByteField(FILE *file, IFF_UByte *value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_writeUByteField(FILE *file, const IFF_UByte value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_readUWordField(FILE *file, IFF_UWord *value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_writeUWordField(FILE *file, const IFF_UWord value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_readWordField(FILE *file, IFF_Word *value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_writeWordField(FILE *file, const IFF_Word value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_readULongField(FILE *file, IFF_ULong *value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_writeULongField(FILE *file, const IFF_ULong value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_readLongField(FILE *file, IFF_Long *value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_writeLongField(FILE *file, const IFF_Long value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_readIdField(FILE *file, IFF_ID *value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

IFF_FieldStatus IFF_writeIdField(FILE *file, const IFF_ID value, const IFF_Chunk *chunk, const char *attributeName, IFF_Long *bytesProcessed);

#ifdef __cplusplus
}
#endif

#endif
