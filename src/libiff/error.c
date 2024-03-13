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

#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void IFF_errorCallbackStderr(const char *formatString, va_list ap)
{
    vfprintf(stderr, formatString, ap);
}

void (*IFF_errorCallback) (const char *formatString, va_list ap) = &IFF_errorCallbackStderr;

void IFF_error(const char *formatString, ...)
{
    va_list ap;

    va_start(ap, formatString);
    IFF_errorCallback(formatString, ap);
    va_end(ap);
}

void IFF_errorId(const IFF_ID id)
{
    unsigned int i;
    IFF_ID2 id2;

    IFF_idToString(id, id2);

    for(i = 0; i < IFF_ID_SIZE; i++)
        IFF_error("%c", id2[i]);
}

void IFF_readError(const IFF_ID chunkId, const char *attributeName)
{
    IFF_error("Error reading '");
    IFF_errorId(chunkId);
    IFF_error("'.%s\n", attributeName);
}

void IFF_writeError(const IFF_ID chunkId, const char *attributeName)
{
    IFF_error("Error writing '");
    IFF_errorId(chunkId);
    IFF_error("'.%s\n", attributeName);
}

IFF_IOError *IFF_createDataIOError(FILE *file, unsigned int dataSize, IFF_AttributePath *attributePath, char *attributeName, char *description, const IFF_ID chunkId)
{
    IFF_DataIOError *error = (IFF_DataIOError*)malloc(sizeof(IFF_DataIOError));

    if(error != NULL)
    {
        error->type = IFF_IO_ERROR_DATA;
        error->dataSize = dataSize;
        error->attributePath = attributePath;
        error->attributeName = attributeName;
        error->position = ftell(file);
        error->description = description;
        error->chunkId = chunkId;
    }

    return (IFF_IOError*)error;
}

IFF_IOError *IFF_createFileIOError(char *filename)
{
    IFF_FileIOError *error = (IFF_FileIOError*)malloc(sizeof(IFF_FileIOError));

    if(error != NULL)
    {
        error->type = IFF_IO_ERROR_FILE;
        error->filename = filename;
        error->reason = strerror(errno);
    }

    return (IFF_IOError*)error;
}

static void clearDataIOError(IFF_DataIOError *error)
{
    IFF_freeAttributePath(error->attributePath);
}

void IFF_freeIOError(IFF_IOError *error)
{
    if(error->type == IFF_IO_ERROR_DATA)
        clearDataIOError((IFF_DataIOError*)error);
    free(error);
}

static void printDataIOError(const IFF_DataIOError *error)
{
    fprintf(stderr, " %u bytes (%s)", error->dataSize, error->description);

    if(error->attributeName == NULL)
    {
        fprintf(stderr, " while processing a chunk at path: ");
        IFF_printAttributePath(error->attributePath);
    }
    else
    {
        fprintf(stderr, " for field: ");
        IFF_printAttributePath(error->attributePath);
        fprintf(stderr, ".%s", error->attributeName);
    }

    if(error->chunkId != 0)
    {
        fputs("inside a chunk with ID: \"", stderr);
        IFF_errorId(error->chunkId);
        fputs("\"", stderr);
    }

    fprintf(stderr, " at position: %ld\n", error->position);
}

static void printFileIOError(const IFF_FileIOError *error)
{
    fprintf(stderr, " file: %s, reason: %s\n", error->filename, error->reason);
}

static void printIOError(const IFF_IOError *error)
{
    switch(error->type)
    {
        case IFF_IO_ERROR_DATA:
            printDataIOError((const IFF_DataIOError*)error);
            break;
        case IFF_IO_ERROR_FILE:
            printFileIOError((const IFF_FileIOError*)error);
            break;
    }
}

void IFF_printReadError(const IFF_IOError *error)
{
    fprintf(stderr, "Cannot read");
    printIOError(error);
}

void IFF_printWriteError(const IFF_IOError *error)
{
    fprintf(stderr, "Cannot write");
    printIOError(error);
}
