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

#include "structure.h"
#include <stdlib.h>
#include <util.h>

IFF_FieldStatus IFF_readStructure(FILE *file, const IFF_Structure *structure, void *object, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int i;

    for(i = 0; i < structure->fieldsLength; i++)
    {
        IFF_Field *field = &structure->fields[i];

        if(field->cardinality == IFF_CARDINALITY_SINGLE)
        {
            IFF_FieldStatus status;
            void *value = structure->getFieldPointer(object, i);

            if((status = field->type->readField(file, field, value, chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
                return status;
        }
        else if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            IFF_FieldStatus status;
            IFF_Long arrayLength = structure->getSpecifiedArrayFieldLength(object, i);
            IFF_Long *actualArrayLengthPtr;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &actualArrayLengthPtr);

            *arrayPtr = (void**)malloc(arrayLength * field->type->elementSize);

            if((status = IFF_readArrayField(file, field, *arrayPtr, arrayLength, chunk, attributePath, actualArrayLengthPtr, bytesProcessed, error)) != IFF_FIELD_MORE)
                return status;
        }
    }

    return IFF_FIELD_MORE;
}

IFF_FieldStatus IFF_writeStructure(FILE *file, const IFF_Structure *structure, void *object, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
{
    unsigned int i;

    for(i = 0; i < structure->fieldsLength; i++)
    {
        IFF_Field *field = &structure->fields[i];

        if(field->cardinality == IFF_CARDINALITY_SINGLE)
        {
            IFF_FieldStatus status;
            void *value = structure->getFieldPointer(object, i);

            if((status = field->type->writeField(file, field, value, chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
                return status;
        }
        else if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            IFF_FieldStatus status;
            IFF_Long *arrayLengthPtr;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &arrayLengthPtr);

            if((status = IFF_writeArrayField(file, field, *arrayPtr, *arrayLengthPtr, chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
                return status;
        }
    }

    return IFF_FIELD_MORE;
}

static void clearArrayField(const IFF_Field *field, void *array, const IFF_Long arrayLength)
{
    if(field->type->clearArrayField == NULL)
        IFF_clearValueArray(array, arrayLength);
    else
        field->type->clearArrayField(array, arrayLength);
}

void IFF_clearStructure(const IFF_Structure *structure, void *object)
{
    unsigned int i;

    for(i = 0; i < structure->fieldsLength; i++)
    {
        IFF_Field *field = &structure->fields[i];

        if(field->cardinality == IFF_CARDINALITY_SINGLE)
        {
            void *value = structure->getFieldPointer(object, i);
            field->type->clearField(value);
        }
        else if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            IFF_Long *arrayLengthPtr;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &arrayLengthPtr);

            clearArrayField(field, *arrayPtr, *arrayLengthPtr);
        }
    }
}

IFF_QualityLevel IFF_checkStructureArrayLengths(const IFF_Structure *structure, void *object, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_printCheckMessageFunction printCheckMessage, void *data)
{
    IFF_QualityLevel qualityLevel = IFF_QUALITY_PERFECT;
    unsigned int i;

    for(i = 0; i < structure->fieldsLength; i++)
    {
        IFF_Field *field = &structure->fields[i];

        if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            IFF_Long arrayLength = structure->getSpecifiedArrayFieldLength(object, i);
            IFF_Long *actualArrayLengthPtr;
            structure->getArrayFieldPointer(object, i, &actualArrayLengthPtr);

            if(arrayLength != *actualArrayLengthPtr)
            {
                printCheckMessage(attributePath, field->attributeName, chunk->chunkId, data, "is truncated. It should contain: %d elements, but it has: %d elements", arrayLength, *actualArrayLengthPtr);
                qualityLevel = IFF_degradeQualityLevel(qualityLevel, IFF_QUALITY_TRUNCATED);
            }
        }
    }

    return qualityLevel;
}

IFF_Bool IFF_compareStructure(const IFF_Structure *structure, void *object1, void *object2)
{
    unsigned int i;

    for(i = 0; i < structure->fieldsLength; i++)
    {
        IFF_Field *field = &structure->fields[i];

        if(field->cardinality == IFF_CARDINALITY_SINGLE)
        {
            void *value1 = structure->getFieldPointer(object1, i);
            void *value2 = structure->getFieldPointer(object2, i);

            if(!field->type->compareField(value1, value2))
                return FALSE;
        }
        else if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            IFF_Long *array1LengthPtr, *array2LengthPtr;
            void **array1Ptr = structure->getArrayFieldPointer(object1, i, &array1LengthPtr);
            void **array2Ptr = structure->getArrayFieldPointer(object2, i, &array2LengthPtr);

            if(!IFF_compareArrayField(field, *array1Ptr, *array1LengthPtr, *array2Ptr, *array2LengthPtr))
                return FALSE;
        }
    }

    return TRUE;
}

void IFF_printStructureFields(FILE *file, const unsigned int indentLevel, const IFF_Structure *structure, void *object)
{
    unsigned int i;

    for(i = 0; i < structure->fieldsLength; i++)
    {
        IFF_Field *field = &structure->fields[i];

        if(i > 0)
            fputs(",\n", file);

        if(field->cardinality == IFF_CARDINALITY_SINGLE)
        {
            void *value = structure->getFieldPointer(object, i);

            IFF_printField(file, indentLevel, field, value);
        }
        else if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            IFF_Long *arrayLengthPtr;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &arrayLengthPtr);

            IFF_printArrayField(file, field, indentLevel, *arrayPtr, *arrayLengthPtr, 10);
        }
    }
}

void IFF_printStructureContents(FILE *file, const unsigned int indentLevel, const IFF_Structure *structure, void *object)
{
    fputs(",\n", file);
    IFF_printStructureFields(file, indentLevel, structure, object);
}

void IFF_printStructure(FILE *file, const unsigned int indentLevel, const IFF_Structure *structure, void *object)
{
    fputs("{\n", file);
    IFF_printStructureFields(file, indentLevel + 1, structure, object);
    fputc('\n', file);
    IFF_printIndent(file, indentLevel, "}");
}
