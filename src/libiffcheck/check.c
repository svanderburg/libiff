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

#include "check.h"
#include "iffcore.h"

int IFF_conformanceCheck(const char *filename, int minLevel, int maxLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    /* Parse the chunk */
    IFF_IOError *error = NULL;
    IFF_Chunk *chunk = IFF_parseCore(filename, chunkRegistry, &error);

    IFF_QualityLevel qualityLevel;
    IFF_Bool status;

    if(error != NULL)
    {
        IFF_printReadError(stderr, error);
        IFF_freeIOError(error);
    }

    /* Check the file and print the quality level */
    qualityLevel = IFF_checkCore(chunk, chunkRegistry);

    if(qualityLevel == IFF_QUALITY_PERFECT)
        fprintf(stderr, "No conformance issues were found!\n");

    printf("%d", qualityLevel);

    /* Check if the quality is between the specified minimum and maximum level */
    status = qualityLevel >= minLevel && qualityLevel <= maxLevel;

    /* Free the chunk structure */
    IFF_freeCore(chunk);

    return !status;
}
