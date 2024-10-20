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
#include "value.h"

IFF_Bool IFF_readId(FILE *file, void *value)
{
    return IFF_readULong(file, value);
}

IFF_Bool IFF_writeId(FILE *file, const void *value)
{
    return IFF_writeULong(file, value);
}

void IFF_idToString(const IFF_ID id, IFF_ID2 id2)
{
    id2[0] = id >> 24;
    id2[1] = (id & 0xff0000) >> 16;
    id2[2] = (id & 0xff00) >> 8;
    id2[3] = id & 0xff;
}

IFF_QualityLevel IFF_checkId(const IFF_ID id, IFF_AttributePath *attributePath, char *attributeName, IFF_printCheckMessageFunction printCheckMessage, void *data, const IFF_ID chunkId)
{
    IFF_ID2 id2;
    unsigned int i;

    IFF_idToString(id, id2);

    /* ID characters must be readable: a value between 0x20 and 0x7e */

    for(i = 0; i < IFF_ID_SIZE; i++)
    {
        if(id2[i] < 0x20 || id2[i] > 0x7e)
        {
            printCheckMessage(attributePath, attributeName, chunkId, data, "contains an illegal character: %c", id2[i]);
            return IFF_QUALITY_OK;
        }
    }

    /* Spaces may not precede an ID, trailing spaces are ok */

    if(id2[0] == ' ')
    {
        printCheckMessage(attributePath, attributeName, chunkId, data, "contains preceding white spaces: \"%.4s\"", id2);
        return IFF_QUALITY_OK;
    }

    return IFF_QUALITY_PERFECT;
}

IFF_Bool IFF_compareId(const void *value1, const void *value2)
{
    return IFF_compareULong(value1, value2);
}

void IFF_printIdValue(FILE *file, const void *value, const unsigned int indentLevel)
{
    const IFF_ID *idValue = (const IFF_ID*)value;
    IFF_ID2 value2;
    IFF_idToString(*idValue, value2);
    fprintf(file, "\"%.4s\"", value2);
}
