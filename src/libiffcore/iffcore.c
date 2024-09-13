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

#include "iffcore.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

IFF_Chunk *IFF_parseFdCore(FILE *file, const IFF_Registry *registry, IFF_IOError **error)
{
    int byte;
    IFF_AttributePath *attributePath = IFF_createAttributePath();

    /* Read the chunk */
    IFF_Chunk *chunk = IFF_parseChunk(file, 0, registry, attributePath, error);

    /* We should have reached the EOF now */
    if((byte = fgetc(file)) != EOF)
        fprintf(stderr, "WARNING: Trailing contents found: %d!\n", byte);

    /* Remove the attribute path if we no longer need it */
    if(*error == NULL)
        IFF_freeAttributePath(attributePath);

    /* Return the parsed main chunk */
    return chunk;
}

IFF_Chunk *IFF_parseFileCore(const char *filename, const IFF_Registry *registry, IFF_IOError **error)
{
    FILE *file = fopen(filename, "rb");

    if(file == NULL)
    {
        *error = IFF_createFileIOError();
        return NULL;
    }
    else
    {
        IFF_Chunk *chunk = IFF_parseFdCore(file, registry, error);
        fclose(file);
        return chunk;
    }
}

IFF_Chunk *IFF_parseCore(const char *filename, const IFF_Registry *registry, IFF_IOError **error)
{
    if(filename == NULL)
        return IFF_parseFdCore(stdin, registry, error);
    else
        return IFF_parseFileCore(filename, registry, error);
}

IFF_Bool IFF_writeFdCore(FILE *file, const IFF_Chunk *chunk, IFF_IOError **error)
{
    IFF_AttributePath *attributePath = IFF_createAttributePath();
    IFF_Bool status = IFF_writeChunk(file, chunk, 0, attributePath, error);

    if(*error == NULL)
        IFF_freeAttributePath(attributePath);

    return status;
}

IFF_Bool IFF_writeFileCore(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error)
{
    FILE *file = fopen(filename, "wb");

    if(file == NULL)
    {
        *error = IFF_createFileIOError();
        return FALSE;
    }
    else
    {
        IFF_Bool status = IFF_writeFdCore(file, chunk, error);
        fclose(file);
        return status;
    }
}

IFF_Bool IFF_writeCore(const char *filename, const IFF_Chunk *chunk, IFF_IOError **error)
{
    if(filename == NULL)
        return IFF_writeFdCore(stdout, chunk, error);
    else
        return IFF_writeFileCore(filename, chunk, error);
}

void IFF_freeCore(IFF_Chunk *chunk)
{
    IFF_freeChunk(chunk, 0);
}

IFF_QualityLevel IFF_advancedCheckCore(const IFF_Chunk *chunk, const IFF_Registry *registry, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_AttributePath *attributePath = IFF_createAttributePath();
    IFF_QualityLevel qualityLevel;

    if(chunk == NULL)
    {
        printCheckMessage(attributePath, NULL, 0, data, "The file cannot be processed");
        qualityLevel = IFF_QUALITY_GARBAGE;
    }
    else
    {
        qualityLevel = registry->checkMainChunk(chunk, attributePath, printCheckMessage, data);
        qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_checkChunk(chunk, 0, attributePath, printCheckMessage, data));
    }

    IFF_freeAttributePath(attributePath);
    return qualityLevel;
}

IFF_QualityLevel IFF_checkCore(const IFF_Chunk *chunk, const IFF_Registry *registry)
{
    return IFF_advancedCheckCore(chunk, registry, IFF_printCheckMessageOnStderr, NULL);
}

void IFF_printFdCore(FILE *file, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_printChunk(file, chunk, indentLevel, 0);
}

IFF_Bool IFF_printFileCore(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    FILE *file = fopen(filename, "wb");

    if(file == NULL)
        return FALSE;
    else
    {
        IFF_printFdCore(file, chunk, indentLevel);
        fclose(file);
        return TRUE;
    }
}

IFF_Bool IFF_printCore(const char *filename, const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    if(filename == NULL)
    {
        IFF_printFdCore(stdout, chunk, indentLevel);
        return TRUE;
    }
    else
        return IFF_printFileCore(filename, chunk, indentLevel);
}

IFF_Bool IFF_compareCore(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compareChunk(chunk1, chunk2, 0);
}

IFF_Bool IFF_traverseCore(IFF_Chunk *chunk, void *data, IFF_visitChunkFunction visitChunk)
{
    return IFF_traverseChunkHierarchy(chunk, 0, data, visitChunk);
}

void IFF_recalculateChunkSizesCore(IFF_Chunk *chunk)
{
    IFF_recalculateChunkHierarchySizes(chunk, 0);
}
