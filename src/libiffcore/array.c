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

void IFF_printUByteValueArray(FILE *file, const unsigned int indentLevel, IFF_UByte *array, const unsigned int arrayLength, const unsigned int elementsPerRow, IFF_printValueFunction printByteValue)
{
    unsigned int i;

    fputs("{\n", file);
    IFF_printIndent(file, indentLevel + 1, "");

    for(i = 0; i < arrayLength; i++)
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

        printByteValue(file, &array[i], indentLevel + 1);
    }

    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

void IFF_printUByteHexArray(FILE *file, const unsigned int indentLevel, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    IFF_printUByteValueArray(file, indentLevel, array, arrayLength, elementsPerRow, IFF_printUByteHex);
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
