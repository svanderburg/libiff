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

static void **decreasePointerArrayAndShiftLeft(void **pointerArray, const unsigned index, unsigned int *pointerArrayLength)
{
    unsigned int i;
    *pointerArrayLength = *pointerArrayLength - 1;

    for(i = index; i < *pointerArrayLength; i++)
        pointerArray[i] = pointerArray[i + 1];

    return (void**)realloc(pointerArray, *pointerArrayLength * sizeof(void*));
}


void **IFF_removeElementFromPointerArrayByIndex(void **pointerArray, const unsigned int index, unsigned int *pointerArrayLength, void **obsoleteElement)
{
    if(index >= *pointerArrayLength)
    {
        *obsoleteElement = NULL;
        return pointerArray;
    }
    else
    {
        /* Update the obsolete element */
        *obsoleteElement = pointerArray[index];

        /* Move all elements after the index to the left */
        return decreasePointerArrayAndShiftLeft(pointerArray, index, pointerArrayLength);
    }
}

void **IFF_removeElementFromPointerArrayByValue(void **pointerArray, void *obsoleteElement, unsigned int *pointerArrayLength)
{
    unsigned int i;

    for(i = 0; i < *pointerArrayLength; i++)
    {
        if(pointerArray[i] == obsoleteElement)
            return decreasePointerArrayAndShiftLeft(pointerArray, i, pointerArrayLength);
    }

    return NULL;
}

void *IFF_replaceElementInPointerArrayByIndex(void **pointerArray, const unsigned int pointerArrayLength, const unsigned int index, void *newElement)
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

void IFF_replaceElementInPointerArrayByValue(void **pointerArray, const unsigned int pointerArrayLength, void *oldElement, void *newElement)
{
    unsigned int i;

    for(i = 0; i < pointerArrayLength; i++)
    {
        if(pointerArray[i] == oldElement)
        {
            pointerArray[i] = newElement;
            break;
        }
    }
}

void **IFF_appendPointerArrayToPointerArray(void **basePointerArray, const unsigned int basePointerArrayLength, void **appendPointerArray, const unsigned int appendPointerArrayLength, unsigned int *resultPointerArrayLength)
{
    unsigned int i;
    basePointerArray = (void**)realloc(basePointerArray, (basePointerArrayLength + appendPointerArrayLength) * sizeof(void*));

    for(i = 0; i < appendPointerArrayLength; i++)
        basePointerArray[basePointerArrayLength + i] = appendPointerArray[i];

    *resultPointerArrayLength = basePointerArrayLength + appendPointerArrayLength;

    return basePointerArray;
}
