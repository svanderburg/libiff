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

#ifndef __IFFCORE_H
#define __IFFCORE_H

#include "ifftypes.h"
#include "chunk.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an IFF file from a given file descriptor. The resulting chunk must be freed using IFF_free().
 *
 * @param file File descriptor
 * @param registry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @param error An error object that is set when the parsing fails, or NULL when the parsing succeeds
 * @return A chunk hierarchy derived from the IFF file, or NULL when it completely fails to parse
 */
IFF_Chunk *IFF_parseFdCore(FILE *file, const IFF_Registry *registry, IFF_IOError **error);

/**
 * Reads an IFF file from a file with the given filename. The resulting chunk must be freed using IFF_free().
 *
 * @param filename Filename of the file
 * @param registry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @param error An error object that is set when the parsing fails, or NULL when the parsing succeeds
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_parseFileCore(const char *filename, const IFF_Registry *registry, IFF_IOError **error);

/**
 * Reads an IFF file from a file with the given filename or from the standard input when no filename was provided.
 * The resulting chunk must be freed using IFF_free().
 *
 * @param filename Filename of the file or NULL to read from the standard input
 * @param registry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @param error An error object that is set when the parsing fails, or NULL when the parsing succeeds
 * @return A chunk hierarchy derived from the IFF file, or NULL if an error occurs
 */
IFF_Chunk *IFF_parseCore(const char *filename, const IFF_Registry *registry, IFF_IOError **error);

/**
 * Writes an IFF file to a given file descriptor.
 *
 * @param file File descriptor
 * @param chunk A chunk hierarchy representing an IFF file
 * @param error An error object that is set when the writing fails, or NULL when the writing succeeds
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeFd(FILE *file, const IFF_Chunk *chunk, IFF_IOError **error);

/**
 * Writes an IFF file to a file with the given filename.
 *
 * @param filename Filename of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param error An error object that is set when the writing fails, or NULL when the writing succeeds
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_writeFile(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error);

/**
 * Writes an IFF file to a file with the given filename or to the standard output if no filename was provided.
 *
 * @param filename Filename of the file or NULL to write to the standard output
 * @param chunk A chunk hierarchy representing an IFF file
 * @param error An error object that is set when the writing fails, or NULL when the writing succeeds
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_write(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error);

/**
 * Frees an IFF chunk hierarchy from memory.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 */
void IFF_free(IFF_Chunk *chunk);

/**
 * Checks whether an IFF file conforms to the IFF specification.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param registry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @param printCheckMessage Function that gets invoked to print a warning or error
 * @param data An arbitrary data structure passed to the printCheckMessage function
 * @return A numeric value indicating the quality level of the IFF file
 */
IFF_QualityLevel IFF_advancedCheckCore(const IFF_Chunk *chunk, const IFF_Registry *registry, IFF_printCheckMessageFunction printCheckMessage, void *data);

/**
 * Checks whether an IFF file conforms to the IFF specification.
 * It prints warnings and errors on the standard error.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param registry A registry that determines how to handle a chunk of a certain type within a specified scope
 * @return A numeric value indicating the quality level of the IFF file
 */
IFF_QualityLevel IFF_checkCore(const IFF_Chunk *chunk, const IFF_Registry *registry);

/**
 * Prints a textual representation of an IFF file to the given file descriptor.
 *
 * @param file File descriptor of the file
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 */
void IFF_printFd(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel);

/**
 * Prints a textual representation of an IFF file to a file with a given filename.
 *
 * @param filename Filename of the file or NULL to write to the standard output
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_printFile(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel);

/**
 * Prints a textual representation of an IFF file to a file with a given filename or to the standard output if no filename was provided.
 *
 * @param filename Filename of the file or NULL to write to the standard output
 * @param chunk A chunk hierarchy representing an IFF file
 * @param indentLevel Indent level of the textual representation
 * @return TRUE if the file has been successfully written, else FALSE
 */
IFF_Bool IFF_print(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel);

/**
 * Checks whether two given IFF files are equal.
 *
 * @param chunk1 Chunk hierarchy to compare
 * @param chunk2 Chunk hierarchy to compare
 * @return TRUE if the given chunk hierarchies are equal, else FALSE
 */
IFF_Bool IFF_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

/**
 * Traverses over the chunk and its sub chunks, invoking a visitor function for each chunk that it encounters
 *
 * @param chunk A chunk hierarchy representing an IFF file
 * @param data An arbitrary data structure propagated to the visitor function
 * @param visitChunk Function that gets invoked for each chunk that is encountered
 * @return TRUE if the entire chunk hierarchy was traversed, else FALSE
 */
IFF_Bool IFF_traverse(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk);

/**
 * Recalculates the chunk size of the given chunk and recursively updates the chunk sizes of the parent chunks.
 *
 * @param chunk A chunk hierarchy representing an IFF file
 */
void IFF_recalculateChunkSizes(IFF_Chunk *chunk);

#ifdef __cplusplus
}
#endif

#endif
