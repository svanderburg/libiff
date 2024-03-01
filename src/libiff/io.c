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

#include "io.h"
#include <stdlib.h>
#include "error.h"

IFF_Bool IFF_readUByte(FILE *file, IFF_UByte *value, const IFF_ID chunkId, const char *attributeName)
{
    int byte = fgetc(file);

    if(byte == EOF)
    {
        IFF_readError(chunkId, attributeName);
        return FALSE;
    }
    else
    {
        *value = byte;
        return TRUE;
    }
}

IFF_Bool IFF_writeUByte(FILE *file, const IFF_UByte value, const IFF_ID chunkId, const char *attributeName)
{
    if(fputc(value, file) == EOF)
    {
        IFF_writeError(chunkId, attributeName);
        return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_readUWord(FILE *file, IFF_UWord *value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_readError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_writeUWord(FILE *file, const IFF_UWord value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_writeError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_readWord(FILE *file, IFF_Word *value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_readError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_writeWord(FILE *file, const IFF_Word value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_writeError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_readULong(FILE *file, IFF_ULong *value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_readError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_writeULong(FILE *file, const IFF_ULong value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_writeError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_readLong(FILE *file, IFF_Long *value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_readError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_writeLong(FILE *file, const IFF_Long value, const IFF_ID chunkId, const char *attributeName)
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
        IFF_writeError(chunkId, attributeName);
        return FALSE;
    }
}

IFF_Bool IFF_skipUnknownBytes(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Long bytesProcessed)
{
    if(bytesProcessed < chunkSize)
    {
        long bytesToSkip = chunkSize - bytesProcessed;

        if(fseek(file, bytesToSkip, SEEK_CUR) == 0)
        {
            IFF_error("Cannot skip: %d bytes in data chunk: '", bytesToSkip);
            IFF_errorId(chunkId);
            IFF_error("'\n");
            return TRUE;
        }
        else
            return FALSE;
    }
    else
        return TRUE;
}

IFF_Bool IFF_writeZeroFillerBytes(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Long bytesProcessed)
{
    if(bytesProcessed < chunkSize)
    {
        size_t bytesToSkip = chunkSize - bytesProcessed;
        IFF_UByte *emptyData = (IFF_UByte*)calloc(bytesToSkip, sizeof(IFF_UByte));
        IFF_Bool status = fwrite(emptyData, sizeof(IFF_UByte), bytesToSkip, file) == bytesToSkip;

        if(!status)
        {
            IFF_error("Cannot write: %u zero bytes in data chunk: '", bytesToSkip);
            IFF_errorId(chunkId);
            IFF_error("'\n");
        }

        free(emptyData);
        return status;
    }
    else
        return TRUE;
}

IFF_Bool IFF_readPaddingByte(FILE *file, const IFF_Long chunkSize, const IFF_ID chunkId)
{
    if(chunkSize % 2 != 0) /* Check whether the chunk size is an odd number */
    {
        int byte = fgetc(file); /* Read padding byte */

        if(byte == EOF) /* We shouldn't have reached the EOF yet */
        {
            IFF_error("Unexpected end of file, while reading padding byte of '");
            IFF_errorId(chunkId);
            IFF_error("'\n");
            return FALSE;
        }
        else if(byte != 0) /* Normally, a padding byte is 0, warn if this is not the case */
            IFF_error("WARNING: Padding byte is non-zero!\n");
    }

    return TRUE;
}

IFF_Bool IFF_writePaddingByte(FILE *file, const IFF_Long chunkSize, const IFF_ID chunkId)
{
    if(chunkSize % 2 != 0) /* Check whether the chunk size is an odd number */
    {
        if(fputc('\0', file) == EOF)
        {
            IFF_error("Cannot write padding byte of '");
            IFF_errorId(chunkId);
            IFF_error("'\n");
            return FALSE;
        }
        else
            return TRUE;
    }

    return TRUE;
}
