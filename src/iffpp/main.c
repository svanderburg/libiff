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

#if HAVE_GETOPT_H == 1
#include <getopt.h>
#elif _MSC_VER
#include <string.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "pp.h"

static void printUsage(const char *command)
{
    printf("Usage: %s [OPTION] [file.IFF]\n\n", command);

    puts(
    "The command `iffpp' displays a textual representation of a given IFF file, which\n"
    "can be used for manual inspection of its contents. If no IFF file is specified,\n"
    "it reads an IFF file from the standard input.\n\n"

    "Options:\n"
#if _MSC_VER
    "  /c    Do not check the IFF file for validity\n"
    "  /?    Shows the usage of this command to the user\n"
    "  /v    Shows the version of this command to the user"
#else
    "  -c, --disable-check    Do not check the IFF file for validity\n"
    "  -h, --help             Shows the usage of this command to the user\n"
    "  -v, --version          Shows the version of this command to the user"
#endif
    );
}

static void printVersion(const char *command)
{
    printf(
    "%s (" PACKAGE_NAME ") " PACKAGE_VERSION "\n\n"
    "Copyright (C) 2012-2015 Sander van der Burg\n"
    , command);
}

int main(int argc, char *argv[])
{
    int options = 0;
    char *filename;

#if _MSC_VER
    unsigned int optind = 1;
    unsigned int i;

    for(i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "/c") == 0)
        {
            options |= IFFPP_DISABLE_CHECK;
            optind++;
        }
        else if (strcmp(argv[i], "/?") == 0)
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "/v") == 0)
        {
            printVersion(argv[0]);
            return 0;
        }
    }
#else
    int c;
#if HAVE_GETOPT_H == 1
    int option_index = 0;
    struct option long_options[] =
    {
        {"disable-check", no_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };
#endif

    /* Parse command-line options */
    
#if HAVE_GETOPT_H == 1
    while((c = getopt_long(argc, argv, "chv", long_options, &option_index)) != -1)
#else
    while((c = getopt(argc, argv, "chv")) != -1)
#endif
    {
        switch(c)
        {
            case 'c':
                options |= IFFPP_DISABLE_CHECK;
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            case '?':
                printUsage(argv[0]);
                return 1;
            case 'v':
                printVersion(argv[0]);
                return 0;
        }
    }

#endif
    /* Validate non options */
    
    if(optind >= argc)
        filename = NULL;
    else
        filename = argv[optind];

    /* Pretty print the IFF file */
    return IFF_prettyPrint(filename, options);
}
