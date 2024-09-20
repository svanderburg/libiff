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

IFF_Bool IFF_readUByte(FILE *file, IFF_UByte *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    int byte = fgetc(file);

    if(byte == EOF)
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_UByte), attributePath, attributeName, "UBYTE", chunkId);
        return FALSE;
    }
    else
    {
        *value = byte;
        return TRUE;
    }
}

IFF_Bool IFF_writeUByte(FILE *file, const IFF_UByte value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    if(fputc(value, file) == EOF)
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_UByte), attributePath, attributeName, "UBYTE", chunkId);
        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_readUWord(FILE *file, IFF_UWord *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    IFF_UWord readUWord;

    if(fread(&readUWord, sizeof(IFF_UWord), 1, file) == 1)
    {
#if IFF_BIG_ENDIAN == 1
        *value = readUWord;
#else
        /* Byte swap it */
        *value = (readUWord & 0xff) << 8 | (readUWord & 0xff00) >> 8;
#endif

        return TRUE;
    }
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_UWord), attributePath, attributeName, "UWORD", chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_writeUWord(FILE *file, const IFF_UWord value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
#if IFF_BIG_ENDIAN == 1
    IFF_UWord writeUWord = value;
#else
    /* Byte swap it */
    IFF_UWord writeUWord = (value & 0xff) << 8 | (value & 0xff00) >> 8;
#endif

    if(fwrite(&writeUWord, sizeof(IFF_UWord), 1, file) == 1)
        return TRUE;
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_UWord), attributePath, attributeName, "UWORD", chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_readWord(FILE *file, IFF_Word *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    IFF_Word readWord;

    if(fread(&readWord, sizeof(IFF_Word), 1, file) == 1)
    {
#if IFF_BIG_ENDIAN == 1
        *value = readWord;
#else
        /* Byte swap it */
        *value = (readWord & 0xff) << 8 | (readWord & 0xff00) >> 8;
#endif
        return TRUE;
    }
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_Word), attributePath, attributeName, "WORD", chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_writeWord(FILE *file, const IFF_Word value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
#if IFF_BIG_ENDIAN == 1
    IFF_Word writeWord = value;
#else
    IFF_Word writeWord = (value & 0xff) << 8 | (value & 0xff00) >> 8;
#endif

    if(fwrite(&writeWord, sizeof(IFF_Word), 1, file) == 1)
        return TRUE;
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_Word), attributePath, attributeName, "WORD", chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_readULong(FILE *file, IFF_ULong *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    IFF_ULong readValue;

    if(fread(&readValue, sizeof(IFF_ULong), 1, file) == 1)
    {
#if IFF_BIG_ENDIAN == 1
        *value = readValue;
#else
        /* Byte swap it */
        *value = (readValue & 0xff) << 24 | (readValue & 0xff00) << 8 | (readValue & 0xff0000) >> 8 | (readValue & 0xff000000) >> 24;
#endif
        return TRUE;
    }
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_ULong), attributePath, attributeName, "ULONG", chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_writeULong(FILE *file, const IFF_ULong value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
#if IFF_BIG_ENDIAN == 1
    IFF_ULong writeValue = value;
#else
    /* Byte swap it */
    IFF_ULong writeValue = (value & 0xff) << 24 | (value & 0xff00) << 8 | (value & 0xff0000) >> 8 | (value & 0xff000000) >> 24;
#endif

    if(fwrite(&writeValue, sizeof(IFF_ULong), 1, file) == 1)
        return TRUE;
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_ULong), attributePath, attributeName, "ULONG", chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_readLong(FILE *file, IFF_Long *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
    IFF_Long readValue;

    if(fread(&readValue, sizeof(IFF_Long), 1, file) == 1)
    {
#if IFF_BIG_ENDIAN == 1
        *value = readValue;
#else
        /* Byte swap it */
        *value = (readValue & 0xff) << 24 | (readValue & 0xff00) << 8 | (readValue & 0xff0000) >> 8 | (readValue & 0xff000000) >> 24;
#endif
        return TRUE;
    }
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_Long), attributePath, attributeName, "LONG", chunkId);
        return FALSE;
    }
}

IFF_Bool IFF_writeLong(FILE *file, const IFF_Long value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error)
{
#if IFF_BIG_ENDIAN == 1
    IFF_Long writeValue = value;
#else
    /* Byte swap it */
    IFF_Long writeValue = (value & 0xff) << 24 | (value & 0xff00) << 8 | (value & 0xff0000) >> 8 | (value & 0xff000000) >> 24;
#endif

    if(fwrite(&writeValue, sizeof(IFF_Long), 1, file) == 1)
        return TRUE;
    else
    {
        *error = IFF_createDataIOError(file, sizeof(IFF_Long), attributePath, attributeName, "LONG", chunkId);
        return FALSE;
    }
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
