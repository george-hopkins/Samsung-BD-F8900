/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_xml_lib.h"

#ifdef CSR_BT_INSTALL_OBEX_UTIL_XML

void CsrBtFreeXmlTree(CsrBtElementList * xmlTree)
{
    CsrBtElementList * tmpPtr;

    while (xmlTree != NULL)
    {
        tmpPtr = xmlTree->next;
        CsrPmemFree(xmlTree);
        xmlTree = tmpPtr;
    }
}

static CsrBtElementList * allocateElementList(void)
{
    CsrBtElementList * newElementList;
    int index;

    newElementList = (CsrBtElementList *) CsrPmemAlloc(sizeof(CsrBtElementList));
    newElementList->currentIndex = 0;
    for (index = 0; index < CSR_BT_ELEMENTS_PER_BUFFER; index++)
    {
        newElementList->elementArray[index].rootElement.type = CSR_BT_INVALID_TYPE;
    }
    newElementList->next = NULL;

    return newElementList;
}

static CsrUint8 * nextElementStart(CsrUint8 * data, CsrUint8 * dataMax)
{
    while ((*data) && (*data != '<'))
    {
        if (data + 1 < dataMax)
        {
            data++;
        }
        else
        {
            return NULL;
        }
    }
    return data;
}

static CsrUint8 * skipToEndOfElement(CsrUint8 * data, CsrUint8 * dataMax)
{
    CsrUint16 gtMatches = 1;

    do
    {
        if (*data != '>')
        {
            if (data+1 < dataMax)
            {
                if (*data++ == '<')
                {
                    gtMatches++;
                }
            }
        }
        else
        {
            gtMatches--;
            if (gtMatches)
            {
                if (data+1 < dataMax)
                {
                    data++;
                }
                else
                {
                    return NULL;
                }
            }
        }
    } while (gtMatches);
    return data;
}

static CsrUint8 * skipWhiteSpaces(CsrUint8 * string, CsrUint8 * stringMax)
{
    while (CsrIsSpace(*string))
    {
        if (string+1 < stringMax)
        {
            string++;
        }
        else
        {
            return NULL;
        }
    }
    return string;
}

static CsrUint8 * nextWhiteSpaceOrElementEnd(CsrUint8 * string, CsrUint8 * stringMax)
{
    while ((!CsrIsSpace(*string)) && (*string != '>') && (*string != '/'))
    {
        if (string+1 < stringMax)
        {
            string++;
        }
        else
        {
            return NULL;
        }
    }
    return string;
}

static CsrUint8 * nextEqualSign(CsrUint8 * string, CsrUint8 * stringMax)
{
    while (*string != '=')
    {
        if (string+1 < stringMax)
        {
            string++;
        }
        else
        {
            return NULL;
        }
    }
    return string;
}

static CsrUint8 * nextQuote(CsrUint8 quoteCharacter, CsrUint8 * string, CsrUint8 * stringMax)
{
    while (*string != quoteCharacter)
    {
        if (string+1 < stringMax)
        {
            string++;
        }
        else
        {
            return NULL;
        }
    }
    return string;
}

static CsrUint8 * skipWhiteSpacesBackwards(CsrUint8 * string)
{
    while (CsrIsSpace(*string))
    {
        string--;
    }
    return string;
}

