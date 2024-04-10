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

#ifndef __IFF_H
#define __IFF_H
#include "chunk.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

IFF_Chunk *IFF_readFd(FILE *file, IFF_IOError **error);

IFF_Chunk *IFF_readFile(const char *filename, IFF_IOError **error);

IFF_Chunk *IFF_read(const char *filename, IFF_IOError **error);

IFF_Bool IFF_writeFd(FILE *file, const IFF_Chunk *chunk, IFF_IOError **error);

IFF_Bool IFF_writeFile(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error);

IFF_Bool IFF_write(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error);

void IFF_free(IFF_Chunk *chunk);

IFF_QualityLevel IFF_advancedCheck(const IFF_Chunk *chunk, IFF_printCheckMessageFunction printCheckMessage, void *data);

IFF_QualityLevel IFF_check(const IFF_Chunk *chunk);

void IFF_printFd(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel);

IFF_Bool IFF_printFile(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel);

IFF_Bool IFF_print(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel);

IFF_Bool IFF_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#ifdef __cplusplus
}
#endif

#endif
