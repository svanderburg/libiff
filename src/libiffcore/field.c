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

static IFF_FieldStatus readValueField(FILE *file, IFF_readValueFunction readValue, void *value, size_t fieldSize, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    if(fieldDoesNotFitInChunk(fieldSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(readValue(file, value))
    {
        increaseBytesProcessed(bytesProcessed, fieldSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, attributeName, description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

static IFF_FieldStatus writeValueField(FILE *file, IFF_writeValueFunction writeValue, const void *value, size_t fieldSize, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    if(fieldDoesNotFitInChunk(fieldSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(writeValue(file, value))
    {
        increaseBytesProcessed(bytesProcessed, fieldSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, attributeName, description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

IFF_FieldStatus IFF_readUByteField(FILE *file, IFF_UByte *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, IFF_readUByte, value, sizeof(IFF_UByte), chunk, attributePath, attributeName, "UBYTE", bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUByteField(FILE *file, const IFF_UByte value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, IFF_writeUByte, &value, sizeof(IFF_UByte), chunk, attributePath, attributeName, "UBYTE", bytesProcessed, error);
}

IFF_FieldStatus IFF_readUWordField(FILE *file, IFF_UWord *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, IFF_readUWord, value, sizeof(IFF_UWord), chunk, attributePath, attributeName, "UWORD", bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUWordField(FILE *file, const IFF_UWord value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, IFF_writeUWord, &value, sizeof(IFF_UWord), chunk, attributePath, attributeName, "UWORD", bytesProcessed, error);
}

IFF_FieldStatus IFF_readWordField(FILE *file, IFF_Word *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, IFF_readWord, value, sizeof(IFF_Word), chunk, attributePath, attributeName, "WORD", bytesProcessed, error);
}

IFF_FieldStatus IFF_writeWordField(FILE *file, const IFF_Word value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, IFF_writeWord, &value, sizeof(IFF_Word), chunk, attributePath, attributeName, "WORD", bytesProcessed, error);
}

IFF_FieldStatus IFF_readULongField(FILE *file, IFF_ULong *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, IFF_readULong, value, sizeof(IFF_ULong), chunk, attributePath, attributeName, "ULONG", bytesProcessed, error);
}

IFF_FieldStatus IFF_writeULongField(FILE *file, const IFF_ULong value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, IFF_writeULong, &value, sizeof(IFF_ULong), chunk, attributePath, attributeName, "ULONG", bytesProcessed, error);
}

IFF_FieldStatus IFF_readLongField(FILE *file, IFF_Long *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, IFF_readLong, value, sizeof(IFF_Long), chunk, attributePath, attributeName, "LONG", bytesProcessed, error);
}

IFF_FieldStatus IFF_writeLongField(FILE *file, const IFF_Long value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, IFF_writeLong, &value, sizeof(IFF_Long), chunk, attributePath, attributeName, "LONG", bytesProcessed, error);
}

IFF_FieldStatus IFF_readIdField(FILE *file, IFF_ID *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readValueField(file, IFF_readId, value, sizeof(IFF_ID), chunk, attributePath, attributeName, "ID", bytesProcessed, error);
}

IFF_FieldStatus IFF_writeIdField(FILE *file, const IFF_ID value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeValueField(file, IFF_writeId, &value, sizeof(IFF_ID), chunk, attributePath, attributeName, "ID", bytesProcessed, error);
}

static IFF_FieldStatus readArrayField(FILE *file, IFF_readArrayFunction readArray, void *array, size_t elementSize, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    size_t fieldSize = length * elementSize;

    if(fieldDoesNotFitInChunk(fieldSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(readArray(file, array, length))
    {
        increaseBytesProcessed(bytesProcessed, fieldSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, attributeName, description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

static IFF_FieldStatus writeArrayField(FILE *file, IFF_writeArrayFunction writeArray, void *array, size_t elementSize, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, char *description, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    size_t fieldSize = length * elementSize;

    if(fieldDoesNotFitInChunk(fieldSize, chunk->chunkSize, *bytesProcessed))
        return IFF_FIELD_LAST;
    else if(writeArray(file, array, length))
    {
        increaseBytesProcessed(bytesProcessed, fieldSize);
        return IFF_FIELD_MORE;
    }
    else
    {
        *error = IFF_createDataIOError(file, fieldSize, attributePath, attributeName, description, chunk->chunkId);
        return IFF_FIELD_FAILURE;
    }
}

IFF_FieldStatus IFF_readUByteArrayField(FILE *file, IFF_UByte *ubyteArray, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return readArrayField(file, IFF_readUByteArray, ubyteArray, sizeof(IFF_UByte), length, chunk, attributePath, attributeName, "UBYTE[]", bytesProcessed, error);
}

IFF_FieldStatus IFF_writeUByteArrayField(FILE *file, IFF_UByte *ubyteArray, size_t length, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, char *attributeName, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return writeArrayField(file, IFF_writeUByteArray, ubyteArray, sizeof(IFF_UByte), length, chunk, attributePath, attributeName, "UBYTE[]", bytesProcessed, error);
}

static void printAttributeName(FILE *file, const unsigned int indentLevel, const char *attributeName)
{
    IFF_printIndent(file, indentLevel, ".%s = ", attributeName);
}

void IFF_printFirstField(FILE *file, const unsigned int indentLevel, const char *attributeName, const void *value, IFF_printValueFunction printValue)
{
    printAttributeName(file, indentLevel, attributeName);
    printValue(file, value, indentLevel);
}

void IFF_printField(FILE *file, const unsigned int indentLevel, const char *attributeName, const void *value, IFF_printValueFunction printValue)
{
    fputs(",\n", file);
    IFF_printFirstField(file, indentLevel, attributeName, value, printValue);
}

void IFF_printCharField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_UByte value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printCharValue);
}

void IFF_printUByteField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_UByte value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printUByteValue);
}

void IFF_printByteField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Byte value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printByteValue);
}

void IFF_printUWordField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_UWord value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printUWordValue);
}

void IFF_printWordField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Word value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printWordValue);
}

void IFF_printULongField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_ULong value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printULongValue);
}

void IFF_printLongField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_Long value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printLongValue);
}

void IFF_printIdField(FILE *file, const unsigned int indentLevel, const char *attributeName, const IFF_ID value)
{
    IFF_printField(file, indentLevel, attributeName, &value, IFF_printIdValue);
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

static void printArrayField(FILE *file, IFF_printArrayFunction printArray, const unsigned int indentLevel, const char *attributeName, IFF_UByte *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    fputs(",\n", file);
    printAttributeName(file, indentLevel, attributeName);
    printArray(file, indentLevel, array, arrayLength, elementsPerRow);
}

void IFF_printUByteHexArrayField(FILE *file, const unsigned int indentLevel, const char *attributeName, IFF_UByte *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    printArrayField(file, IFF_printUByteHexArray, indentLevel, attributeName, array, arrayLength, elementsPerRow);
}

void IFF_printTextField(FILE *file, const unsigned int indentLevel, const char *attributeName, IFF_UByte *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    printArrayField(file, IFF_printText, indentLevel, attributeName, array, arrayLength, elementsPerRow);
}
