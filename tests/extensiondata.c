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

#include "extensiondata.h"
#include <form.h>
#include "test.h"
#include "hello.h"
#include "bye.h"

IFF_Form *IFF_createTestForm()
{
    TEST_Hello *hello;
    TEST_Bye *bye;
    
    IFF_Form *form = IFF_createForm("TEST");
    
    hello = TEST_createHello();
    hello->a = 'a';
    hello->b = 'b';
    hello->c = 4096;
    
    bye = TEST_createBye();
    bye->one = 1;
    bye->two = 2;
    
    IFF_addToForm(form, (IFF_Chunk*)hello);
    IFF_addToForm(form, (IFF_Chunk*)bye);

    return form;
}
