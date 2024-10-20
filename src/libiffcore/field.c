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

#include "field.h"
#include "util.h"
#include "array.h"
#include "chunksarray.h"

IFF_Bool IFF_deriveSuccess(const IFF_FieldStatus status)
{
    return (status != IFF_FIELD_FAILURE);
}

static IFF_Bool fieldDoesNotFitInChunk(const size_t fieldSize, const IFF_Long chunkSize, const IFF_Long bytesProcessed)
{
    return bytesProcessed > chunkSize - fieldSize;
}

static void increaseBytesProcessed(IFF_Long *bytesProcessed, const size_t fieldSize)
{
    *bytesProcessed = *bytesProcessed + fieldSize;
}

static IFF_Bool readValueHeaderField(FILE *file, IFF_readValueFunction readValue, void *value, size_t fieldSize, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_IOError **error)
{
    if(readValue(file, value))
        return TRUE;
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, attributeName, description, chunkId);
        return FALSE;
    }
}

static IFF_Bool writeValueHeaderField(FILE *file, IFF_writeValueFunction writeValue, const void *value, size_t fieldSize, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_IOError **error)
{
    if(writeValue(file, value))
        return TRUE;
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, attributeName, description, chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_readChunkIdField(FILE *file, void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error)
{
    return readValueHeaderField(file, IFF_readId, value, sizeof(IFF_ID), chunkId, attributePath, attributeName, "ID", error);
}

IFF_Bool IFF_writeChunkIdField(FILE *file, const void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error)
{
    return writeValueHeaderField(file, IFF_writeId, value, sizeof(IFF_ID), chunkId, attributePath, attributeName, "ID", error);
}

IFF_Bool IFF_readChunkSizeField(FILE *file, void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error)
{
    return readValueHeaderField(file, IFF_readLong, value, sizeof(IFF_Long), chunkId, attributePath, attributeName, "LONG", error);
}

IFF_Bool IFF_writeChunkSizeField(FILE *file, const void *value, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, IFF_IOError **error)
{
    return writeValueHeaderField(file, IFF_writeLong, value, sizeof(IFF_Long), chunkId, attributePath, attributeName, "LONG", error);
}

static IFF_FieldStatus readValueField(FILE *file, const IFF_Field *field, IFF_readValueFunction readValue, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    if(fieldDoesNotFitInChunk(field->type->elementSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(readValue(file, value))
    {
        increaseBytesProcessed(bytesProcessed, field->type->elementSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, field->type->elementSize, attributePath, field->attributeName, field->type->description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

static IFF_FieldStatus writeValueField(FILE *file, const IFF_Field *field, IFF_writeValueFunction writeValue, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    if(fieldDoesNotFitInChunk(field->type->elementSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(writeValue(file, value))
    {
        increaseBytesProcessed(bytesProcessed, field->type->elementSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, field->type->elementSize, attributePath, field->attributeName, field->type->description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

static void printAttributeName(FILE *file, const unsigned int indentLevel, const char *attributeName)
{
    IFF_printIndent(file, indentLevel, ".%s = ", attributeName);
}

void IFF_printFirstField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value, IFF_printValueFunction printValue)
{
    printAttributeName(file, indentLevel, field->attributeName);
    printValue(file, value, indentLevel);
}

void IFF_printField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value, IFF_printValueFunction printValue)
{
    fputs(",\n", file);
    IFF_printFirstField(file, indentLevel, field, value, printValue);
}

static IFF_FieldStatus readArrayField(FILE *file, const IFF_Field *field, IFF_readArrayFunction readArray, void *array, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    size_t fieldSize = length * field->type->elementSize;

    if(fieldDoesNotFitInChunk(fieldSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(readArray(file, array, length))
    {
        increaseBytesProcessed(bytesProcessed, fieldSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, field->attributeName, field->type->description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

static IFF_FieldStatus writeArrayField(FILE *file, const IFF_Field *field, IFF_writeArrayFunction writeArray, void *array, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    size_t fieldSize = length * field->type->elementSize;

    if(fieldDoesNotFitInChunk(fieldSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(writeArray(file, array, length))
    {
        increaseBytesProcessed(bytesProcessed, fieldSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, field->attributeName, field->type->description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

static void printArrayField(FILE *file, const IFF_Field *field, IFF_printArrayFunction printArray, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    fputs(",\n", file);
    printAttributeName(file, indentLevel, field->attributeName);
    printArray(file, indentLevel, array, arrayLength, elementsPerRow);
}

IFF_Type IFF_Type_UByte = {
    "UBYTE",
    sizeof(IFF_UByte),
    IFF_readUByteField,
    IFF_writeUByteField,
    IFF_clearValue,
    IFF_compareUByte,
    IFF_printUByteField,
    IFF_readUByteArrayField,
    IFF_writeUByteArrayField,
    IFF_clearValueArray,
    IFF_compareUByteArray,
    IFF_printUByteHexArrayField
};

IFF_Type IFF_Type_Char = {
    "UBYTE",
    sizeof(IFF_UByte),
    IFF_readUByteField,
    IFF_writeUByteField,
    IFF_clearValue,
    IFF_compareUByte,
    IFF_printCharField,
    IFF_readUByteArrayField,
    IFF_writeUByteArrayField,
    IFF_clearValueArray,
    IFF_compareUByteArray,
    IFF_printTextField
};

IFF_FieldStatus IFF_readUByteField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, field, IFF_readUByte, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUByteField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, field, IFF_writeUByte, value, chunk, attributePath, bytesProcessed, error);
}

void IFF_printUByteField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printUByteValue);
}

void IFF_printCharField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printCharValue);
}

void IFF_printByteField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printByteValue);
}

IFF_FieldStatus IFF_readUByteArrayField(FILE *file, const IFF_Field *field, void *array, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readArrayField(file, field, IFF_readUByteArray, array, length, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUByteArrayField(FILE *file, const IFF_Field *field, void *array, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeArrayField(file, field, IFF_writeUByteArray, array, length, chunk, attributePath, bytesProcessed, error);
}

void IFF_printUByteHexArrayField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    printArrayField(file, field, IFF_printUByteHexArray, indentLevel, array, arrayLength, elementsPerRow);
}

void IFF_printTextField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    printArrayField(file, field, IFF_printText, indentLevel, array, arrayLength, elementsPerRow);
}

IFF_Type IFF_Type_UWord = {
    "UWORD",
    sizeof(IFF_UWord),
    IFF_readUWordField,
    IFF_writeUWordField,
    IFF_clearValue,
    IFF_compareUWord,
    IFF_printUWordField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

IFF_FieldStatus IFF_readUWordField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, field, IFF_readUWord, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUWordField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, field, IFF_writeUWord, value, chunk, attributePath, bytesProcessed, error);
}

void IFF_printUWordField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printUWordValue);
}

IFF_Type IFF_Type_Word = {
    "WORD",
    sizeof(IFF_Word),
    IFF_readWordField,
    IFF_writeWordField,
    IFF_clearValue,
    IFF_compareWord,
    IFF_printWordField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

IFF_FieldStatus IFF_readWordField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, field, IFF_readWord, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeWordField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, field, IFF_writeWord, value, chunk, attributePath, bytesProcessed, error);
}

void IFF_printWordField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printWordValue);
}

IFF_Type IFF_Type_ULong = {
    "ULONG",
    sizeof(IFF_ULong),
    IFF_readULongField,
    IFF_writeULongField,
    IFF_clearValue,
    IFF_compareULong,
    IFF_printULongField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

IFF_FieldStatus IFF_readULongField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, field, IFF_readULong, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeULongField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, field, IFF_writeULong, value, chunk, attributePath, bytesProcessed, error);
}

void IFF_printULongField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printULongValue);
}

IFF_Type IFF_Type_Long = {
    "LONG",
    sizeof(IFF_Long),
    IFF_readLongField,
    IFF_writeLongField,
    IFF_clearValue,
    IFF_compareLong,
    IFF_printLongField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

IFF_FieldStatus IFF_readLongField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, field, IFF_readLong, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeLongField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, field, IFF_writeLong, value, chunk, attributePath, bytesProcessed, error);
}

void IFF_printLongField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printLongValue);
}

IFF_Type IFF_Type_ID = {
    "ID",
    sizeof(IFF_ID),
    IFF_readIdField,
    IFF_writeIdField,
    IFF_clearValue,
    IFF_compareId,
    IFF_printIdField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

IFF_FieldStatus IFF_readIdField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, field, IFF_readId, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeIdField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, field, IFF_writeId, value, chunk, attributePath, bytesProcessed, error);
}

void IFF_printIdField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printField(file, indentLevel, field, value, IFF_printIdValue);
}

void IFF_printChunkField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Chunk *chunk)
{
    if(chunk != NULL)
    {
        fputs(",\n", file);
        printAttributeName(file, indentLevel, attributeName);
        IFF_printChunk(file, chunk, indentLevel);
    }
}

void IFF_printChunksArrayField(FILE *file, const unsigned int indentLevel, const char *attributeName, IFF_Chunk **chunks, unsigned int chunksLength)
{
    fputs(",\n", file);
    printAttributeName(file, indentLevel, attributeName);
    IFF_printChunksArray(file, chunks, chunksLength, indentLevel);
}