static CsrUint8 * extractAttributes(CsrUint8 * xmlDocument, CsrUint8 * xmlDocMax, CsrBtElementAttributeType ** attributePtr, CsrBtElementList ** currentElementList)
{
    CsrBtElementAttributeType * currentAttribute = NULL;
    CsrUint8 * endOfNamePointer;

    *attributePtr = NULL;
    do
    {
        xmlDocument = skipWhiteSpaces(xmlDocument, xmlDocMax);
        if (xmlDocument == NULL)
        {
            return NULL;
        }
        if ((*xmlDocument != '/') && (*xmlDocument != '>'))
        {
            if (*xmlDocument != '<')
            {
                if ((*currentElementList)->currentIndex >= CSR_BT_ELEMENTS_PER_BUFFER)
                {
                    (*currentElementList)->next = allocateElementList();
                    *currentElementList = (*currentElementList)->next;
                }

                if ((*currentElementList)->currentIndex < CSR_BT_ELEMENTS_PER_BUFFER)
                {
                    if (*attributePtr == NULL)
                    {
                        currentAttribute = &(*currentElementList)->elementArray[(*currentElementList)->currentIndex++].attribute;
                        *attributePtr = currentAttribute;
                    }
                    else
                    {
                        currentAttribute->nextAttribute = &(*currentElementList)->elementArray[(*currentElementList)->currentIndex++].attribute;
                        currentAttribute = currentAttribute->nextAttribute;
                    }

                    currentAttribute->type = CSR_BT_ELEMENT_ATTRIBUTE_TYPE;
                    currentAttribute->nextAttribute = NULL;
                    currentAttribute->name = xmlDocument;
                }

                xmlDocument = nextEqualSign(xmlDocument, xmlDocMax);

                if (xmlDocument == NULL)
                {
                    return NULL;
                }
                endOfNamePointer = xmlDocument++ - 1;
                endOfNamePointer = skipWhiteSpacesBackwards(endOfNamePointer);
                *(++endOfNamePointer) = '\0';
                xmlDocument = skipWhiteSpaces(xmlDocument, xmlDocMax);

                if (xmlDocument == NULL)
                {
                    return NULL;
                }

                if (currentAttribute)
                {
                    currentAttribute->value = &xmlDocument[1];
                }

                if (xmlDocument+1 < xmlDocMax)
                {
                    xmlDocument = nextQuote(*xmlDocument, xmlDocument + 1, xmlDocMax);
                }
                else
                {
                    return NULL;
                }

                if ((xmlDocument != NULL) && (xmlDocument+1 < xmlDocMax))
                {
                    *xmlDocument++ = '\0';
                }
                else
                {
                    return NULL;
                }
            }
            else /* comment? */
            {
                if (xmlDocument+1 < xmlDocMax)
                {
                    xmlDocument = skipToEndOfElement(xmlDocument + 1, xmlDocMax);
                }
                else
                {
                    return NULL;
                }
                if ((xmlDocument != NULL) && (xmlDocument+1 < xmlDocMax))
                {
                    xmlDocument++;
                }
                else
                {
                    return NULL;
                }
            }
        }
    } while ((*xmlDocument != '/') && (*xmlDocument != '>'));

    return xmlDocument;
}

