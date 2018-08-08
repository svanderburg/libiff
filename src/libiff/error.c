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

#include "error.h"
#include <stdio.h>

void IFF_errorCallbackStderr(const char *formatString, va_list ap)
{
    vfprintf(stderr, formatString, ap);
}

void (*IFF_errorCallback) (const char *formatString, va_list ap) = &IFF_errorCallbackStderr;

void IFF_error(const char *formatString, ...)
{
    va_list ap;
    
    va_start(ap, formatString);
    IFF_errorCallback(formatString, ap);
    va_end(ap);
}

void IFF_errorId(const IFF_ID id)
{
    unsigned int i;
    
    for(i = 0; i < IFF_ID_SIZE; i++)
	IFF_error("%c", id[i]);
}

void IFF_readError(const IFF_ID chunkId, const char *attributeName)
{
    IFF_error("Error reading '");
    IFF_errorId(chunkId);
    IFF_error("'.%s\n", attributeName);
}

void IFF_writeError(const IFF_ID chunkId, const char *attributeName)
{
    IFF_error("Error writing '");
    IFF_errorId(chunkId);
    IFF_error("'.%s\n", attributeName);
}
