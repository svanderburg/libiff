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

#ifndef __IFF_EXTENSION_H
#define __IFF_EXTENSION_H

typedef struct IFF_FormExtension IFF_FormExtension;
typedef struct IFF_Extension IFF_Extension;

#include <stdio.h>
#include "ifftypes.h"
#include "chunk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines how a particular application chunk within a FORM should be handled.
 */
struct IFF_FormExtension
{
    /** A 4 character chunk id */
    const char *chunkId;
    
    /** Function resposible for reading the given chunk */
    IFF_Chunk* (*readChunk) (FILE *file, const IFF_Long chunkSize);
    
    /** Function resposible for writing the given chunk */
    int (*writeChunk) (FILE *file, const IFF_Chunk *chunk);
    
    /** Function resposible for checking the given chunk */
    int (*checkChunk) (const IFF_Chunk *chunk);
    
    /** Function resposible for freeing the given chunk */
    void (*freeChunk) (IFF_Chunk *chunk);
    
    /** Function responsible for printing the given chunk */
    void (*printChunk) (const IFF_Chunk *chunk, const unsigned int indentLevel);
    
    /** Function responsible for comparing the given chunk */
    int (*compareChunk) (const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);
};

/**
 * @brief Defines how application chunks in a FORM with a particular formType should be handled.
 */
struct IFF_Extension
{
    /** A 4 character form type id */
    const char *formType;
    
    /** Specifies the number of application chunks in the form that should be handled by external functions */
    unsigned int formExtensionsLength;
    
    /** An array specifying how application chunks within the form context should be handled */
    IFF_FormExtension *formExtensions;
};

/**
 * Searches for a form extension that can deal with a chunk in a given form with a form type and a given chunk id
 *
 * @param formType A 4 character form type id. If the formType is NULL, the function will always return NULL
 * @param chunkId A 4 character chunk id
 * @param extension Extension array which specifies how application file format chunks can be handled
 * @param extensionLength Length of the extension array
 * @return The form extension that handles the specified chunk or NULL if it does not exists
 */
const IFF_FormExtension *IFF_findFormExtension(const char *formType, const char *chunkId, const IFF_Extension *extension, const unsigned int extensionLength);

#ifdef __cplusplus
}
#endif

#endif
