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

#ifndef __IFF_FORMREGISTRY_H
#define __IFF_FORMREGISTRY_H

typedef struct IFF_FormType IFF_FormType;
typedef struct IFF_FormTypesNode IFF_FormTypesNode;

#include "id.h"
#include "groupstructure.h"
#include "registry.h"

/**
 * @brief Defines how sub chunks within a specific FORM chunk with a specify formType should be managed
 */
struct IFF_FormType
{
    /** A 4 character formType id */
    IFF_ID formType;

    /** An object that specifies the structure of the FORM */
    IFF_GroupStructure *formStructure;
};

struct IFF_FormTypesNode
{
    /** Specifies the number of form types that have a specific meaning inside a FORM */
    unsigned int formTypesLength;

    /** An array specifying how chunks within a FORM should be managed */
    IFF_FormType *formTypes;

    /** Link to the parent node that provides additional FORM chunks */
    IFF_FormTypesNode *parent;
};

#ifdef __cplusplus
extern "C" {
#endif

IFF_GroupStructure *IFF_findGroupStructure(const IFF_Registry *registry, const IFF_ID formType);

#ifdef __cplusplus
}
#endif

#endif
