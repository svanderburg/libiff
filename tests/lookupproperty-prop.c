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
#include "chunk.h"
#include "id.h"
#include "group.h"
#include "form.h"
#include "iff.h"

#define ID_HELO IFF_MAKEID('H', 'E', 'L', 'O')
#define ID_TEST IFF_MAKEID('T', 'E', 'S', 'T')

static IFF_Bool lookupPropertyAndCheck(IFF_Chunk *chunk)
{
    unsigned int formsLength;
    IFF_Form **forms = IFF_searchForms(chunk, ID_TEST, &formsLength);

    if(formsLength > 0)
    {
        IFF_Chunk *heloChunk = IFF_getChunkFromForm(forms[0], ID_HELO);

        if(heloChunk == NULL || heloChunk->chunkId != ID_HELO)
        {
            fprintf(stderr, "Error: the chunk we should find must be the HELO chunk!\n");
            return FALSE;
        }
        else
            return TRUE;
    }
    else
    {
        fprintf(stderr, "Error: we should be able to find a TEST form!\n");
        return FALSE;
    }
}

int main(int argc, char *argv[])
{
    IFF_IOError *error = NULL;
    IFF_Chunk *chunk = IFF_parse("list.TEST", &error);
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
