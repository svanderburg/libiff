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

#include "pp.h"
#include "iffcore.h"

int IFF_prettyPrint(const char *inputFilename, const char *outputFilename, const IFF_Registry *registry)
{
    int status;

    /* Parse the chunk */
    IFF_IOError *error = NULL;
    IFF_Chunk *chunk = IFF_parseCore(inputFilename, registry, &error);

    if(error != NULL)
    {
        IFF_printReadError(stderr, error);
        IFF_freeIOError(error);
    }

    /* Check the file */
    if(IFF_checkCore(chunk, registry) < IFF_QUALITY_GARBAGE)
    {
        /* Print the file */
        if(IFF_print(outputFilename, chunk, 0))
            status = 0;
        else
        {
            fprintf(stderr, "Cannot open output file: %s\n", outputFilename);
            status = 1;
        }
    }
    else
        status = 1;

    /* Free the chunk structure */
    IFF_free(chunk);

    return status;
}
