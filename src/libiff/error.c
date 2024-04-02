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

IFF_IOError *IFF_createFileIOError(void)
{
    IFF_FileIOError *error = (IFF_FileIOError*)malloc(sizeof(IFF_FileIOError));

    if(error != NULL)
    {
        error->type = IFF_IO_ERROR_FILE;
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
        IFF_ID2 chunkId;
        IFF_idToString(error->chunkId, chunkId);
        fprintf(stderr, " inside a chunk with ID: \"%.4s\"", chunkId);
    }

    fprintf(stderr, " at position: %ld\n", error->position);
}

static void printFileIOError(const IFF_FileIOError *error)
{
    fprintf(stderr, " file, reason: %s\n", error->reason);
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

void IFF_printCheckMessageOnStderr(const IFF_AttributePath *attributePath, const char *attributeName, const IFF_ID chunkId, void *data, const char *formatString, ...)
{
    va_list ap;

    IFF_printAttributePath(attributePath);

    if(attributeName != NULL)
        fprintf(stderr, ".%s", attributeName);

    fputs(" ", stderr);

    va_start(ap, formatString);
    vfprintf(stderr, formatString, ap);
    va_end(ap);

    if(chunkId != 0)
    {
        IFF_ID2 chunkId2;
        IFF_idToString(chunkId, chunkId2);
        fprintf(stderr, " inside a chunk with chunkId: \"%.4s\"", chunkId2);
    }

    fputs("\n", stderr);
}

IFF_QualityLevel IFF_adjustQualityLevel(const IFF_QualityLevel currentLevel, const IFF_QualityLevel newLevel)
{
    if(newLevel > currentLevel)
        return newLevel;
    else
        return currentLevel;
}
