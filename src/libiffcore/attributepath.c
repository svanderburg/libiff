#include "attributepath.h"
#include <stdlib.h>
#include <string.h>

static IFF_AttributePathNode *createAttributePathNodeByIndex(unsigned int index)
{
    IFF_AttributePathNode *node = (IFF_AttributePathNode*)malloc(sizeof(IFF_AttributePathNode));

    if(node != NULL)
    {
        node->next = NULL;
        node->type = IFF_ATTRIBUTE_INDEX;
        node->attribute.index = index;
    }

    return node;
}

static IFF_AttributePathNode *createAttributePathNodeByName(char *name)
{
    IFF_AttributePathNode *node = (IFF_AttributePathNode*)malloc(sizeof(IFF_AttributePathNode));

    if(node != NULL)
    {
        node->next = NULL;
        node->type = IFF_ATTRIBUTE_NAME;
        node->attribute.name = name;
    }

    return node;
}

static void linkAttributePathNodes(IFF_AttributePathNode *attributePathNode1, IFF_AttributePathNode *attributePathNode2)
{
    attributePathNode1->next = attributePathNode2;
    attributePathNode2->previous = attributePathNode1;
}

static void freeAttributePathNode(IFF_AttributePathNode *attributePathNode)
{
    free(attributePathNode);
}

static void printAttributePathNode(FILE *file, const IFF_AttributePathNode *attributePathNode)
{
    switch(attributePathNode->type)
    {
        case IFF_ATTRIBUTE_INDEX:
            fprintf(file, "[%d]", attributePathNode->attribute.index);
            break;
        case IFF_ATTRIBUTE_NAME:
            fprintf(file, ".%s", attributePathNode->attribute.name);
            break;
    }
}

IFF_AttributePath *IFF_createAttributePath(void)
{
    return (IFF_AttributePath*)calloc(1, sizeof(IFF_AttributePath));
}

void IFF_freeAttributePath(IFF_AttributePath *attributePath)
{
    IFF_AttributePathNode *node = attributePath->rootNode;

    while(node != NULL)
    {
        IFF_AttributePathNode *nextNode = node->next;
        freeAttributePathNode(node);
        node = nextNode;
    }

    free(attributePath);
}

static void linkAttributePathNodeToAttributePath(IFF_AttributePath *attributePath, IFF_AttributePathNode *node)
{
    if(attributePath->rootNode == NULL)
    {
        node->previous = NULL;
        attributePath->rootNode = node;
    }

    if(attributePath->currentNode != NULL)
        linkAttributePathNodes(attributePath->currentNode, node);

    attributePath->currentNode = node;
}


void IFF_visitAttributeByIndex(IFF_AttributePath *attributePath, unsigned int index)
{
    IFF_AttributePathNode *node = createAttributePathNodeByIndex(index);
    linkAttributePathNodeToAttributePath(attributePath, node);
}

void IFF_visitAttributeByName(IFF_AttributePath *attributePath, char *name)
{
    IFF_AttributePathNode *node = createAttributePathNodeByName(name);
    linkAttributePathNodeToAttributePath(attributePath, node);
}

void IFF_unvisitAttribute(IFF_AttributePath *attributePath)
{
    IFF_AttributePathNode *currentNode = attributePath->currentNode;
    IFF_AttributePathNode *previous = currentNode->previous;

    attributePath->currentNode = previous;

    if(previous == NULL)
        attributePath->rootNode = NULL;
    else
        previous->next = NULL;

    freeAttributePathNode(currentNode);
}

void IFF_printAttributePath(FILE *file, const IFF_AttributePath *attributePath)
{
    IFF_AttributePathNode *node = attributePath->rootNode;

    while(node != NULL)
    {
        printAttributePathNode(file, node);
        node = node->next;
    }
}
