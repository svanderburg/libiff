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

#ifndef __IFF_VALUE_H
#define __IFF_VALUE_H

#include <stdio.h>
#include "ifftypes.h"
#include "attributepath.h"
#include "error.h"
#include "id.h"

typedef void (*IFF_printValueFunction) (FILE *file, const void *value, const unsigned int indentLevel);

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
IFF_Bool IFF_readUByte(FILE *file, IFF_UByte *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Writes an unsigned byte to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeUByte(FILE *file, const IFF_UByte value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Reads an unsigned word from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readUWord(FILE *file, IFF_UWord *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Writes an unsigned word to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeUWord(FILE *file, const IFF_UWord value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Reads a signed word from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readWord(FILE *file, IFF_Word *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Writes a signed word to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeWord(FILE *file, const IFF_Word value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Reads an unsigned long from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readULong(FILE *file, IFF_ULong *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Writes an unsigned long to a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeULong(FILE *file, const IFF_ULong value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Reads a signed long from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readLong(FILE *file, IFF_Long *value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

/**
 * Writes a signed long to a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @param chunkId A 4 character chunk id in which the operation takes place (used for error reporting)
 * @param attributeName The name of the attribute that is examined (used for error reporting)
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeLong(FILE *file, const IFF_Long value, IFF_AttributePath *attributePath, char *attributeName, const IFF_ID chunkId, IFF_IOError **error);

void IFF_printCharValue(FILE *file, const void *value, const unsigned int indentLevel);

void IFF_printUByteValue(FILE *file, const void *value, const unsigned int indentLevel);

void IFF_printUByteHex(FILE *file, const void *value, const unsigned int indentLevel);

void IFF_printByteValue(FILE *file, const void *value, const unsigned int indentLevel);

void IFF_printUWordValue(FILE *file, const void *value, const unsigned int indentLevel);

void IFF_printWordValue(FILE *file, const void *value, const unsigned int indentLevel);

void IFF_printULongValue(FILE *file, const void *value, const unsigned int indentLevel);

void IFF_printLongValue(FILE *file, const void *value, const unsigned int indentLevel);

#ifdef __cplusplus
}
#endif

#endif
