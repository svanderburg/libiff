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

typedef IFF_Bool (*IFF_readValueFunction) (FILE *file, void *value);
typedef IFF_Bool (*IFF_writeValueFunction) (FILE *file, const void *value);
typedef void (*IFF_printValueFunction) (FILE *file, const void *value, const unsigned int indentLevel);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an unsigned byte from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readUByte(FILE *file, void *value);

/**
 * Writes an unsigned byte to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeUByte(FILE *file, const void *value);

/**
 * Reads an unsigned word from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readUWord(FILE *file, void *value);

/**
 * Writes an unsigned word to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeUWord(FILE *file, const void *value);

/**
 * Reads a signed word from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readWord(FILE *file, void *value);

/**
 * Writes a signed word to a file.
 *
 * @param file File descriptor of the file
 * @param value Value written to the file
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeWord(FILE *file, const void *value);

/**
 * Reads an unsigned long from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readULong(FILE *file, void *value);

/**
 * Writes an unsigned long to a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeULong(FILE *file, const void *value);

/**
 * Reads a signed long from a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @return TRUE if the value has been successfully read, else FALSE
 */
IFF_Bool IFF_readLong(FILE *file, void *value);

/**
 * Writes a signed long to a file.
 *
 * @param file File descriptor of the file
 * @param value Value read from the file
 * @return TRUE if the value has been successfully written, else FALSE
 */
IFF_Bool IFF_writeLong(FILE *file, const void *value);

IFF_Bool IFF_compareUByte(const void *value1, const void *value2);

IFF_Bool IFF_compareUWord(const void *value1, const void *value2);

IFF_Bool IFF_compareWord(const void *value1, const void *value2);

IFF_Bool IFF_compareULong(const void *value1, const void *value2);

IFF_Bool IFF_compareLong(const void *value1, const void *value2);

void IFF_clearValue(void *value);

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
