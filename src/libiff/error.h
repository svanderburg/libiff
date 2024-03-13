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

typedef struct IFF_IOError IFF_IOError;
typedef struct IFF_DataIOError IFF_DataIOError;
typedef struct IFF_FileIOError IFF_FileIOError;

#include <stdarg.h>
#include "id.h"
#include "attributepath.h"

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
    char *filename;
    char *reason;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A function pointer specifying which error callback function should be used.
 */
extern void (*IFF_errorCallback) (const char *formatString, va_list ap);

/**
 * An error callback function printing errors to the standard error.
 *
 * @param formatString A format specifier for fprintf()
 * @param ap A list of command-line parameters
 */
void IFF_errorCallbackStderr(const char *formatString, va_list ap);

/**
 * The error callback function used by the IFF library and derivatives.
 *
 * @param formatString A format specifier for fprintf()
 */
void IFF_error(const char *formatString, ...);

/**
 * Prints a 4 character IFF id on the standard error
 *
 * @param id A 4 character IFF id
 */
void IFF_errorId(const IFF_ID id);

/**
 * Prints a standard read error message.
 *
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 */
void IFF_readError(const IFF_ID chunkId, const char *attributeName);

/**
 * Prints a standard write error message.
 *
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 */
void IFF_writeError(const IFF_ID chunkId, const char *attributeName);

IFF_IOError *IFF_createDataIOError(FILE *file, unsigned int dataSize, IFF_AttributePath *attributePath, char *attributeName, char *description, const IFF_ID chunkId);

IFF_IOError *IFF_createFileIOError(char *filename);

void IFF_freeIOError(IFF_IOError *error);

void IFF_printReadError(const IFF_IOError *error);

void IFF_printWriteError(const IFF_IOError *error);

#ifdef __cplusplus
}
#endif

#endif