/* on entry of extractChildElement, xmlDocument points to first character after the element begin ('<') */
static CsrUint8 * extractChildElement(CsrUint8 * xmlDocument, CsrUint8 * xmlDocMax, CsrBtChildElementType ** firstChildElement, CsrBtElementList ** currentElementList)
{
    CsrBtChildElementType * currentChildElement;

    if ((*currentElementList)->currentIndex >= CSR_BT_ELEMENTS_PER_BUFFER)
    {
        (*currentElementList)->next = allocateElementList();
        *currentElementList = (*currentElementList)->next;
    }

    if (*firstChildElement == NULL)
    {
        *firstChildElement = &(*currentElementList)->elementArray[(*currentElementList)->currentIndex++].childElement;
        currentChildElement = *firstChildElement;
    }
    else
    {
        for (currentChildElement = *firstChildElement; currentChildElement->nextSiblingElement != NULL; currentChildElement = currentChildElement->nextSiblingElement)
        {
            ;
        }
        currentChildElement->nextSiblingElement = &(*currentElementList)->elementArray[(*currentElementList)->currentIndex++].childElement;
        currentChildElement = currentChildElement->nextSiblingElement;
    }
    xmlDocument = skipWhiteSpaces(xmlDocument, xmlDocMax);
    if (xmlDocument == NULL)
    {
        return NULL;
    }
    currentChildElement->type = CSR_BT_CHILD_ELEMENT_TYPE;
    currentChildElement->name = xmlDocument;
    currentChildElement->value = NULL;
    currentChildElement->firstChildElement = NULL;
    currentChildElement->nextSiblingElement = NULL;
    currentChildElement->firstAttribute = NULL;
    xmlDocument = nextWhiteSpaceOrElementEnd(xmlDocument, xmlDocMax);
    if (xmlDocument == NULL)
    {
        return NULL;
    }
    if (*xmlDocument == '/')
    {
        if (xmlDocument+1 < xmlDocMax)
        {
            *xmlDocument++ = '\0';
        }
        else
        {
            return NULL;
        }
        xmlDocument = skipToEndOfElement(xmlDocument, xmlDocMax);
        if (xmlDocument == NULL)
        {
            return NULL;
        }
    }
    else
    {
        if (*xmlDocument != '>')
        {
            if (xmlDocument+1 < xmlDocMax)
            {
                *xmlDocument++ = '\0';
            }
            else
            {
                return NULL;
            }
            xmlDocument = extractAttributes(xmlDocument, xmlDocMax, &currentChildElement->firstAttribute, currentElementList);
            if (xmlDocument == NULL)
            {
                return NULL;
            }
        }
        if (*xmlDocument == '>')
        {
            if (xmlDocument+1 < xmlDocMax)
            {
                *xmlDocument++ = '\0';
            }
            else
            {
                return NULL;
            }
            xmlDocument = skipWhiteSpaces(xmlDocument, xmlDocMax);
            if (xmlDocument == NULL)
            {
                return NULL;
            }
            while (1)
            {
                if (((xmlDocument[0] == '<') || (xmlDocument[0] == '\0')) && (xmlDocument[1] != '/'))
                {
                    if ((*xmlDocument != '?') && (*xmlDocument != '!') && (CsrStrNICmp((char *)xmlDocument, "XML", 3) != 0)) /* skip xml declarations, schemas, comments, DTD and reserved xml tags */
                    {
                        xmlDocument = extractChildElement(xmlDocument + 1, xmlDocMax, &currentChildElement->firstChildElement, currentElementList);
                        if (xmlDocument == NULL)
                        {
                            return NULL;
                        }
                    }
                    else
                    {
                        if (xmlDocument+1 < xmlDocMax)
                        {
                            xmlDocument = skipToEndOfElement(xmlDocument + 1, xmlDocMax);
                        }
                        else
                        {
                            return NULL;
                        }
                        if ((xmlDocument != NULL) && (xmlDocument+1 < xmlDocMax))
                        {
                            xmlDocument = skipWhiteSpaces(xmlDocument + 1, xmlDocMax);
                        }
                        else
                        {
                            return NULL;
                        }
                        if (xmlDocument == NULL)
                        {
                            return NULL;
                        }
                    }
                }
                else if (((xmlDocument[0] == '<') || (xmlDocument[0] == '\0')) && (xmlDocument[1] == '/'))
                {
                    if (xmlDocument+2 < xmlDocMax)
                    {
                        xmlDocument = skipToEndOfElement(xmlDocument + 2, xmlDocMax);
                    }
                    else
                    {
                        return NULL;
                    }
                    if ((xmlDocument != NULL) && (xmlDocument+1 < xmlDocMax))
                    {
                        xmlDocument = skipWhiteSpaces(xmlDocument + 1, xmlDocMax);
                    }
                    else
                    {
                        return NULL;
                    }
                    if (xmlDocument == NULL)
                    {
                        return NULL;
                    }
                    break;
                }
                else
                {
                    CsrUint8 * valueZeroTerminator;

                    currentChildElement->value = xmlDocument;
                    xmlDocument = nextElementStart(xmlDocument, xmlDocMax);
                    if (xmlDocument == NULL)
                    {
                        return NULL;
                    }
                    valueZeroTerminator = skipWhiteSpacesBackwards(xmlDocument - 1);
                    valueZeroTerminator[1] = '\0';
                }
            }
        }
        else /* must be '/' in which case the element is empty, so exit the construction site */
        {
            *xmlDocument = '\0';
            if (xmlDocument+1 < xmlDocMax)
            {
                xmlDocument = skipToEndOfElement(xmlDocument + 1, xmlDocMax);
            }
            else
            {
                return NULL;
            }
            if ((xmlDocument != NULL) && (xmlDocument+1 < xmlDocMax))
            {
                xmlDocument = skipWhiteSpaces(xmlDocument + 1, xmlDocMax);
            }
            else
            {
                return NULL;
            }
            if (xmlDocument == NULL)
            {
                return NULL;
            }
        }
    }
    return xmlDocument;
}

