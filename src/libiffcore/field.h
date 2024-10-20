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

typedef struct IFF_Type IFF_Type;
typedef struct IFF_Field IFF_Field;

#include <stdio.h>
#include "id.h"
#include "chunk.h"
#include "attributepath.h"
#include "value.h"

typedef enum
{
    IFF_FIELD_MORE = 0,
    IFF_FIELD_FAILURE = 1,
    IFF_FIELD_LAST = 2
}
IFF_FieldStatus;

typedef IFF_FieldStatus (*IFF_readFieldFunction) (FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);
typedef IFF_FieldStatus (*IFF_writeFieldFunction) (FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);
typedef void (*IFF_clearFieldFunction) (void *value);
typedef IFF_Bool (*IFF_compareFieldFunction) (const void *value1, const void *value2);
typedef void (*IFF_printFieldFunction) (FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

typedef IFF_FieldStatus (*IFF_readArrayFieldFunction) (FILE *file, const IFF_Field *field, void *array, size_t arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);
typedef IFF_FieldStatus (*IFF_writeArrayFieldFunction) (FILE *file, const IFF_Field *field, void *array, size_t arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);
typedef IFF_Bool (*IFF_compareArrayFieldFunction) (const void *array1, const unsigned int array1Length, const void *array2, const unsigned int array2Length);
typedef void (*IFF_clearArrayFieldFunction) (void *array, const unsigned int arrayLength);
typedef void (*IFF_printArrayFieldFunction) (FILE *file, const unsigned int indentLevel, const IFF_Field *field, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow);

struct IFF_Type
{
    char *description;
    unsigned int elementSize;

    IFF_readFieldFunction readField;
    IFF_writeFieldFunction writeField;
    IFF_clearFieldFunction clearField;
    IFF_compareFieldFunction compareField;
    IFF_printFieldFunction printField;

    IFF_readArrayFieldFunction readArrayField;
    IFF_writeArrayFieldFunction writeArrayField;
    IFF_clearArrayFieldFunction clearArrayField;
    IFF_compareArrayFieldFunction compareArrayField;
    IFF_printArrayFieldFunction printArrayField;
};

typedef enum
{
    IFF_CARDINALITY_SINGLE = 0,
    IFF_CARDINALITY_MULTIPLE = 1
}
IFF_Cardinality;

struct IFF_Field
{
    char *attributeName;
    IFF_Type *type;
    IFF_Cardinality cardinality;
};

extern IFF_Type IFF_Type_UByte;
extern IFF_Type IFF_Type_Char;
extern IFF_Type IFF_Type_UWord;
extern IFF_Type IFF_Type_Word;
extern IFF_Type IFF_Type_ULong;
extern IFF_Type IFF_Type_Long;
extern IFF_Type IFF_Type_ID;

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

IFF_Bool IFF_readChunkIdField(FILE *file, void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error);

IFF_Bool IFF_writeChunkIdField(FILE *file, const void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error);

IFF_Bool IFF_readChunkSizeField(FILE *file, void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error);

IFF_Bool IFF_writeChunkSizeField(FILE *file, const void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error);

void IFF_printFirstField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value, IFF_printValueFunction printValue);

void IFF_printField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value, IFF_printValueFunction printValue);

IFF_FieldStatus IFF_readUByteField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeUByteField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printUByteField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

void IFF_printCharField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

void IFF_printByteField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

IFF_FieldStatus IFF_readUByteArrayField(FILE *file, const IFF_Field *field, void *array, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeUByteArrayField(FILE *file, const IFF_Field *field, void *array, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printUByteHexArrayField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow);

void IFF_printTextField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow);

IFF_FieldStatus IFF_readUWordField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeUWordField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printUWordField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

IFF_FieldStatus IFF_readWordField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeWordField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printWordField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

IFF_FieldStatus IFF_readULongField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeULongField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printULongField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

IFF_FieldStatus IFF_readLongField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeLongField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printLongField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

IFF_FieldStatus IFF_readIdField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus IFF_writeIdField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

void IFF_printIdField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value);

void IFF_printChunkField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Chunk *chunk);

void IFF_printChunksArrayField(FILE *file, const unsigned int indentLevel, const char *attributeName, IFF_Chunk **chunks, unsigned int chunksLength);

#ifdef __cplusplus
}
#endif

#endif
