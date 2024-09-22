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

IFF_Bool IFF_readValueArray(FILE *file, void *pointer, size_t elementSize, size_t length, IFF_AttributePath *attributePath, char *attributeName, char *description, const IFF_ID chunkId, IFF_IOError **error)
{
    size_t arraySize = length * elementSize;

    if(fread(pointer, elementSize, length, file) < arraySize)
    {
        *error = IFF_createDataIOError(file, length, attributePath, attributeName, description, chunkId);
        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_readUByteArray(FILE *file, IFF_UByte *ubyteArray, size_t length, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    return IFF_readValueArray(file, ubyteArray, sizeof(IFF_UByte), length, attributePath, attributeName, "UBYTE[]", chunkId, error);
}

IFF_Bool IFF_writeValueArray(FILE *file, void *pointer, size_t elementSize, size_t length, IFF_AttributePath *attributePath, char *attributeName, char *description, const IFF_ID chunkId, IFF_IOError **error)
{
    size_t arraySize = length * elementSize;

    if(fwrite(pointer, elementSize, length, file) < arraySize)
    {
        *error = IFF_createDataIOError(file, length, attributePath, attributeName, description, chunkId);
        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_writeUByteArray(FILE *file, IFF_UByte *ubyteArray, size_t length, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    return IFF_writeValueArray(file, ubyteArray, sizeof(IFF_UByte), length, attributePath, attributeName, "UBYTE[]", chunkId, error);
}

void IFF_printUByteValueArray(FILE *file, const unsigned int indentLevel, IFF_UByte *array, const unsigned int arrayLength, unsigned int elementsPerRow, IFF_printValueFunction printByteValue)
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

void IFF_printText(FILE *file, const unsigned int indentLevel, IFF_UByte *array, const unsigned int arrayLength)
{
    unsigned int i;

    fputc('"', file);

    for(i = 0; i < arrayLength; i++)
    {
        char character = array[i];

        if(character == '"')
            fputs("\\\"", file);
        else
            fputc(character, file);
    }

    fputc('"', file);
}
