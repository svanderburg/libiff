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

#include "datachunk.h"
#include "rawchunk.h"
#include "extensionchunk.h"

IFF_Chunk *IFF_readDataChunk(FILE *file, const IFF_ID chunkId, const IFF_Long chunkSize, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunkId, extension, extensionLength);

    if(formExtension == NULL)
        return (IFF_Chunk*)IFF_readRawChunk(file, chunkId, chunkSize);
    else
        return IFF_readExtensionChunk(file, chunkSize, formExtension);
}

IFF_Bool IFF_writeDataChunk(FILE *file, const IFF_Chunk *chunk, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);

    if(formExtension == NULL)
        return IFF_writeRawChunk(file, (IFF_RawChunk*)chunk);
    else
        return IFF_writeExtensionChunk(file, chunk, formExtension);
}

IFF_Bool IFF_checkDataChunk(const IFF_Chunk *chunk, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);

    if(formExtension == NULL)
        return TRUE;
    else
        return IFF_checkExtensionChunk(chunk, formExtension);
}

void IFF_freeDataChunk(IFF_Chunk *chunk, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);

    if(formExtension == NULL)
        IFF_freeRawChunk((IFF_RawChunk*)chunk);
    else
        IFF_freeExtensionChunk(chunk, formExtension);
}

void IFF_printDataChunk(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk->chunkId, extension, extensionLength);

    if(formExtension == NULL)
        IFF_printRawChunk((IFF_RawChunk*)chunk, indentLevel + 1);
    else
        IFF_printExtensionChunk(chunk, indentLevel + 1, formExtension);
}

IFF_Bool IFF_compareDataChunk(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ID formType, const IFF_Extension *extension, const unsigned int extensionLength)
{
    const IFF_FormExtension *formExtension = IFF_findFormExtension(formType, chunk1->chunkId, extension, extensionLength);

    if(formExtension == NULL)
        return IFF_compareRawChunk((const IFF_RawChunk*)chunk1, (const IFF_RawChunk*)chunk2);
    else
        return IFF_compareExtensionChunk(chunk1, chunk2, formExtension);
}
