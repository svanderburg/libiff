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
    IFF_CAT *cat = IFF_createEmptyCAT(IFF_ID_JJJJ);
    IFF_ID lastType = 0;
    IFF_Bool sameIds = TRUE;
    unsigned int i;
    int status = 0;

    for(i = 0; i < inputFilenamesLength; i++)
    {
        /* Open each input IFF file */
        IFF_Chunk *chunk = IFF_readFile(inputFilenames[i], NULL, 0);

        /* Check whether the IFF file is valid */
        if(chunk == NULL || !IFF_check(chunk, NULL, 0))
        {
            IFF_free((IFF_Chunk*)cat, NULL, 0);
            return 1;
        }
        else
        {
            /* Check whether all the form types and contents types are the same */

            if(sameIds)
            {
                if(chunk->chunkId == IFF_ID_FORM || chunk->chunkId == IFF_ID_CAT || chunk->chunkId == IFF_ID_LIST)
                {
                    IFF_Group *group = (IFF_Group*)chunk;

                    if((i > 0) && group->groupType != lastType)
                        sameIds = FALSE;

                    lastType = group->groupType;
                }
            }

            /* Add the input IFF chunk to the concatenation */
            IFF_addToCAT(cat, chunk);
        }
    }

    /* If all form types are the same, then change the contentsType of this CAT to hint about it. Otherwise the contentsType remains 'JJJJ' */
    if(sameIds)
        cat->contentsType = lastType;

    /* Write the resulting CAT to the output file or standard output */
    if(!IFF_write(outputFilename, (IFF_Chunk*)cat, NULL, 0))
        status = 1;

    /* Free everything */
    IFF_free((IFF_Chunk*)cat, NULL, 0);

    /* Return whether the join has succeeded */
    return status;
}
