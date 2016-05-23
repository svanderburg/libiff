libiff
======
`libiff` is a portable, extensible parser library implemented in ANSI C, for
EA-IFF 85: Electronic Arts' Interchange File Format (IFF). It offers the
following features:

* Reading IFF files
* Programmatically creating IFF files
* Retrieving IFF file contents
* Writing IFF files
* IFF conformance checking

Furthermore, this library has an extensible parser interface, which makes it
possible to parse chunk types defined in application formats.

What is the Interchange File Format (IFF)?
==========================================
IFF is a generic container format designed to make the transfer of data
easier between applications produced by different vendors. The main purpose is to
divide files into chunks identified by a type ID consisting of 4 characters,
followed by a chunk size and the specified number of bytes. The IFF standard
also defines a number of predefined _group_ chunks, which can be used to group
data chunks together and to share a set of common properties over a number of
sub chunks.

The IFF container format is frequently used by applications on the Commodore
Amiga for storing images, music and audio samples. The actual IFF specification
can be found in: `doc/IFF.asc`, which is included in this package.

Some _application_ file formats using the IFF container are:

* ILBM, used for storing images. Quite often ILBM files are _wrongly_ named as IFF files because ILBM is the most common application format
* ANIM, an extension to ILBM supporting animations which are delta compressed
* 8SVX, used for storing 8-bit sound samples
* SMUS, used for storing musical scores

This library does not implement support for these subformats, but instead the
parser provides an extension interface, which can be used to handle chunks
defined in these application formats. For example, the `libilbm` package,
provides additional functions to parse ILBM image chunks by extending this IFF
parser. You can easily extend the IFF parser yourself to define your own custom
file format on top of IFF.

Installation on Unix-like systems
=================================
Compilation and installation of this library on Unix-like systems is straight
forward, by using the standard GNU autotools build instructions:

    $ ./configure
    $ make
    $ make install

More details about the installation process can be found in the `INSTALL` file
included in this package.

Building with Visual C++
========================
This package can also be built with Visual C++ for Windows platforms. First, you
must copy `src/libiff/ifftypes.h.in` to `src/libiff/ifftypes.h` and edit the the
latter file.

Change the line:

```C
#define IFF_BIG_ENDIAN @IFF_BIG_ENDIAN@
```

into

```C
#define IFF_BIG_ENDIAN 0
```

Then you can open the solution file: `src/libiff.sln` in Visual Studio to edit or
build it. Alternatively, you can use `MSBuild` to compile it:

    $ MSBuild libiff.sln

The output is produced in the `Debug/` directory.

Portability
===========
Because this package is implemented in ANSI C (with the small exception that the
command line utilities use `getopt()` ), it should be pretty easy to port this
package to new platforms. So far it's tested on the following platforms:

* Linux (`i686-linux`, `x86_64-linux`) using GCC
* Cygwin (`i686-cygwin`, `x86_64-cygwin`) using GCC
* AmigaOS (`m68k-amigaos`) using EGCS through Geek Gadgets
* Windows (`i686-windows`) using Visual C++ 2013

License
=======
This library is available under the MIT license

Using the IFF library API
=========================
Using the IFF API to create, read, write and check IFF files is pretty
straight forward. Full API documentation can be found in the `doc/apidox`
directory of this package.

Reading IFF files
-----------------
To read IFF files, include the `iff.h` header file and call the `IFF_read()`
function:

```C
#include <libiff/iff.h>

int main(int argc, char *argv[])
{
    /* Read an IFF file */
    IFF_Chunk *chunk = IFF_read("input.IFF", NULL, 0);
    
    if(chunk == NULL)
    {
        /* Use the chunk instance for some purpose here */
        
        return 0;
    }
    else
        return 1; /* The chunk cannot be read for some reason */
}
```

Programatically creating IFF files
----------------------------------
An IFF file can be created by composing various IFF struct instances together.
The following example creates a concatenation chunk containing a 'TEST' form.
The 'TEST' form contains a 'HELO' chunk containing "abcd", and a 'BYE ' chunk
containing "1234":

