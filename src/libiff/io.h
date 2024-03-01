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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an unsigned byte from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readUByte(FILE *file, IFF_UByte *value, const IFF_ID chunkId, const char *attributeName);

/**
 * Writes an unsigned byte to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeUByte(FILE *file, const IFF_UByte value, const IFF_ID chunkId, const char *attributeName);

/**
 * Reads an unsigned word from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readUWord(FILE *file, IFF_UWord *value, const IFF_ID chunkId, const char *attributeName);

/**
 * Writes an unsigned word to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeUWord(FILE *file, const IFF_UWord value, const IFF_ID chunkId, const char *attributeName);

/**
 * Reads a signed word from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readWord(FILE *file, IFF_Word *value, const IFF_ID chunkId, const char *attributeName);

/**
 * Writes a signed word to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeWord(FILE *file, const IFF_Word value, const IFF_ID chunkId, const char *attributeName);

/**
 * Reads an unsigned long from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readULong(FILE *file, IFF_ULong *value, const IFF_ID chunkId, const char *attributeName);

/**
 * Writes an unsigned long to a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeULong(FILE *file, const IFF_ULong value, const IFF_ID chunkId, const char *attributeName);

/**
 * Reads a signed long from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readLong(FILE *file, IFF_Long *value, const IFF_ID chunkId, const char *attributeName);

/**
 * Writes a signed long to a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeLong(FILE *file, const IFF_Long value, const IFF_ID chunkId, const char *attributeName);

/**
 * Skips the remaining data in a chunk that was not processed.
 *
 * @param file File descriptor of the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param chunkSize Size of the chunk in bytes
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the data was successfully skipped, else FALSE
 */
IFF_Bool IFF_skipUnknownBytes(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Long bytesProcessed);

/**
 * Writes 0-filler bytes for the remainder of the data in a chunk.
 *
 * @param file File descriptor of the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param chunkSize Size of the chunk in bytes
 * @param bytesProcessed Indicates how many bytes in the chunk body were processed
 * @return TRUE if the data was successfully written, else FALSE
 */
IFF_Bool IFF_writeZeroFillerBytes(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_Long bytesProcessed);

/**
 * Reads a padding byte from a chunk with an odd size.
 *
 * @param file File descriptor of the file
 * @param chunkSize Size of the chunk in bytes
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @return TRUE if the byte has been successfully read, else FALSE
 */
IFF_Bool IFF_readPaddingByte(FILE *file, const IFF_Long chunkSize, const IFF_ID chunkId);

/**
 * Writes a padding byte to a chunk with an odd size.
 *
 * @param file File descriptor of the file
 * @param chunkSize Size of the chunk in bytes
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @return TRUE if the byte has been successfully written, else FALSE
 */
IFF_Bool IFF_writePaddingByte(FILE *file, const IFF_Long chunkSize, const IFF_ID chunkId);

#ifdef __cplusplus
}
#endif

#endif
