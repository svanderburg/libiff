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

#include "chunksarray.h"
#include <stdlib.h>
#include "array.h"
#include "util.h"

IFF_Chunk **IFF_addChunkToChunksArray(IFF_Chunk **chunks, unsigned int *chunksLength, IFF_Chunk *chunk)
{
    return (IFF_Chunk**)IFF_addElementToPointerArray((void**)chunks, (void*)chunk, chunksLength);
}

IFF_Chunk *IFF_replaceChunkInChunksArrayByIndex(IFF_Chunk **chunks, const unsigned int chunksLength, const unsigned int index, IFF_Chunk *chunk)
{
    return (IFF_Chunk*)IFF_replaceElementInPointerArrayByIndex((void**)chunks, chunksLength, index, (void*)chunk);
}

IFF_Chunk **IFF_removeChunkFromChunksArrayByIndex(IFF_Chunk **chunks, unsigned int *chunksLength, const unsigned int index, IFF_Chunk **obsoleteChunk)
{
    return (IFF_Chunk**)IFF_removeElementFromPointerArrayByIndex((void**)chunks, index, chunksLength, (void**)obsoleteChunk);
}

IFF_Chunk **IFF_appendChunksArrayToChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength, IFF_Chunk **appendChunks, const unsigned int appendChunksLength, unsigned int *resultChunksLength)
{
    return (IFF_Chunk**)IFF_appendPointerArrayToPointerArray((void**)chunks, chunksLength, (void**)appendChunks, appendChunksLength, resultChunksLength);
}

IFF_Bool IFF_writeChunksArray(FILE *file, IFF_Chunk **chunks, const unsigned int chunksLength, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int i;

    for(i = 0; i < chunksLength; i++)
    {
        IFF_Chunk *chunk = chunks[i];

        IFF_visitAttributeByIndex(attributePath, i);

        if(!IFF_writeChunk(file, chunk, attributePath, error))
            return FALSE;

        /* Increase the bytes processed counter */
        *bytesProcessed = IFF_addChunkSize(*bytesProcessed, chunk);

        IFF_unvisitAttribute(attributePath);
    }

    return TRUE;
}

IFF_QualityLevel IFF_checkChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    unsigned int i;

    for(i = 0; i < chunksLength; i++)
    {
        IFF_visitAttributeByIndex(attributePath, i);
        qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(chunks[i], attributePath, printCheckMessage, data));
        IFF_unvisitAttribute(attributePath);
    }

    return qualityLevel;
}

void IFF_freeChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength)
{
    unsigned int i;

    for(i = 0; i < chunksLength; i++)
        IFF_freeChunk(chunks[i]);

    free(chunks);
}

void IFF_printChunksArray(FILE *file, IFF_Chunk **chunks, const unsigned int chunksLength, const unsigned int indentLevel)
{
    unsigned int i;

    fputs("{\n", file);

    for(i = 0; i < chunksLength; i++)
    {
        if(i > 0)
            fputs(",\n", file);

        IFF_printIndent(file, indentLevel + 1, "");
        IFF_printChunk(file, (const IFF_Chunk*)chunks[i], indentLevel + 1);
    }

    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}

IFF_Bool IFF_compareChunksArray(IFF_Chunk **chunks1, const unsigned int chunks1Length, IFF_Chunk **chunks2, const unsigned int chunks2Length)
{
    if(chunks1Length == chunks2Length)
    {
        unsigned int i;

        for(i = 0; i < chunks1Length; i++)
        {
            if(!IFF_compareChunk(chunks1[i], chunks2[i]))
                return FALSE;
        }

        return TRUE;
    }
    else
        return FALSE;
}

IFF_Bool IFF_traverseChunksArray(IFF_Chunk **chunks, const unsigned int chunksLength, void *data, IFF_visitChunkFunction visitChunk)
{
    unsigned int i;

    for(i = 0; i < chunksLength; i++)
    {
        if(!IFF_traverseChunkHierarchy(chunks[i], data, visitChunk))
            return FALSE;
    }

    return TRUE;
}

IFF_Long IFF_addChunksArraySize(IFF_Chunk **chunks, const unsigned int chunksLength, const IFF_Long chunkSize)
{
    IFF_Long result = chunkSize;
    unsigned int i;

    for(i = 0; i < chunksLength; i++)
        result = IFF_addChunkSize(result, chunks[i]);

    return result;
}
