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
#include "attributepath.h"

typedef enum
{
    IFF_FIELD_MORE = 0,
    IFF_FIELD_FAILURE = 1,
    IFF_FIELD_LAST = 2
}
IFF_FieldStatus;

typedef void (*IFF_printValueFunction) (FILE *file, const void *value, const unsigned int indentLevel);

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

IFF_FieldStatus IFF_readUByteField(FILE *file, IFF_UByte *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeUByteField(FILE *file, const IFF_UByte value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_readUWordField(FILE *file, IFF_UWord *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeUWordField(FILE *file, const IFF_UWord value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_readWordField(FILE *file, IFF_Word *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeWordField(FILE *file, const IFF_Word value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_readULongField(FILE *file, IFF_ULong *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeULongField(FILE *file, const IFF_ULong value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_readLongField(FILE *file, IFF_Long *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeLongField(FILE *file, const IFF_Long value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_readIdField(FILE *file, IFF_ID *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeIdField(FILE *file, const IFF_ID value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printFirstField(FILE *file, const unsigned int indentLevel, const char *attributeName, const void *value, IFF_printValueFunction printValue);

void IFF_printField(FILE *file, const unsigned int indentLevel, const char *attributeName, const void *value, IFF_printValueFunction printValue);

void IFF_printCharField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_UByte value);

void IFF_printUByteField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_UByte value);

void IFF_printByteField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Byte value);

void IFF_printUWordField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_UWord value);

void IFF_printWordField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Word value);

void IFF_printULongField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_ULong value);

void IFF_printLongField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Long value);

void IFF_printIdField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_ID value);

void IFF_printChunkField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Chunk *chunk);

void IFF_printChunksArrayField(FILE *file, const unsigned int indentLevel, const char *attributeName, IFF_Chunk **chunks, unsigned int chunksLength);

#ifdef __cplusplus
}
#endif

#endif
