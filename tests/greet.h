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

#ifndef __TEST_GREET_H
#define __TEST_GREET_H

typedef struct TEST_Greet TEST_Greet;

#include <ifftypes.h>
#include <chunk.h>
#include <attributepath.h>
#include <error.h>
#include <field.h>

typedef enum
{
    TEST_OPENING_NOTHING = 0,
    TEST_OPENING_HELLO = 1,
    TEST_OPENING_HI = 2,
    TEST_OPENING_GREETINGS = 3
}
TEST_OpeningMessage;

typedef enum
{
    TEST_CLOSURE_NOTHING = 0,
    TEST_CLOSURE_BYE = 1,
    TEST_CLOSURE_SEEYA = 2,
    TEST_CLOSURE_GOOD_DAY = 3
}
TEST_ClosureMessage;

struct TEST_Greet
{
    IFF_UByte opening;
    IFF_UByte closure;
};

extern IFF_Type TEST_Type_Greet;

#ifdef __cplusplus
extern "C" {
#endif

IFF_FieldStatus TEST_readGreet(FILE *file, const IFF_Field *field, void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus TEST_writeGreet(FILE *file, const IFF_Field *field, const void *value, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_Bool TEST_compareGreet(const void *value1, const void *value2);

void TEST_printGreet(FILE *file, const void *value, const unsigned int indentLevel);

IFF_FieldStatus TEST_readGreetArray(FILE *file, const IFF_Field *field, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *actualArrayLength, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_FieldStatus TEST_writeGreetArray(FILE *file, const IFF_Field *field, void *array, const IFF_Long arrayLength, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error);

IFF_Bool TEST_compareGreetArray(const void *array1, const IFF_Long array1Length, const void *array2, const IFF_Long array2Length);

void TEST_printGreetArray(FILE *file, const unsigned int indentLevel, void *array, const IFF_Long arrayLength, const unsigned int elementsPerRow);

#ifdef __cplusplus
}
#endif

#endif
