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

static IFF_Field fields[] = {
    { "opening", &IFF_Type_UByte, IFF_CARDINALITY_SINGLE },
    { "closure", &IFF_Type_UByte, IFF_CARDINALITY_SINGLE }
};

typedef enum
{
    FIELD_INDEX_OPENING = 0,
    FIELD_INDEX_CLOSURE = 1
}
FieldIndex;

static void *getFieldPointer(void *object, const unsigned int index)
{
    TEST_Greet *greet = (TEST_Greet*)object;

    switch(index)
    {
        case FIELD_INDEX_OPENING:
            return &greet->opening;
        case FIELD_INDEX_CLOSURE:
            return &greet->closure;
        default:
            return NULL;
    }
}

static IFF_Structure greetStructure = {
    2,
    fields,
    getFieldPointer,
    NULL,
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
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
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

void TEST_printGreet(FILE *file, const void *value, const unsigned int indentLevel)
{
    IFF_printStructure(file, indentLevel, &greetStructure, (void*)value);
}
