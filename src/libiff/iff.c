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

#include "iff.h"
#include "iffcore.h"
#include "defaultregistry.h"

IFF_Chunk *IFF_readFd(FILE *file, IFF_IOError **error)
{
    return IFF_readFdCore(file, &IFF_defaultChunkRegistry, error);
}

IFF_Chunk *IFF_readFile(const char *filename, IFF_IOError **error)
{
    return IFF_readFileCore(filename, &IFF_defaultChunkRegistry, error);
}

IFF_Chunk *IFF_read(const char *filename, IFF_IOError **error)
{
    return IFF_readCore(filename, &IFF_defaultChunkRegistry, error);
}

IFF_Bool IFF_writeFd(FILE *file, const IFF_Chunk *chunk, IFF_IOError **error)
{
    return IFF_writeFdCore(file, chunk, &IFF_defaultChunkRegistry, error);
}

IFF_Bool IFF_writeFile(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error)
{
    return IFF_writeFileCore(filename, chunk, &IFF_defaultChunkRegistry, error);
}

IFF_Bool IFF_write(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error)
{
    return IFF_writeCore(filename, chunk, &IFF_defaultChunkRegistry, error);
}

void IFF_free(IFF_Chunk *chunk)
{
    IFF_freeCore(chunk, &IFF_defaultChunkRegistry);
}

IFF_QualityLevel IFF_advancedCheck(const IFF_Chunk *chunk, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    return IFF_advancedCheckCore(chunk, &IFF_defaultChunkRegistry, printCheckMessage, data);
}

IFF_QualityLevel IFF_check(const IFF_Chunk *chunk)
{
    return IFF_checkCore(chunk, &IFF_defaultChunkRegistry);
}

void IFF_printFd(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printFdCore(file, chunk, indentLevel, &IFF_defaultChunkRegistry);
}

IFF_Bool IFF_printFile(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    return IFF_printFileCore(filename, chunk, indentLevel, &IFF_defaultChunkRegistry);
}

IFF_Bool IFF_print(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    return IFF_printCore(filename, chunk, indentLevel, &IFF_defaultChunkRegistry);
}

IFF_Bool IFF_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareCore(chunk1, chunk2, &IFF_defaultChunkRegistry);
}

IFF_Bool IFF_traverse(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk)
{
    return IFF_traverseCore(chunk, data, visitChunk, &IFF_defaultChunkRegistry);
}
