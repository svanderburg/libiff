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
#include <stdlib.h>
#include "join.h"

static void printUsage(const char *command)
{
    printf("Usage: %s [OPTION] file1.IFF file2.IFF ...\n\n", command);

    puts(
    "The command `iffjoin' joins an aribitrary number of IFF files into a single\n"
    "concatenation IFF file. The result is written to the standard output, or\n"
    "optionally to a given destination file.\n\n"

    "Options:\n"
#if _MSC_VER
    "  /o FILE    Specify an output file name\n"
    "  /?         Shows the usage of this command to the user\n"
    "  /v         Shows the version of this command to the user"
#else
    "  -o, --output-file=FILE    Specify an output file name\n"
    "  -h, --help                Shows the usage of this command to the user\n"
    "  -v, --version             Shows the version of this command to the user"
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
    char *outputFilename = NULL;

#if _MSC_VER
    unsigned int optind = 1;
    unsigned int i;

    for(i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "/o") == 0)
        {
            outputFilename = argv[i];
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
        {"output-file", required_argument, 0, 'o'},
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
    {
        fprintf(stderr, "ERROR: No IFF input files given!\n");
        return 1;
    }
    else
    {
        unsigned int inputFilenamesLength = argc - optind;
        char **inputFilenames = (char**)malloc(inputFilenamesLength * sizeof(char*));
        unsigned int i;
        int status;
        
        /* Create an array of input file names */
        for(i = 0; i < inputFilenamesLength; i++)
            inputFilenames[i] = argv[optind + i];
        
        /* Join the IFF files */
        status = IFF_join(inputFilenames, inputFilenamesLength, outputFilename);
        
        /* Cleanup */
        free(inputFilenames);
        
        /* Return whether the join has succeeded or not */
        return status;
    }
}
