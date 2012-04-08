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
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "pp.h"

static void printUsage(const char *command)
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "%s [options] file.IFF\n\n", command);
    fprintf(stderr, "-c, --disable-check    Do not check the IFF file for validity\n");
    fprintf(stderr, "-h, --help             Shows the usage of this command to the user\n");
}

int main(int argc, char *argv[])
{
    /* Declarations */
    int c;
#if HAVE_GETOPT_H == 1
    int option_index = 0;
    struct option long_options[] =
    {
	{"disable-check", no_argument, 0, 'c'},
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
    };
#endif
    int options = 0;
    char *filename;
    
    /* Parse command-line options */
    
#if HAVE_GETOPT_H == 1
    while((c = getopt_long(argc, argv, "ch", long_options, &option_index)) != -1)
#else
    while((c = getopt(argc, argv, "ch")) != -1)
#endif
    {
	switch(c)
	{
	    case 'c':
		options |= IFFPP_DISABLE_CHECK;
		break;
		
	    case 'h':
	    case '?':
		printUsage(argv[0]);
		return 0;
	}
    }
    
    /* Validate non options */
    
    if(optind >= argc)
	filename = NULL;
    else
	filename = argv[optind];

    /* Pretty print the IFF file */
    return IFF_prettyPrint(filename, options);
}
