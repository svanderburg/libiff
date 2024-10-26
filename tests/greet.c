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

#include "greet.h"
#include <structure.h>
#include <array.h>
#include <util.h>

static IFF_Field fields[] = {
    { "opening", &IFF_Type_UByte, IFF_CARDINALITY_SINGLE },
    { "closure", &IFF_Type_UByte, IFF_CARDINALITY_SINGLE }
};

static void *getFieldPointer(void *object, const unsigned int index)
{
    TEST_Greet *greet = (TEST_Greet*)object;

    switch(index)
    {
        case 0:
            return &greet->opening;
        case 1:
            return &greet->closure;
        default:
            return NULL;
    }
}

static IFF_Structure greetStructure = {
    2,
    fields,
    getFieldPointer,
    NULL
};

IFF_Type TEST_Type_Greet = {
    "Greet",
    sizeof(TEST_Greet),
    TEST_readGreet,
    TEST_writeGreet,
    IFF_clearValue,
    TEST_compareGreet,
    TEST_printGreet,
    TEST_readGreetArray,
    TEST_writeGreetArray,
    IFF_clearValueArray,
    TEST_compareGreetArray,
    TEST_printGreetArray
};


IFF_FieldStatus TEST_readGreet(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_readStructure(file, &greetStructure, value, chunk, attributePath, bytesProcessed, error);
}

IFF_FieldStatus TEST_writeGreet(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    return IFF_writeStructure(file, &greetStructure, (void*)value, chunk, attributePath, bytesProcessed, error);
}

IFF_Bool TEST_compareGreet(const void *value1, const void *value2)
{
    return IFF_compareStructure(&greetStructure, (void*)value1, (void*)value2);
}

void TEST_printGreet(FILE *file, const unsigned int indentLevel, const IFF_Field *field, const void *value)
{
    IFF_printStructure(file, indentLevel, &greetStructure, (void*)value);
}

IFF_FieldStatus TEST_readGreetArray(FILE *file, const IFF_Field *field, void *array, size_t arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    TEST_Greet *greets = (TEST_Greet*)array;
    unsigned int i;

    for(i = 0; i < arrayLength; i++)
    {
        IFF_FieldStatus status;

        if((status = TEST_readGreet(file, field, &greets[i], chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
            return status;
    }

    return IFF_FIELD_MORE;
}

IFF_FieldStatus TEST_writeGreetArray(FILE *file, const IFF_Field *field, void *array, size_t arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    TEST_Greet *greets = (TEST_Greet*)array;
    unsigned int i;

    for(i = 0; i < arrayLength; i++)
    {
        IFF_FieldStatus status;

        if((status = TEST_writeGreet(file, field, &greets[i], chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
            return status;
    }

    return IFF_FIELD_MORE;
}

IFF_Bool TEST_compareGreetArray(const void *array1, const unsigned int array1Length, const void *array2, const unsigned int array2Length)
{
    if(array1Length == array2Length)
    {
        const TEST_Greet *greet1Array = (const TEST_Greet*)array1;
        const TEST_Greet *greet2Array = (const TEST_Greet*)array2;

        unsigned int i;

        for(i = 0; i < array1Length; i++)
        {
            if(!TEST_compareGreet(&greet1Array[i], &greet2Array[i]))
                return FALSE;
        }

        return TRUE;
    }
    else
        return FALSE;
}

void TEST_printGreetArray(FILE *file, const unsigned int indentLevel, const IFF_Field *field, void *array, const unsigned int arrayLength, const unsigned int elementsPerRow)
{
    TEST_Greet *greetArray = (TEST_Greet*)array;
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

        TEST_printGreet(file, indentLevel, field, &greetArray[i]);
    }

    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}
