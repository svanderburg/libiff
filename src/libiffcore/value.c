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

#include "value.h"

#if IFF_BIG_ENDIAN != 1

static IFF_UWord byteSwapUWord(const IFF_UWord uWordValue)
{
    return (uWordValue & 0xff) << 8 | (uWordValue & 0xff00) >> 8;
}

static IFF_Word byteSwapWord(const IFF_Word wordValue)
{
    return (wordValue & 0xff) << 8 | (wordValue & 0xff00) >> 8;
}

static IFF_ULong byteSwapULong(const IFF_ULong uLongValue)
{
    return (uLongValue & 0xff) << 24 | (uLongValue & 0xff00) << 8 | (uLongValue & 0xff0000) >> 8 | (uLongValue & 0xff000000) >> 24;
}

static IFF_ULong byteSwapLong(const IFF_Long longValue)
{
    return (longValue & 0xff) << 24 | (longValue & 0xff00) << 8 | (longValue & 0xff0000) >> 8 | (longValue & 0xff000000) >> 24;
}

#endif

IFF_Bool IFF_readUByte(FILE *file, void *value)
{
    IFF_UByte *uByteValue = (IFF_UByte*)value;
    *uByteValue = fgetc(file);
    return *uByteValue != EOF;
}

IFF_Bool IFF_writeUByte(FILE *file, const void *value)
{
    const IFF_UByte *uByteValue = (const IFF_UByte*)value;
    return fputc(*uByteValue, file) != EOF;
}

IFF_Bool IFF_readUWord(FILE *file, void *value)
{
    IFF_UWord *uWordValue = (IFF_UWord*)value;

    if(fread(uWordValue, sizeof(IFF_UWord), 1, file) != 1)
        return FALSE;

#if IFF_BIG_ENDIAN != 1
    *uWordValue = byteSwapUWord(*uWordValue);
#endif

    return TRUE;
}

IFF_Bool IFF_writeUWord(FILE *file, const void *value)
{
    IFF_UWord uWordValue = *((const IFF_UWord*)value);

#if IFF_BIG_ENDIAN != 1
    uWordValue = byteSwapUWord(uWordValue);
#endif

    return fwrite(&uWordValue, sizeof(IFF_UWord), 1, file) == 1;
}

IFF_Bool IFF_readWord(FILE *file, void *value)
{
    IFF_Word *wordValue = (IFF_Word*)value;

    if(fread(wordValue, sizeof(IFF_Word), 1, file) != 1)
        return FALSE;

#if IFF_BIG_ENDIAN != 1
    *wordValue = byteSwapWord(*wordValue);
#endif

    return TRUE;
}

IFF_Bool IFF_writeWord(FILE *file, const void *value)
{
    IFF_Word wordValue = *((const IFF_Word*)value);

#if IFF_BIG_ENDIAN != 1
    wordValue = byteSwapWord(wordValue);
#endif

    return fwrite(&wordValue, sizeof(IFF_Word), 1, file) == 1;
}

IFF_Bool IFF_readULong(FILE *file, void *value)
{
    IFF_ULong *uLongValue = (IFF_ULong*)value;

    if(fread(uLongValue, sizeof(IFF_ULong), 1, file) != 1)
        return FALSE;

#if IFF_BIG_ENDIAN != 1
    *uLongValue = byteSwapULong(*uLongValue);
#endif

    return TRUE;
}

IFF_Bool IFF_writeULong(FILE *file, const void *value)
{
    IFF_ULong uLongValue = *((const IFF_ULong*)value);

#if IFF_BIG_ENDIAN != 1
    uLongValue = byteSwapULong(uLongValue);
#endif

    return fwrite(&uLongValue, sizeof(IFF_ULong), 1, file) == 1;
}

IFF_Bool IFF_readLong(FILE *file, void *value)
{
    IFF_Long *longValue = (IFF_Long*)value;

    if(fread(longValue, sizeof(IFF_Long), 1, file) != 1)
        return FALSE;

    *longValue = byteSwapLong(*longValue);

    return TRUE;
}

IFF_Bool IFF_writeLong(FILE *file, const void *value)
{
    IFF_Long longValue = *((const IFF_Long*)value);

#if IFF_BIG_ENDIAN != 1
    longValue = byteSwapLong(longValue);
#endif

    return fwrite(&longValue, sizeof(IFF_Long), 1, file) == 1;
}

void IFF_clearValue(void *value)
{
}

IFF_Bool IFF_compareUByte(const void *value1, const void *value2)
{
    const IFF_UByte *uByte1Value = (const IFF_UByte*)value1;
    const IFF_UByte *uByte2Value = (const IFF_UByte*)value2;

    return *uByte1Value == *uByte2Value;
}

IFF_Bool IFF_compareUWord(const void *value1, const void *value2)
{
    const IFF_UWord *uWord1Value = (const IFF_UWord*)value1;
    const IFF_UWord *uWord2Value = (const IFF_UWord*)value2;

    return *uWord1Value == *uWord2Value;
}

IFF_Bool IFF_compareWord(const void *value1, const void *value2)
{
    const IFF_Word *word1Value = (const IFF_Word*)value1;
    const IFF_Word *word2Value = (const IFF_Word*)value2;

    return *word1Value == *word2Value;
}

IFF_Bool IFF_compareULong(const void *value1, const void *value2)
{
    const IFF_ULong *uLong1Value = (const IFF_ULong*)value1;
    const IFF_ULong *uLong2Value = (const IFF_ULong*)value2;

    return *uLong1Value == *uLong2Value;
}

IFF_Bool IFF_compareLong(const void *value1, const void *value2)
{
    const IFF_Long *long1Value = (const IFF_Long*)value1;
    const IFF_Long *long2Value = (const IFF_Long*)value2;

    return *long1Value == *long2Value;
}

void IFF_printCharValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    fprintf(file, "'%c'", *((const IFF_UByte*)value));
}

void IFF_printUByteValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    fprintf(file, "(UBYTE)%uU", *((const IFF_UByte*)value));
}

void IFF_printUByteHex(FILE *file, const void *value, const unsigned int indentLevel)
{
    const IFF_UByte byte = *((const IFF_UByte*)value);

    fputs("0x", file);

    /* Print 0 prefix for small numbers */

    if(byte <= 0xf)
        fputs("0", file);

    fprintf(file, "%x", byte);
}

void IFF_printByteValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    fprintf(file, "(BYTE)%d", *((const IFF_Byte*)value));
}

void IFF_printUWordValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    fprintf(file, "(UWORD)%uU", *((const IFF_UWord*)value));
}

void IFF_printWordValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    fprintf(file, "(WORD)%d", *((const IFF_Word*)value));
}

void IFF_printULongValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    fprintf(file, "%uU", *((const IFF_ULong*)value));
}

void IFF_printLongValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    fprintf(file, "%d", *((const IFF_Long*)value));
}
