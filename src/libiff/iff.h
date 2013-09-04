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

#include "ifftypes.h"
#include "chunk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an IFF file from a given file descriptor. The resulting chunk must be freed using IFF_free().
 *
 * @param file File descriptor of the file
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_readFd(FILE *file, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Reads an IFF file from a file with the given filename. The resulting chunk must be freed using IFF_free().
 *
 * @param filename Filename of the file
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_read(const char *filename, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes an IFF file to a given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the file has been successfully written, else FALSE
 */
int IFF_writeFd(FILE *file, const IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Writes an IFF file to a file with the given filename.
 *
 * @param filename Filename of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the file has been successfully written, else FALSE
 */
int IFF_write(const char *filename, const IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Frees an IFF chunk hierarchy from memory.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 */
void IFF_free(IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether an IFF file conforms to the IFF specification.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the IFF file conforms to the IFF specification, else FALSE
 */
int IFF_check(const IFF_Chunk *chunk, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Displays a textual representation of an IFF file on the standard output.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 */
void IFF_print(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_Extension *extension, const unsigned int extensionLength);

/**
 * Checks whether two given IFF files are equal.
 *
 * @param chunk1 Chunk hierarchy to compare
 * @param chunk2 Chunk hierarchy to compare
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return TRUE if the given chunk hierarchies are equal, else FALSE
 */
int IFF_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_Extension *extension, const unsigned int extensionLength);

#ifdef __cplusplus
}
#endif

#endif
