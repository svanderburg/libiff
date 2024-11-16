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
#include "chunksarray.h"

IFF_Bool IFF_deriveSuccess(const IFF_FieldStatus status)
{
    return (status != IFF_FIELD_FAILURE);
}

static IFF_Bool fieldDoesNotFitInChunk(const IFF_Long fieldSize, const IFF_Long chunkSize, const IFF_Long bytesProcessed)
{
    return bytesProcessed > chunkSize - fieldSize;
}

static void increaseBytesProcessed(IFF_Long *bytesProcessed, const IFF_Long fieldSize)
{
    *bytesProcessed = *bytesProcessed + fieldSize;
}

static IFF_Bool readValueHeaderField(FILE *file, IFF_readValueFunction readValue, void *value, const IFF_Long fieldSize, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_IOError **error)
{
    if(readValue(file, value))
        return TRUE;
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, attributeName, description, chunkId);
        return FALSE;
    }
}

static IFF_Bool writeValueHeaderField(FILE *file, IFF_writeValueFunction writeValue, const void *value, const IFF_Long fieldSize, const IFF_ID chunkId, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_IOError **error)
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

void IFF_printField(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    printAttributeName(file, indentLevel, field->attributeName);
    field->type->printField(file, (void*)value, indentLevel);
}

static IFF_FieldStatus readValueArrayField(FILE *file, const IFF_Field *field, IFF_readArrayFunction readArray, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *actualArrayLength, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_Long arraySize = arrayLength * field->type->elementSize;

    if(fieldDoesNotFitInChunk(arraySize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(readArray(file, array, arrayLength, actualArrayLength))
    {
        increaseBytesProcessed(bytesProcessed, arraySize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, arraySize, attributePath, field->attributeName, field->type->description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

static IFF_FieldStatus writeValueArrayField(FILE *file, const IFF_Field *field, IFF_writeArrayFunction writeArray, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_Long arraySize = arrayLength * field->type->elementSize;

    if(fieldDoesNotFitInChunk(arraySize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(writeArray(file, array, arrayLength))
    {
        increaseBytesProcessed(bytesProcessed, arraySize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, arraySize, attributePath, field->attributeName, field->type->description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

IFF_FieldStatus IFF_readArrayField(FILE *file, const IFF_Field *field, IFF_readFieldFunction readField, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *actualArrayLength, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_UByte *rawArray = (IFF_UByte*)array;
    IFF_Long arraySize = field->type->elementSize * arrayLength;
    IFF_Long i;

    *actualArrayLength = 0;

    for(i = 0; i < arraySize; i += field->type->elementSize)
    {
        IFF_FieldStatus status = readField(file, field, rawArray + i, chunk, attributePath, bytesProcessed, error);

        if(status != IFF_FIELD_FAILURE)
            *actualArrayLength = *actualArrayLength + 1;

        if(status != IFF_FIELD_MORE)
            return status;
    }

    return IFF_FIELD_MORE;
}

IFF_FieldStatus IFF_writeArrayField(FILE *file, const IFF_Field *field, IFF_writeFieldFunction writeField, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    IFF_UByte *rawArray = (IFF_UByte*)array;
    IFF_Long arraySize = field->type->elementSize * arrayLength;
    IFF_Long i;

    for(i = 0; i < arraySize; i += field->type->elementSize)
    {
        IFF_FieldStatus status;

        if((status = writeField(file, field, rawArray + i, chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
            return status;
    }

    return IFF_FIELD_MORE;
}

void IFF_printArrayField(FILE *file, const IFF_Field *field, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    printAttributeName(file, indentLevel, field->attributeName);
    field->type->printArrayField(file, indentLevel, array, arrayLength, elementsPerRow);
}

IFF_Type IFF_Type_UByte = {
    "UBYTE",
    sizeof(IFF_UByte),
    IFF_readUByteField,
    IFF_writeUByteField,
    IFF_clearValue,
    IFF_compareUByte,
    IFF_printUByteValue,
    IFF_readUByteArrayField,
    IFF_writeUByteArrayField,
    IFF_clearValueArray,
    IFF_compareUByteArray,
    IFF_printUByteHexArray
};

IFF_Type IFF_Type_Char = {
    "UBYTE",
    sizeof(IFF_UByte),
    IFF_readUByteField,
    IFF_writeUByteField,
    IFF_clearValue,
    IFF_compareUByte,
    IFF_printCharValue,
    IFF_readUByteArrayField,
    IFF_writeUByteArrayField,
    IFF_clearValueArray,
    IFF_compareUByteArray,
    IFF_printText
};

IFF_FieldStatus IFF_readUByteField(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, field, IFF_readUByte, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUByteField(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, field, IFF_writeUByte, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus IFF_readUByteArrayField(FILE *file, const IFF_Field *field, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *actualArrayLength, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueArrayField(file, field, IFF_readUByteArray, array, arrayLength, chunk, attributePath, actualArrayLength, bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUByteArrayField(FILE *file, const IFF_Field *field, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueArrayField(file, field, IFF_writeUByteArray, array, arrayLength, chunk, attributePath, bytesProcessed, error);
}

IFF_Type IFF_Type_UWord = {
    "UWORD",
    sizeof(IFF_UWord),
    IFF_readUWordField,
    IFF_writeUWordField,
    IFF_clearValue,
    IFF_compareUWord,
    IFF_printUWordValue,
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

IFF_Type IFF_Type_Word = {
    "WORD",
    sizeof(IFF_Word),
    IFF_readWordField,
    IFF_writeWordField,
    IFF_clearValue,
    IFF_compareWord,
    IFF_printWordValue,
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

IFF_Type IFF_Type_ULong = {
    "ULONG",
    sizeof(IFF_ULong),
    IFF_readULongField,
    IFF_writeULongField,
    IFF_clearValue,
    IFF_compareULong,
    IFF_printULongValue,
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

IFF_Type IFF_Type_Long = {
    "LONG",
    sizeof(IFF_Long),
    IFF_readLongField,
    IFF_writeLongField,
    IFF_clearValue,
    IFF_compareLong,
    IFF_printLongValue,
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

IFF_Type IFF_Type_ID = {
    "ID",
    sizeof(IFF_ID),
    IFF_readIdField,
    IFF_writeIdField,
    IFF_clearValue,
    IFF_compareId,
    IFF_printIdValue,
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

void IFF_printChunkField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Chunk *chunk)
{
    printAttributeName(file, indentLevel, attributeName);
    IFF_printChunk(file, chunk, indentLevel);
}

void IFF_printChunksArrayField(FILE *file, const unsigned int indentLevel, const char *attributeName, IFF_Chunk **chunks, unsigned int chunksLength)
{
    printAttributeName(file, indentLevel, attributeName);
    IFF_printChunksArray(file, chunks, chunksLength, indentLevel);
}
