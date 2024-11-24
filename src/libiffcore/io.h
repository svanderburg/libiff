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

#ifndef __IFF_IO_H
#define __IFF_IO_H

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"
#include "error.h"
#include "attributepath.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Skips the remaining data in a chunk that was not processed.
 *
 * @param file File descriptor of the file
 * @param chunk A sub chunk
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the data was successfully skipped, else FALSE
 */
IFF_Bool IFF_skipUnknownBytes(FILE *file, const IFF_Chunk *chunk, const IFF_Long bytesProcessed, IFF_AttributePath *attributePath, IFF_IOError **error);

/**
 * Writes 0-filler bytes for the remainder of the data in a chunk.
 *
 * @param file File descriptor of the file
 * @param chunk A sub chunk
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the data was successfully written, else FALSE
 */
IFF_Bool IFF_writeZeroFillerBytes(FILE *file, const IFF_Chunk *chunk, const IFF_Long bytesProcessed, IFF_AttributePath *attributePath, IFF_IOError **error);

/**
 * Reads a padding byte from a chunk with an odd size.
 *
 * @param file File descriptor of the file
 * @param chunk A sub chunk
 * @return TRUE if the byte has been successfully read, else FALSE
 */
IFF_Bool IFF_readOptionalPaddingByte(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_IOError **error);

/**
 * Writes a padding byte to a chunk with an odd size.
 *
 * @param file File descriptor of the file
 * @param chunk A sub chunk
 * @return TRUE if the byte has been successfully written, else FALSE
 */
IFF_Bool IFF_writeOptionalPaddingByte(FILE *file, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_IOError **error);

#ifdef __cplusplus
}
#endif

#endif
