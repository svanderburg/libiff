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

#include "array.h"
#include <string.h>
#include <stdlib.h>
#include "util.h"

static size_t computeArraySize(size_t elementSize, size_t arrayLength)
{
    return arrayLength * elementSize;
}

static IFF_Bool readValueArray(FILE *file, void *array, size_t elementSize, size_t arrayLength)
{
    size_t arraySize = computeArraySize(elementSize, arrayLength);

    return fread(array, elementSize, arrayLength, file) == arraySize;
}

IFF_Bool IFF_readUByteArray(FILE *file, void *array, size_t arrayLength)
{
    return readValueArray(file, array, sizeof(IFF_UByte), arrayLength);
}

static IFF_Bool writeValueArray(FILE *file, void *array, size_t elementSize, size_t arrayLength)
{
    size_t arraySize = computeArraySize(elementSize, arrayLength);

    return fwrite(array, elementSize, arrayLength, file) == arraySize;
}

IFF_Bool IFF_writeUByteArray(FILE *file, void *array, size_t arrayLength)
{
    return writeValueArray(file, array, sizeof(IFF_UByte), arrayLength);
}

void IFF_clearValueArray(void *array, const unsigned int arrayLength)
{
    free(array);
}

IFF_Bool IFF_compareArray(const void *array1, size_t element1Size, const unsigned int array1Length, const void *array2, size_t element2Size, const unsigned int array2Length, IFF_compareValueFunction compareValue)
{
    if(array1Length == array2Length && element1Size == element2Size)
    {
        const IFF_UByte *rawArray1 = (const IFF_UByte*)array1;
        const IFF_UByte *rawArray2 = (const IFF_UByte*)array2;
        size_t arraySize = element1Size * array1Length;
        unsigned int i;

        for(i = 0; i < arraySize; i += element1Size)
        {
            if(!compareValue(rawArray1 + i, rawArray2 + i))
                return FALSE;
        }

        return TRUE;
    }
    else
        return FALSE;
}

static IFF_Bool compareValueArray(const void *array1, const unsigned int array1Length, const void *array2, const unsigned int array2Length, size_t elementSize)
{
    return array1Length == array2Length && memcmp(array1, array2, array1Length * elementSize) == 0;
}

IFF_Bool IFF_compareUByteArray(const void *array1, const unsigned int array1Length, const void *array2, const unsigned int array2Length)
{
    return compareValueArray(array1, array1Length, array2, array2Length, sizeof(IFF_UByte));
}

void IFF_printArray(FILE *file, const unsigned int indentLevel, void *array, size_t elementSize, const unsigned int arrayLength, const unsigned int elementsPerRow, IFF_printValueFunction printValue)
{
    size_t i;
    IFF_UByte *rawArray = (IFF_UByte*)array;
    size_t arraySize = elementSize * arrayLength;

    fputs("{\n", file);
    IFF_printIndent(file, indentLevel + 1, "");

    for(i = 0; i < arraySize; i += elementSize)
    {
        if(i > 0)
        {
            fputs(",\n", file);
            IFF_printIndent(file, indentLevel + 1, "");
        }

        printValue(file, rawArray + i, indentLevel + 1);
    }

    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

void IFF_printValueArray(FILE *file, const unsigned int indentLevel, IFF_UByte *array, size_t elementSize, const unsigned int arrayLength, const unsigned int elementsPerRow, IFF_printValueFunction printValue)
{
    size_t i;
    IFF_UByte *rawArray = (IFF_UByte*)array;
    size_t arraySize = elementSize * arrayLength;

    fputs("{\n", file);
    IFF_printIndent(file, indentLevel + 1, "");

    for(i = 0; i < arraySize; i += elementSize)
    {
        if(i > 0)
        {
            fputs(", ", file);

            if(i % elementsPerRow == 0)
            {
                fputc('\n', file);
                IFF_printIndent(file, indentLevel + 1, "");
            }
        }

        printValue(file, rawArray + i, indentLevel + 1);
    }

    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

void IFF_printUByteHexArray(FILE *file, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    IFF_printValueArray(file, indentLevel, array, sizeof(IFF_UByte), arrayLength, elementsPerRow, IFF_printUByteHex);
}

void IFF_printText(FILE *file, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    IFF_UByte *ubyteArray = (IFF_UByte*)array;
    unsigned int i;

    fputc('"', file);

    for(i = 0; i < arrayLength; i++)
    {
        char character = ubyteArray[i];

        if(character == '"')
            fputs("\\\"", file);
        else
            fputc(character, file);
    }

    fputc('"', file);
}
