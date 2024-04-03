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

#include "checkinterface.h"
#if HAVE_GETOPT_H == 1
#include <getopt.h>
#elif _MSC_VER
#include <string.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "check.h"
#include "error.h"

static void printUsage(const char *command, const char *fileFormat)
{
    printf("Usage: %s [OPTION] [file.IFF]\n\n", command);

    printf(
    "The command `%s' checks %s files for conformance.\n\n", command, fileFormat);

    puts(
    "Options:\n");
#if _MSC_VER
    printf(
    "  /l    Specifies the minimum desired quality level\n"
    "        (defaults to: %d)\n"
    "  /u    Specifies the maximum desired quality level\n"
    "        (defaults to: %d)\n"
    "  /L    Specifies the exact desired quality level (alias for\n"
    "        combining /l and /u)\n",
    IFF_QUALITY_PERFECT, IFF_QUALITY_GARBAGE);

    puts(
    "  /?    Shows the usage of this command to the user\n"
    "  /v    Shows the version of this command to the user"
    );
#else
    printf(
    "  -l, --min              Specifies the minimum desired quality level\n"
    "                         (defaults to: %d)\n"
    "  -u, --max              Specifies the maximum desired quality level\n"
    "                         (defaults to: %d)\n"
    "  -L, --level            Specifies the exact desired quality level (shortcut\n"
    "                         for combining -l and -u)\n",
    IFF_QUALITY_PERFECT, IFF_QUALITY_GARBAGE);

    puts(
    "  -h, --help             Shows the usage of this command to the user\n"
    "  -v, --version          Shows the version of this command to the user"
#endif
    );
}

int IFF_checkCmdLineInterface(const char *command, const char *fileFormat, int argc, char **argv, const IFF_ChunkRegistry *chunkRegistry, void (*printVersion) (const char *command))
{
    char *filename;
    int minLevel = IFF_QUALITY_PERFECT;
    int maxLevel = IFF_QUALITY_GARBAGE;

#if _MSC_VER
    unsigned int optind = 1;
    unsigned int i;

    for(i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "/l") == 0)
            minLevel = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "/u") == 0)
            maxLevel = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "/L") == 0)
        {
            minLevel = atoi(argv[i + 1]);
            maxLevel = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "/?") == 0)
        {
            printUsage(command, fileFormat);
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
        {"min", required_argument, 0, 'l'},
        {"max", required_argument, 0, 'u'},
        {"level", required_argument, 0, 'L'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };
#endif

    /* Parse command-line options */

#if HAVE_GETOPT_H == 1
    while((c = getopt_long(argc, argv, "l:u:L:hv", long_options, &option_index)) != -1)
#else
    while((c = getopt(argc, argv, "l:u:L:hv")) != -1)
#endif
    {
        switch(c)
        {
            case 'l':
                minLevel = atoi(optarg);
                break;
            case 'u':
                maxLevel = atoi(optarg);
                break;
            case 'L':
                minLevel = atoi(optarg);
                maxLevel = atoi(optarg);
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
        filename = NULL;
    else
        filename = argv[optind];

    /* Pretty print the IFF file */
    return IFF_conformanceCheck(filename, minLevel, maxLevel, chunkRegistry);
}
