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

#include <stdio.h>
#include <stdlib.h>
#include "chunk.h"
#include "id.h"
#include "form.h"
#include "iff.h"
#include "rawchunk.h"

#define ID_HELO IFF_MAKEID('H', 'E', 'L', 'O')

static IFF_Bool checkCommonChunkProperties(const IFF_Chunk *chunk)
{
    if(chunk->chunkId != ID_HELO)
    {
        fprintf(stderr, "Form chunk should be an HELO chunk!\n");
        return FALSE;
    }

    if(chunk->chunkSize != 4)
    {
        fprintf(stderr, "Form should have size: 4!\n");
        return FALSE;
    }

    return TRUE;
}

static IFF_Bool checkHello1Chunk(const IFF_Chunk *hello1Chunk)
{
    const IFF_RawChunk *hello1rawChunk = (const IFF_RawChunk*)hello1Chunk;

    if(!checkCommonChunkProperties(hello1Chunk))
        return FALSE;

    if(hello1rawChunk->chunkData[0] != '1' ||
       hello1rawChunk->chunkData[1] != '2' ||
       hello1rawChunk->chunkData[2] != '3' ||
       hello1rawChunk->chunkData[3] != '4')
    {
        fprintf(stderr, "Error: the first HELO chunk contents should be: '1', '2', '3', '4'\n");
        return FALSE;
    }

    return TRUE;
}

static IFF_Bool checkHello2Chunk(const IFF_Chunk *hello2Chunk)
{
    const IFF_RawChunk *hello2rawChunk = (const IFF_RawChunk*)hello2Chunk;

    if(!checkCommonChunkProperties(hello2Chunk))
        return FALSE;

    if(hello2rawChunk->chunkData[0] != 'a' ||
       hello2rawChunk->chunkData[1] != 'b' ||
       hello2rawChunk->chunkData[2] != 'c' ||
       hello2rawChunk->chunkData[3] != 'd')
    {
        fprintf(stderr, "Error: the second HELO chunk contents should be: 'a', 'b', 'c', 'd'\n");
        return FALSE;
    }

    return TRUE;
}

static IFF_Bool lookupPropertyAndCheck(const IFF_Chunk *chunk)
{
    if(chunk->chunkId == IFF_ID_FORM)
    {
        IFF_Form *form = (IFF_Form*)chunk;
        unsigned int numOfChunks;
        IFF_Chunk **chunks = IFF_getChunksFromForm(form, ID_HELO, &numOfChunks);
        IFF_Bool status = TRUE;

        if(numOfChunks == 2)
        {
            if(!checkHello1Chunk(chunks[0]) || !checkHello2Chunk(chunks[1]))
                status = FALSE;
        }
        else
        {
            fprintf(stderr, "We should find 2 HELO chunks, instead we have: %u!\n", numOfChunks);
            status = FALSE;
        }

        free(chunks);
        return status;
    }
    else
    {
        fprintf(stderr, "Error: the IFF file must be a FORM!\n");
        return FALSE;
    }
}

int main(int argc, char *argv[])
{
    IFF_IOError *error = NULL;
    IFF_Chunk *chunk = IFF_read("lookupproperty-override-local.TEST", &error);
    int status;

    if(error == NULL)
        status = !lookupPropertyAndCheck(chunk);
    else
    {
        status = 1;
        IFF_printReadError(stderr, error);
        IFF_freeIOError(error);
    }

    IFF_free(chunk);

    return status;
}
