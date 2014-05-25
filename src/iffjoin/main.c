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
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "%s [options] file1.IFF file2.IFF ...\n\n", command);
#if _MSC_VER
	fprintf(stderr, "/o    Specify an output file name\n");
	fprintf(stderr, "/?    Shows the usage of this command to the user\n");
#else
    fprintf(stderr, "-o, --output-file    Specify an output file name\n");
    fprintf(stderr, "-h, --help           Shows the usage of this command to the user\n");
#endif
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
	}
#else
    int c;
#if HAVE_GETOPT_H == 1
    int option_index = 0;
    struct option long_options[] =
    {
	{"output-file", required_argument, 0, 'o'},
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
    };
#endif
    
    /* Parse command-line options */
#if HAVE_GETOPT_H == 1
    while((c = getopt_long(argc, argv, "o:h", long_options, &option_index)) != -1)
#else
    while((c = getopt(argc, argv, "o:h")) != -1)
#endif
    {
	switch(c)
	{
	    case 'o':
		outputFilename = optarg;
		break;
		
	    case 'h':
	    case '?':
		printUsage(argv[0]);
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