CsrBtElementList * CsrBtBuildXmlTree(CsrUint8 * xmlDocument, CsrUint32 docLength) /* The CsrBtBuildXmlTree function does only work with valid well-formed xml documents! In addition, although not strictly forbidden, it only works if the "text" parts does not contain '>' */
{
    CsrBtElementList * returnPointer;
    CsrBtElementList * currentElementList;
    CsrBtRootElementType * rootElement;
    CsrUint8 * xmlDocMax = xmlDocument + docLength;

    currentElementList = returnPointer = allocateElementList();
    do
    {
        xmlDocument = nextElementStart(xmlDocument, xmlDocMax);
        if (xmlDocument == NULL)
        {
            if(returnPointer !=NULL)
            {
                CsrBtFreeXmlTree(returnPointer);
            }
            return NULL;
        }
        if (*xmlDocument)
        {
            if (xmlDocument+1 < xmlDocMax)
            {
                xmlDocument++;
            }
            else
            {
                if(returnPointer !=NULL)
                {
                    CsrBtFreeXmlTree(returnPointer);
                }
                return NULL;
            }
            if ((*xmlDocument == '?') || (*xmlDocument == '!') || (CsrStrNICmp((char *) xmlDocument, "XML", 3) == 0)) /* skip xml declarations, schemas, comments, DTD and reserved xml tags */
            {
                if (xmlDocument+1 < xmlDocMax)
                {
                    xmlDocument = skipToEndOfElement(xmlDocument + 1, xmlDocMax);
                    if (xmlDocument == NULL)
                    {
                        if(returnPointer !=NULL)
                        {
                            CsrBtFreeXmlTree(returnPointer);
                        }
                        return NULL;
                    }
                }
                else
                {
                    if(returnPointer !=NULL)
                    {
                        CsrBtFreeXmlTree(returnPointer);
                    }
                    return NULL;
                }
            }
            else
            {
                xmlDocument = skipWhiteSpaces(xmlDocument, xmlDocMax);
                if (xmlDocument == NULL)
                {
                    if(returnPointer !=NULL)
                    {
                        CsrBtFreeXmlTree(returnPointer);
                    }
                    return NULL;
                }
                rootElement = &currentElementList->elementArray[currentElementList->currentIndex++].rootElement;
                rootElement->type = CSR_BT_ROOT_ELEMENT_TYPE;
                rootElement->name = xmlDocument;
                rootElement->value = NULL;
                rootElement->firstChildElement = NULL;
                rootElement->firstAttribute = NULL;
                xmlDocument = nextWhiteSpaceOrElementEnd(xmlDocument, xmlDocMax);
                if (xmlDocument == NULL)
                {
                    if(returnPointer !=NULL)
                    {
                        CsrBtFreeXmlTree(returnPointer);
                    }
                    return NULL;
                }
                if (*xmlDocument == '/')
                {
                    if (xmlDocument+1 < xmlDocMax)
                    {
                        *xmlDocument++ = '\0';
                    }
                    else
                    {
                        if(returnPointer !=NULL)
                        {
                            CsrBtFreeXmlTree(returnPointer);
                        }
                        return NULL;
                    }
                    xmlDocument = skipToEndOfElement(xmlDocument, xmlDocMax);
                    if (xmlDocMax == NULL)
                    {
                        if(returnPointer !=NULL)
                        {
                            CsrBtFreeXmlTree(returnPointer);
                        }
                        return NULL;
                    }
                }
                else
                {
                    if (*xmlDocument != '>')
                    {
                        if (xmlDocument+1 < xmlDocMax)
                        {
                            *xmlDocument++ = '\0';
                        }
                        else
                        {
                            if(returnPointer !=NULL)
                            {
                                CsrBtFreeXmlTree(returnPointer);
                            }
                            return NULL;
                        }
                        xmlDocument = extractAttributes(xmlDocument, xmlDocMax, &rootElement->firstAttribute, &currentElementList);
                        if (xmlDocument == NULL)
                        {
                            if(returnPointer !=NULL)
                            {
                                CsrBtFreeXmlTree(returnPointer);
                            }
                            return NULL;
                        }
                    }
                    if (*xmlDocument == '>')
                    {
                        if (xmlDocument+1 < xmlDocMax)
                        {
                            *xmlDocument++ = '\0';
                        }
                        else
                        {
                            if(returnPointer !=NULL)
                            {
                                CsrBtFreeXmlTree(returnPointer);
                            }
                            return NULL;
                        }
                        xmlDocument = skipWhiteSpaces(xmlDocument, xmlDocMax);
                        if (xmlDocument == NULL)
                        {
                            if(returnPointer !=NULL)
                            {
                                CsrBtFreeXmlTree(returnPointer);
                            }
                            return NULL;
                        }
                        while (1)
                        {
                            if (((xmlDocument[0] == '<') || (xmlDocument[0] == '\0')) && (xmlDocument[1] != '/'))
                            {
                                if ((*xmlDocument != '?') && (*xmlDocument != '!') && (CsrStrNICmp((char *) xmlDocument, "XML", 3) != 0)) /* skip xml declarations, schemas, comments, DTD and reserved xml tags */
                                {
                                    if (xmlDocument+1 < xmlDocMax)
                                    {
                                        xmlDocument = extractChildElement(xmlDocument + 1, xmlDocMax, &rootElement->firstChildElement, &currentElementList);
                                    }
                                    else
                                    {
                                        if(returnPointer !=NULL)
                                        {
                                            CsrBtFreeXmlTree(returnPointer);
                                        }
                                        return NULL;
                                    }
                                    if (xmlDocument == NULL)
                                    {
                                        if(returnPointer !=NULL)
                                        {
                                            CsrBtFreeXmlTree(returnPointer);
                                        }
                                        return NULL;
                                    }
                                }
                                else
                                {
                                    if (xmlDocument+1 < xmlDocMax)
                                    {
                                        xmlDocument = skipToEndOfElement(xmlDocument + 1, xmlDocMax);
                                    }
                                    else
                                    {
                                        if(returnPointer !=NULL)
                                        {
                                            CsrBtFreeXmlTree(returnPointer);
                                        }
                                        return NULL;
                                    }
                                    if ((xmlDocument != NULL) && (xmlDocument+1 < xmlDocMax))
                                    {
                                        xmlDocument = nextElementStart(xmlDocument + 1, xmlDocMax);
                                    }
                                    else
                                    {
                                        if(returnPointer !=NULL)
                                        {
                                            CsrBtFreeXmlTree(returnPointer);
                                        }
                                        return NULL;
                                    }
                                    if (xmlDocument == NULL)
                                    {
                                        if(returnPointer !=NULL)
                                        {
                                            CsrBtFreeXmlTree(returnPointer);
                                        }
                                        return NULL;
                                    }
                                }
                            }
                            else if (((xmlDocument[0] == '<') || (xmlDocument[0] == '\0')) && (xmlDocument[1] == '/'))
                            {
                                break;
                            }
                            else
                            {
                                CsrUint8 * valueZeroTerminator;

                                rootElement->value = xmlDocument;
                                xmlDocument = nextElementStart(xmlDocument, xmlDocMax);
                                if (xmlDocument == NULL)
                                {
                                    if(returnPointer !=NULL)
                                    {
                                        CsrBtFreeXmlTree(returnPointer);
                                    }
                                    return NULL;
                                }
                                valueZeroTerminator = skipWhiteSpacesBackwards(xmlDocument - 1);
                                valueZeroTerminator[1] = '\0';
                            }
                        }
                    }
                    else /* must be '/' in which case the root element is empty, so exit the construction site */
                    {
                        if (xmlDocument+1 < xmlDocMax)
                        {
                            *xmlDocument++ = '\0';
                        }
                        else
                        {
                            if(returnPointer !=NULL)
                            {
                                CsrBtFreeXmlTree(returnPointer);
                            }
                            return NULL;
                        }
                    }
                }
            }
        }
    } while ((xmlDocument) && (*xmlDocument) && (returnPointer->elementArray[0].rootElement.type == CSR_BT_INVALID_TYPE));

    if (returnPointer->elementArray[0].rootElement.type == CSR_BT_INVALID_TYPE)
    {
        CsrBtFreeXmlTree(returnPointer);
        returnPointer = NULL;
    }

    return returnPointer;
}

#endif