```C
#include <stdlib.h>
#include <libiff/iff.h>
#include <libiff/rawchunk.h>
#include <libiff/form.h>
#include <libiff/cat.h>

#define HELO_BYTES_SIZE 4
#define BYE_BYTES_SIZE 4

int main(int argc, char *argv[])
{
    /* Declarations */
    IFF_UByte *heloChunkData, *byteChunkData;
    IFF_RawChunk *heloChunk, *byeChunk;
    IFF_Form *form;
    IFF_CAT *cat;
    
    /* Allocate and create data for the 'HELO' chunk */
    heloChunkData = (IFF_UByte*)malloc(HELO_BYTES_SIZE * sizeof(IFF_UByte));
    heloChunkData[0] = 'a';
    heloChunkData[1] = 'b';
    heloChunkData[2] = 'c';
    heloChunkData[3] = 'd';
    
    /* Create the actual 'HELO' chunk */
    heloChunk = IFF_createRawChunk("HELO");
    
    /* Attach the chunk data to the 'HELO' chunk */
    IFF_setRawChunkData(heloChunk, heloChunkData, HELO_BYTES_SIZE);
    
    /* Allocate and create data for the 'BYE ' chunk */
    byeChunkData = (IFF_UByte*)malloc(BYE_BYTES_SIZE * sizeof(IFF_UByte));
    byeChunkData[0] = '1';
    byeChunkData[1] = '2';
    byeChunkData[2] = '3';
    byeChunkData[3] = '4';
    
    /* Create the actual 'BYE ' chunk and attach the data to it */
    byeChunk = IFF_createRawChunk("BYE ");
    IFF_setRawChunkData(byeChunk, byeChunkData, BYE_BYTES_SIZE);
    
    /* Create the 'TEST' form chunk */
    form = IFF_createForm("TEST");
    
    /* Attach the 'HELO' and 'BYE ' chunk to the form */
    IFF_addToForm((IFF_Chunk*)heloChunk);
    IFF_addToForm((IFF_Chunk*)byeChunk);
    
    /* Create a concatenation chunk */
    cat = IFF_createCAT("TEST");
    
    /* Attach the form to the concatenation chunk */
    IFF_addToCAT((IFF_Chunk*)form);
    
    return 0;
}
```

Retrieving IFF file contents
----------------------------
Quite often you need to retrieve specific properties from an IFF file that are
interesting to you, while you may want to ignore the pieces that are not
relevant.

The scope of most application formats are restricted to a FORM with a particular
form type. For example, ILBM files (which purpose is to store graphics) are
essentially FORM chunks with an ILBM form type. Inside these forms various
aspects of the picture are specified, such as the resolution, color values of
the palette and planar graphics data. The `IFF_searchForms()` function
recursively searches for all form chunks with the given form type inside an IFF
file.

Furthermore, each form chunk contains an arbitrary number of
data chunks or other group chunks. The `IFF_getChunkFromForm()` function can be
used to retrieve a property from a form. Furthermore, if the given form is a
member of a list chunk, which uses shared properties, it also recursively looks 
up the shared property value, if the requested property has not been defined in 
the form itself.

It may also be possible that there are more occurences of the same chunk inside 
a form. In these cases, the `IFF_getChunksFromForm()` can be used to retrieve
all possible values, however this function does not take the shared properties
of a list into account.

The following example shows how these functions can be used:

```C
#include <libiff/chunk.h>
#include <libiff/form.h>

int main(int argc, char *argv[])
{
    /* Declarations */
    IFF_Chunk *chunk;
    IFF_Form **ilbmForms;
    unsigned int ilbmFormsLength, i;
    
    /* Create or read an IFF file here */
    
    /* Search for all forms having an ILBM form type */
    ilbmForms = IFF_searchForms(chunk, "ILBM", &ilbmFormsLength);
    
    /* Iterate over all ILBM forms in the given IFF file */
    
    for(i = 0; i < ilbmFormsLength; i++)
    {
        IFF_Form *ilbmForm = ilbmForms[i];
        unsigned int colorRangesLength;
        
        /* Retrieve the BMHD data property from the ILBM form */
        IFF_Chunk *bitMapHeader = IFF_getChunkFromForm(ilbmForm, "BMHD");
        
        /* Retrieve all possible CRNG properties from the ILBM form */
        IFF_Chunk **colorRanges = IFF_getChunksFromForm(ilbmForm, "CRNG", &colorRangesLength);
    }
    
    return 0;
}
```

Writing IFF files
-----------------
A composition of chunks can be written as an IFF file by invoking the
`IFF_write()` function:

```C
#include <libiff/iff.h>

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk;
    
    /* Create or read a chunk */
    
    if(IFF_write("output.IFF", chunk, NULL, 0))
        return 0; /* The file has been successfully written */
    else
        return 1; /* Some error occured */
}
```

IFF conformance checking
------------------------
The IFF standard defines several constraints that may not be violated. For
example, chunk IDs may only contain ASCII characters from a certain range in the
ASCII table. Furthermore, the chunk size of a group chunk must be correct and
may not be truncated. The `IFF_check()` function can be used to check whether a
composition of chunks conform to the IFF standard:

