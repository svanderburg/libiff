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

#ifndef __IFF_TEXTCHUNK_H
#define __IFF_TEXTCHUNK_H

typedef struct IFF_RawChunk IFF_TextChunk;

#include <stdio.h>
#include "chunk.h"

#define IFF_ID_TEXT IFF_MAKEID('T', 'E', 'X', 'T')

extern IFF_ChunkInterface IFF_textChunkInterface;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a text chunk with the given chunk ID and size. The resulting chunk must be freed using IFF_free().
 *
 * @param chunkId A 4 character id
 * @param chunkSize Length of the bytes array.
 * @return A raw chunk with the given chunk Id, or NULL if the memory can't be allocated
 */
IFF_TextChunk *IFF_createTextChunk(const IFF_ID chunkId, const IFF_Long chunkSize);

/**
 * Copies the given string into the data of the chunk. Additionally, it makes
 * the chunk size equal to the given string.
 *
 * @param textChunk A text chunk
 * @param text Text to store in the body
 */
void IFF_setTextData(IFF_TextChunk *textChunk, const char *text);

/**
 * Prints the data of the raw chunk as text
 *
 * @param chunk A raw chunk instance
 * @param indentLevel Indent level of the textual representation
 * @param chunkRegistry A registry that determines how to handle a chunk of a certain type, optionally in the scope of a FORM with a certain formType
 */
void IFF_printTextChunkContents(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
