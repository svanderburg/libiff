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

#include "ppinterface.h"

#if HAVE_GETOPT_H == 1
#include <getopt.h>
#elif _MSC_VER
#include <string.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "pp.h"

static void printUsage(const char *command, const char *fileFormat)
{
    printf("Usage: %s [OPTION] [file.IFF]\n\n", command);

    printf(
    "The command `%s' displays textual representations of %s files,\n"
    "which can be used for manual inspection. If no file is specified it reads from\n"
    "the standard input.\n\n", command, fileFormat);

    puts(
    "Options:\n"
#if _MSC_VER
    "  /o    Specifies where to store the output. By default,\n"
    "        it will be redirected to the standard output.\n"
    "  /?    Shows the usage of this command to the user\n"
    "  /v    Shows the version of this command to the user"
#else
    "  -o, --output           Specifies where to store the output. By default,\n"
    "                         it will be redirected to the standard output.\n"
    "  -h, --help             Shows the usage of this command to the user\n"
    "  -v, --version          Shows the version of this command to the user"
#endif
    );
}

int IFF_prettyPrintCmdLineInterface(const char *command, const char *fileFormat, int argc, char **argv, const IFF_ChunkRegistry *chunkRegistry, void (*printVersion) (const char *command))
{
    char *inputFilename;
    char *outputFilename = NULL;

#if _MSC_VER
    unsigned int optind = 1;
    unsigned int i;

    for(i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "/o") == 0)
        {
            outputFilename = argv[i + 1];
            optind++;
        }
        else if (strcmp(argv[i], "/?") == 0)
        {
            printUsage(command);
            return 0;
        }
        else if (strcmp(argv[i], "/v") == 0)
        {
            printVersion(command);
            return 0;
        }
    }
#else
    int c;
#if HAVE_GETOPT_H == 1
    int option_index = 0;
    struct option long_options[] =
    {
        {"output", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };
#endif

    /* Parse command-line options */

#if HAVE_GETOPT_H == 1
    while((c = getopt_long(argc, argv, "o:hv", long_options, &option_index)) != -1)
#else
    while((c = getopt(argc, argv, "o:hv")) != -1)
#endif
    {
        switch(c)
        {
            case 'o':
                outputFilename = optarg;
                break;
            case 'h':
                printUsage(command, fileFormat);
                return 0;
            case '?':
                printUsage(command, fileFormat);
                return 1;
            case 'v':
                printVersion(command);
                return 0;
        }
    }

#endif
    /* Validate non options */

    if(optind >= argc)
        inputFilename = NULL;
    else
        inputFilename = argv[optind];

    /* Pretty print the IFF file */
    return IFF_prettyPrint(inputFilename, outputFilename, chunkRegistry);
}