```C
#include <libiff/iff.h>

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk;
    
    /* Create or read a chunk here */
    
    if(IFF_check(chunk, NULL, 0))
        return 0; /* A valid IFF file */
    else
        return 1; /* Not a valid IFF file */
}
```

Comparing IFF chunk hierarchies
-------------------------------
In some cases, it may also be useful to compare IFF chunk hierarchies. For
example, to check whether to files (or subsets thereof) are identical. These
hierarchies can be compared by using the `IFF_compare()` function:

```C
#include <libiff/iff.h>

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk1, *chunk2;
    
    /* Read or create the chunks here */

    if(IFF_compare(chunk1, chunk2, NULL, 0))
        return 0; /* Both chunk hierarchies are equal */
    else
        return 1; /* Both chunk hierarchies are not equal */
}
```

Command-line utilities
======================
Apart from an API to handle IFF files, this package also includes a number of
command-line utilities to make usage of IFF files more convenient:

* `iffpp` can be used to pretty print an IFF file into a textual representation, so that it can be manually inspected
* `iffjoin` can be used to join an arbitrary number of IFF files into a new IFF file storing these in an concationation chunk

Consult the manual pages of these tools for more information.

Extending the IFF parser
========================
This package only implements a generic IFF parser, which provides support for a
number of group chunks and "raw" data chunks. Application formats typically
define various other data chunks, each having their own structure. In order to
support a particular application format, the IFF parser needs to be extended.

To implement a parser for an application format you need to:

* Create an application format interface
* Implement extension chunk modules

Creating an application format interface
----------------------------------------
Every function in the `iff.h` header require at least two parameters, which are
named `extension` and `extensionLength`. So far, we have omitted these extension
parameters by specifying a NULL pointer and a 0 length.

These function parameters can be used to support application format chunks by
specifying an array of form types pointing to a functions which read, write,
free, check and print extension chunks for a given data property.

Because it is incovenient to bother library users with specifying these
paramaters over and over again, it is recommended to create a new application
format interface, which hides these details. For example, in the `libilbm`
package, you can include the `ilbm.h` header file, instead of the `iff.h` header
file to conveniently access ILBM files and their application chunks.

The following example shows how to define a header file (`test.h`) for the
interface of an imaginary TEST file format:

```C
#ifndef __TEST_H
#define __TEST_H

#include <stdio.h>
#include <libiff/chunk.h>

IFF_Chunk *TEST_read(const char *filename);

int TEST_write(const char *filename, const IFF_Chunk *chunk);

void TEST_free(IFF_Chunk *chunk);

void TEST_print(const IFF_Chunk *chunk, const unsigned int indentLevel);

int TEST_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#endif
```

As you may notice, this header file is almost the same as the `iff.h` header
file, except that the `IFF_` prefixes are replaced by the `TEST_` prefixes and
the `extension` and `extensionLength` parameters are removed.

The implementation of this interface (`test.c`) may look as follows:

```C
#include "test.h"
#include <libiff/iff.h>
#include "hello.h"
#include "bye.h"

#define TEST_NUM_OF_FORM_TYPES 1
#define TEST_NUM_OF_EXTENSION_CHUNKS 2

/*
 * Defines the 'HELO' and 'BYE ' extension chunks and referes to the
 * functions that handles them. The chunks must be alphabetically sorted so
 * that they can be found by a binary search algorithm.
 */
static IFF_FormExtension testFormExtension[] = {
    {"BYE ", &TEST_readBye, &TEST_writeBye, &TEST_checkBye, &TEST_freeBye, &TEST_printBye, &TEST_compareBye},
    {"HELO", &TEST_readHello, &TEST_writeHello, &TEST_checkHello, &TEST_freeHello, &TEST_printHello, &TEST_compareHello}
};

/*
 * Refers to the extension chunk defintions within the TEST form scope.
 * Also these form types must be alphabetically sorted.
 */
static IFF_Extension extension[] = {
    {"TEST", TEST_NUM_OF_EXTENSION_CHUNKS, testFormExtension}
};

/* The following functions hide the the extension parameters for this application format */
IFF_Chunk *TEST_read(const char *filename)
{
    return IFF_read(filename, extension, TEST_NUM_OF_FORM_TYPES);
}

int TEST_write(const char *filename, const IFF_Chunk *chunk)
{
    return IFF_write(filename, chunk, extension, TEST_NUM_OF_FORM_TYPES);
}

void TEST_free(IFF_Chunk *chunk)
{
    IFF_free(chunk, NULL, extension, TEST_NUM_OF_FORM_TYPES);
}

void TEST_print(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_print(chunk, indentLevel, extension, TEST_NUM_OF_FORM_TYPES);
}

int TEST_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compare(chunk1, chunk2, extension, TEST_NUM_OF_FORM_TYPES);
}
```

