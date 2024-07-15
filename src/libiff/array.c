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

#include "array.h"
#include <stdlib.h>

void **IFF_addElementToPointerArray(void **pointerArray, void *element, unsigned int *pointerArrayLength)
{
    void **result = (void**)realloc(pointerArray, (*pointerArrayLength + 1) * sizeof(void*));
    result[*pointerArrayLength] = element;
    *pointerArrayLength = *pointerArrayLength + 1;
    return result;
}

void **IFF_removeElementFromPointerArray(void **pointerArray, const unsigned int index, unsigned int *pointerArrayLength, void **obsoleteElement)
{
    if(index >= *pointerArrayLength)
    {
        *obsoleteElement = NULL;
        return pointerArray;
    }
    else
    {
        unsigned int i;

        /* Update the obsolete element */
        *obsoleteElement = pointerArray[index];

        /* Move all elements after the index to the left */
        *pointerArrayLength = *pointerArrayLength - 1;

        for(i = index; i < *pointerArrayLength; i++)
            pointerArray[i] = pointerArray[i + 1];

        /* Decrease the pointer array size and return it */
        return (void**)realloc(pointerArray, *pointerArrayLength * sizeof(void*));
    }
}

void *IFF_replaceElementInPointerArray(void **pointerArray, const unsigned int pointerArrayLength, const unsigned int index, void *newElement)
{
    if(index >= pointerArrayLength)
        return NULL;
    else
    {
        void *obsoleteElement = pointerArray[index];
        pointerArray[index] = newElement;
        return obsoleteElement;
    }
}
