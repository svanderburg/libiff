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
 
#include "join.h"
#include <stdio.h>
#include "iff.h"
#include "chunk.h"
#include "form.h"
#include "cat.h"
#include "list.h"
#include "id.h"

int IFF_join(char **inputFilenames, const unsigned int inputFilenamesLength, const char *outputFilename)
{
    IFF_IOError *error = NULL;
    IFF_CAT *cat = IFF_createEmptyCAT();
    unsigned int i;
    int status = 0;

    for(i = 0; i < inputFilenamesLength; i++)
    {
        /* Open each input IFF file */
        IFF_IOError *error = NULL;
        IFF_Chunk *chunk = IFF_readFile(inputFilenames[i], &error);

        /* Check whether the IFF file is valid */
        if(error == NULL || IFF_check(chunk))
            IFF_addToCATAndUpdateContentsType(cat, chunk); /* Add the input IFF chunk to the concatenation */
        else
        {
            if(error != NULL)
            {
                IFF_printReadError(stderr, error);
                IFF_freeIOError(error);
            }

            IFF_free((IFF_Chunk*)cat);
            return 1;
        }
    }

    /* Write the resulting CAT to the output file or standard output */
    if(!IFF_write(outputFilename, (IFF_Chunk*)cat, &error))
    {
        status = 1;
        IFF_printWriteError(stderr, error);
        IFF_freeIOError(error);
    }

    /* Free everything */
    IFF_free((IFF_Chunk*)cat);

    /* Return whether the join has succeeded */
    return status;
}