In the code fragment above, an array defining extension chunkIDs and function
pointers (`testFormExtension`) specifies how to read, write, check, free and
print each individual application chunk. The other array binds these extension
chunks into the scope of the TEST form type. This ensures that the TEST.HELO
property is parsed by our extension function and that a chunk with the same ID
in a different form type is parsed ABCD.HELO differently (because they are not
the same).

In the remainder of the interface, we simply call all the functions defined in
`iff.h` with the given `extension` and `extensionLength` parameters dealing with
application format chunks of the TEST format.

Implementing extension chunk modules
------------------------------------
Now that we have defined an application interface, you also need to specify how
to read, write, free, check and print these application chunks. For each
application chunk, you define a seperate module with an header the may look like
this (this example defines `hello.h` to which the previous example refers):

```C
#ifndef __TEST_HELLO_H
#define __TEST_HELLO_H

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>

typedef struct
{
    /* These struct members are mandatory for every chunk */
    IFF_Group *parent;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    /* The remainder of the struct contains custom properties */
    IFF_UByte a;
    IFF_UByte b;
    IFF_UWord c;
}
TEST_Hello;

TEST_Hello *TEST_createHello(void);

IFF_Chunk *TEST_readHello(FILE *file, const IFF_Long chunkSize);

int TEST_writeHello(FILE *file, const IFF_Chunk *chunk);

int TEST_checkHello(const IFF_Chunk *chunk);

void TEST_freeHello(IFF_Chunk *chunk);

void TEST_printHello(const IFF_Chunk *chunk, const unsigned int indentLevel);

int TEST_compareHello(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#endif
```

The code fragment above defines the read, write, check, free, and print
functions for the 'HELO' chunk. As you may notice, every function has the
`TEST_` prefix, because it is part of the TEST application format. Furthermore,
in each module every read, write, check, free and print function must have the
same return type and function parameters.

And the implementation may look as follows:

```C
#include "hello.h"
#include <libiff/io.h>
#include <libiff/util.h>
#include "test.h"

#define CHUNKID "HELO"

TEST_Hello *TEST_createHello(void)
{
    TEST_Hello *hello = (TEST_Hello*)IFF_allocateChunk(CHUNKID, sizeof(TEST_Hello));

    if(hello != NULL)
        hello->chunkSize = 2 * sizeof(IFF_UByte) + sizeof(IFF_UWord);

    return hello;
}

IFF_Chunk *TEST_readHello(FILE *file, const IFF_Long chunkSize)
{
    TEST_Hello *hello = TEST_createHello();

    if(hello != NULL)
    {
        if(!IFF_readUByte(file, &hello->a, CHUNKID, "a"))
        {
            TEST_free((IFF_Chunk*)hello);
            return NULL;
        }

        if(!IFF_readUByte(file, &hello->b, CHUNKID, "b"))
        {
            TEST_free((IFF_Chunk*)hello);
            return NULL;
        }

        if(!IFF_readUWord(file, &hello->c, CHUNKID, "c"))
        {
            TEST_free((IFF_Chunk*)hello);
            return NULL;
        }
    }

    return (IFF_Chunk*)hello;
}

int TEST_writeHello(FILE *file, const IFF_Chunk *chunk)
{
    const TEST_Hello *hello = (TEST_Hello*)chunk;

    if(!IFF_writeUByte(file, hello->a, CHUNKID, "a"))
        return FALSE;

    if(!IFF_writeUByte(file, hello->b, CHUNKID, "b"))
        return FALSE;

    if(!IFF_writeUWord(file, hello->c, CHUNKID, "c"))
        return FALSE;

    return TRUE;
}

int TEST_checkHello(const IFF_Chunk *chunk)
{
    return TRUE;
}

void TEST_freeHello(IFF_Chunk *chunk)
{
}

void TEST_printHello(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const TEST_Hello *hello = (const TEST_Hello*)chunk;

    IFF_printIndent(stdout, indentLevel, "a = %c;\n", hello->a);
    IFF_printIndent(stdout, indentLevel, "b = %c;\n", hello->b);
    IFF_printIndent(stdout, indentLevel, "c = %u;\n", hello->c);
}

int TEST_compareHello(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const TEST_Hello *hello1 = (const TEST_Hello*)chunk1;
    const TEST_Hello *hello2 = (const TEST_Hello*)chunk2;

    if(hello1->a != hello2->a)
        return FALSE;

    if(hello1->b != hello2->b)
        return FALSE;

    if(hello1->c != hello2->c)
        return FALSE;

    return TRUE;
}
```
