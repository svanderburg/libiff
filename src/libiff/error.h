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

#ifndef __IFF_ERROR_H
#define __IFF_ERROR_H
#include "attributepath.h"
#include "ifftypes.h"

typedef struct IFF_IOError IFF_IOError;
typedef struct IFF_DataIOError IFF_DataIOError;
typedef struct IFF_FileIOError IFF_FileIOError;
typedef void (*IFF_printCheckMessage) (const IFF_AttributePath *attributePath, const char *attributeName, const IFF_ID chunkId, void *data, const char *formatString, ...);

#include <stdarg.h>
#include "id.h"

typedef enum
{
    IFF_IO_ERROR_DATA,
    IFF_IO_ERROR_FILE
}
IFF_IOErrorType;

struct IFF_IOError
{
    IFF_IOErrorType type;
};

struct IFF_DataIOError
{
    IFF_IOErrorType type;
    IFF_AttributePath *attributePath;
    char *attributeName;
    unsigned int dataSize;
    long position;
    char *description;
    IFF_ID chunkId;
};

struct IFF_FileIOError
{
    IFF_IOErrorType type;
    char *reason;
};

#ifdef __cplusplus
extern "C" {
#endif

IFF_IOError *IFF_createDataIOError(FILE *file, unsigned int dataSize, IFF_AttributePath *attributePath, char *attributeName, char *description, const IFF_ID chunkId);

IFF_IOError *IFF_createFileIOError(void);

void IFF_freeIOError(IFF_IOError *error);

void IFF_printReadError(const IFF_IOError *error);

void IFF_printWriteError(const IFF_IOError *error);

void IFF_printCheckMessageOnStderr(const IFF_AttributePath *attributePath, const char *attributeName, const IFF_ID chunkId, void *data, const char *formatString, ...);

#ifdef __cplusplus
}
#endif

#endif
