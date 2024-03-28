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

#ifndef __IFF_ATTRIBUTE_PATH_H
#define __IFF_ATTRIBUTE_PATH_H

typedef enum
{
    IFF_ATTRIBUTE_NAME,
    IFF_ATTRIBUTE_INDEX
}
IFF_AttributeType;

typedef struct IFF_AttributePathNode IFF_AttributePathNode;

struct IFF_AttributePathNode
{
    IFF_AttributePathNode *previous;
    IFF_AttributePathNode *next;

    IFF_AttributeType type;

    union
    {
        unsigned int index;
        char *name;
    }
    attribute;
};

typedef struct IFF_AttributePath IFF_AttributePath;

struct IFF_AttributePath
{
    IFF_AttributePathNode *rootNode;
    IFF_AttributePathNode *currentNode;
};

IFF_AttributePath *IFF_createAttributePath(void);

void IFF_freeAttributePath(IFF_AttributePath *attributePath);

void IFF_visitAttributeByIndex(IFF_AttributePath *attributePath, unsigned int index);

void IFF_visitAttributeByName(IFF_AttributePath *attributePath, char *name);

void IFF_unvisitAttribute(IFF_AttributePath *attributePath);

void IFF_printAttributePath(const IFF_AttributePath *attributePath);

#endif
