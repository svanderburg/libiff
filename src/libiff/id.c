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

#include "id.h"
#include <string.h>
#include "io.h"
#include "error.h"

IFF_Bool IFF_readId(FILE *file, IFF_ID *id, const IFF_ID chunkId, const char *attributeName)
{
    return IFF_readULong(file, id, chunkId, attributeName);
}

IFF_Bool IFF_writeId(FILE *file, const IFF_ID id, const IFF_ID chunkId, const char *attributeName)
{
    return IFF_writeULong(file, id, chunkId, attributeName);
}

void IFF_idToString(const IFF_ID id, IFF_ID2 id2)
{
    id2[0] = id >> 24;
    id2[1] = (id & 0xff0000) >> 16;
    id2[2] = (id & 0xff00) >> 8;
    id2[3] = id & 0xff;
}

IFF_Bool IFF_checkId(const IFF_ID id)
{
    IFF_ID2 id2;
    unsigned int i;

    IFF_idToString(id, id2);

    /* ID characters must be between 0x20 and 0x7e */

    for(i = 0; i < IFF_ID_SIZE; i++)
    {
        if(id2[i] < 0x20 || id2[i] > 0x7e)
        {
            IFF_error("Illegal character: '%c' in ID!\n", id2[i]);
            return FALSE;
        }
    }

    /* Spaces may not precede an ID, trailing spaces are ok */

    if(id2[0] == ' ')
    {
        IFF_error("Spaces may not precede an ID!\n");
        return FALSE;
    }

    return TRUE;
}

void IFF_printId(const IFF_ID id)
{
    IFF_ID2 id2;
    unsigned int i;

    IFF_idToString(id, id2);

    for(i = 0; i < IFF_ID_SIZE; i++)
        printf("%c", id2[i]);
}
