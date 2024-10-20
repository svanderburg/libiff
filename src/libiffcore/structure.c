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

IFF_Bool IFF_readStructure(FILE *file, const IFF_Structure *structure, void *object, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
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
                return IFF_deriveSuccess(status);
        }
        else
        {
            IFF_FieldStatus status;
            unsigned int arrayLength;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &arrayLength);

            *arrayPtr = (void**)malloc(arrayLength * field->type->elementSize);

            if((status = field->type->readArrayField(file, field, *arrayPtr, arrayLength, chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
                return IFF_deriveSuccess(status);
        }
    }

    return TRUE;
}

IFF_Bool IFF_writeStructure(FILE *file, const IFF_Structure *structure, void *object, const IFF_Chunk *chunk, IFF_AttributePath *attributePath, IFF_Long *bytesProcessed, IFF_IOError **error)
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
                return IFF_deriveSuccess(status);
        }
        else if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            IFF_FieldStatus status;
            unsigned int arrayLength;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &arrayLength);

            if((status = field->type->writeArrayField(file, field, *arrayPtr, arrayLength, chunk, attributePath, bytesProcessed, error)) != IFF_FIELD_MORE)
                return IFF_deriveSuccess(status);
        }
    }

    return TRUE;
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
            unsigned int arrayLength;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &arrayLength);

            field->type->clearArrayField(*arrayPtr, arrayLength);
        }
    }
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
            unsigned int array1Length, array2Length;
            void **array1Ptr = structure->getArrayFieldPointer(object1, i, &array1Length);
            void **array2Ptr = structure->getArrayFieldPointer(object2, i, &array2Length);

            if(!field->type->compareArrayField(*array1Ptr, array1Length, *array2Ptr, array2Length))
                return FALSE;
        }
    }

    return TRUE;
}

void IFF_printStructureContents(FILE *file, const unsigned int indentLevel, const IFF_Structure *structure, void *object)
{
    unsigned int i;

    for(i = 0; i < structure->fieldsLength; i++)
    {
        IFF_Field *field = &structure->fields[i];

        if(field->cardinality == IFF_CARDINALITY_SINGLE)
        {
            void *value = structure->getFieldPointer(object, i);

            field->type->printField(file, indentLevel, field, value);
        }
        else if(field->cardinality == IFF_CARDINALITY_MULTIPLE)
        {
            unsigned int arrayLength;
            void **arrayPtr = structure->getArrayFieldPointer(object, i, &arrayLength);

            field->type->printArrayField(file, indentLevel, field, *arrayPtr, arrayLength, 10);
        }
    }
}
