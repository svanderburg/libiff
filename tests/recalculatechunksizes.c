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

#include <stdlib.h>
#include "iff.h"
#include "chunk.h"
#include "form.h"
#include "cat.h"
#include "rawchunk.h"
#include "id.h"

#define ABCD_BYTES_SIZE 4

#define ID_ABCD IFF_MAKEID('A', 'B', 'C', 'D')
#define ID_TEST IFF_MAKEID('T', 'E', 'S', 'T')

static IFF_RawChunk *createABCDChunk(void)
{
    IFF_RawChunk *rawChunk = (IFF_RawChunk*)IFF_createRawChunk(ID_ABCD, ABCD_BYTES_SIZE);

    IFF_UByte *chunkData = rawChunk->chunkData;
    chunkData[0] = 'A';
    chunkData[1] = 'B';
    chunkData[2] = 'C';
    chunkData[3] = 'D';

    return rawChunk;
}

static IFF_Chunk *createABCDForm(IFF_RawChunk *abcdChunk)
{
    IFF_Form *form = IFF_createEmptyForm(ID_TEST, NULL);
    IFF_addChunkToForm(form, NULL, (IFF_Chunk*)abcdChunk);
    return (IFF_Chunk*)form;
}

static IFF_CAT *createABCDCAT(IFF_RawChunk *abcdChunk)
{
    IFF_Chunk *form = createABCDForm(abcdChunk);
    IFF_CAT *cat = IFF_createEmptyCATWithContentsType(ID_TEST);
    IFF_addChunkToCAT(cat, form);
    return cat;
}

int main(int argc, char *argv[])
{
    IFF_RawChunk *abcdChunk = createABCDChunk();
    IFF_CAT *cat = createABCDCAT(abcdChunk);
    int status = 0;
    IFF_QualityLevel qualityLevel;

    /* Intentionally increase the size of the ABCD chunk */

    abcdChunk->chunkData = (IFF_UByte*)realloc(abcdChunk->chunkData, (ABCD_BYTES_SIZE + 1) * sizeof(IFF_UByte));
    abcdChunk->chunkSize++;

    /* The IFF file should be invalid now as the, form chunk size is too small */
    if((qualityLevel = IFF_check((IFF_Chunk*)cat)) == IFF_QUALITY_RECOVERED)
    {
        /* Recalculate the chunk sizes */
        IFF_recalculateChunkSizes((IFF_Chunk*)abcdChunk);

        /* Now the IFF file should be valid */

        if((qualityLevel = IFF_check((IFF_Chunk*)cat)) != IFF_QUALITY_PERFECT)
        {
            fprintf(stderr, "The IFF file should be in state: %d, but is in: %d!\n", IFF_QUALITY_RECOVERED, qualityLevel);
            status = 1;
        }
    }
    else
    {
        fprintf(stderr, "The IFF file should be in state: %d, but is in: %d!\n", IFF_QUALITY_RECOVERED, qualityLevel);
        status = 1;
    }

    IFF_free((IFF_Chunk*)cat);

    return status;
}
