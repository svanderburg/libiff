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
#include <stdio.h>

IFF_Bool IFF_skipUnknownBytes(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Long bytesProcessed, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    if(bytesProcessed < chunkSize)
    {
        long bytesToSkip = chunkSize - bytesProcessed;

        if(fseek(file, bytesToSkip, SEEK_CUR) == 0)
            return TRUE;
        else
        {
            *error = IFF_createDataIOError(file, bytesToSkip, attributePath, NULL, "unknown bytes", chunkId);
            return FALSE;
        }
    }
    else
        return TRUE;
}

IFF_Bool IFF_writeZeroFillerBytes(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Long bytesProcessed, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    if(bytesProcessed < chunkSize)
    {
        size_t bytesToSkip = chunkSize - bytesProcessed;
        IFF_UByte *emptyData = (IFF_UByte*)calloc(bytesToSkip, sizeof(IFF_UByte));
        IFF_Bool status = fwrite(emptyData, sizeof(IFF_UByte), bytesToSkip, file) == bytesToSkip;

        if(!status)
            *error = IFF_createDataIOError(file, bytesToSkip, attributePath, NULL, "unknown bytes", chunkId);

        free(emptyData);
        return status;
    }
    else
        return TRUE;
}

IFF_Bool IFF_readPaddingByte(FILE *file, const IFF_Long chunkSize, const IFF_ID chunkId, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    if(chunkSize % 2 != 0) /* Check whether the chunk size is an odd number */
    {
        int byte = fgetc(file); /* Read padding byte */

        if(byte == EOF) /* We shouldn't have reached the EOF yet */
        {
            *error = IFF_createDataIOError(file, 1, attributePath, NULL, "padding byte", chunkId);
            return FALSE;
        }
    }

    return TRUE;
}

IFF_Bool IFF_writePaddingByte(FILE *file, const IFF_Long chunkSize, const IFF_ID chunkId, IFF_AttributePath *attributePath, IFF_IOError **error)
{
    if(chunkSize % 2 != 0) /* Check whether the chunk size is an odd number */
    {
        if(fputc('\0', file) == EOF)
        {
            *error = IFF_createDataIOError(file, 1, attributePath, NULL, "padding byte", chunkId);
            return FALSE;
        }
        else
            return TRUE;
    }

    return TRUE;
}
