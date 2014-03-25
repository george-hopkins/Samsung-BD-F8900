/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"

/* Determine the number of characters in a string */
CsrUint32 CsrUiUtilStringLength(CsrUint16 *string)
{
    CsrUint32 stringlen = 0;

    if (string != NULL)
    {
        while (*string++)
        {
            stringlen++;
        }
    }

    return stringlen;
}

/* Determine the number of lines in a string */
CsrUint16 CsrUiUtilStringLines(CsrUint16 *string)
{
    CsrUint16 c = 0;

    if ((string != NULL) && (*string != 0))
    {
        for (c = 1; *string != 0; string++)
        {
            if (*string == 0x0A)
            {
                c++;
            }
        }
    }

    return c;
}

/* Copy a string (allocates memory for new string) */
CsrUint16 *CsrUiUtilStringCopy(CsrUint16 *string)
{
    CsrUint32 stringlen;
    CsrUint16 *res;
    CsrUint16 *pstr;

    if (!string)
    {
        return NULL;
    }

    stringlen = CsrUiUtilStringLength(string);

    pstr = res = CsrPmemAlloc((stringlen + 1) * 2);

    while (*string != 0)
    {
        *pstr++ = *string++;
    }
    *pstr = 0;

    return res;
}

/* Count number of characters before next word boundary */
CsrUint16 CsrUiUtilWordLength(CsrUint16 *string)
{
    CsrUint16 c;

    /* Count until SPACE, TERMINATOR or LINE FEED */
    for (c = 0; (*string != 0x20) && (*string != 0) && (*string != 0x0A); string++)
    {
        c++;
    }

    return c;
}

/* Perform wordwrap on the supplied string, returning a new allocated string */
CsrUint16 *CsrUiUtilWordWrap(CsrUint16 columns, CsrUint16 rows, CsrUint16 *string)
{
    CsrUint32 stringlen;
    CsrUint16 *newstring;
    CsrUint16 *pstr1;
    CsrUint16 *pstr2;
    CsrUint16 c;

    /* Determine input string length */
    stringlen = CsrUiUtilStringLength(string);

    /* Allocate a new string to insert new line characters in */
    pstr2 = newstring = CsrPmemZalloc((stringlen + 1) * 2);

    /* Copy string contents to new string */
    pstr1 = string;

    while (*pstr1 != 0)
    {
        *pstr2++ = *pstr1++;
    }

    pstr1 = newstring;

    while (TRUE)
    {
        pstr2 = NULL;

        /* Search until max characters reached
           Stop at end of string
           Stop at end of column, unless it is a new line character
           Reset column counter when new line character is encountered
           Save position when space encountered */
        for (c = 0; (c < columns || *pstr1 == 0x0A) && (*pstr1 != 0); pstr1++)
        {
            c++;
            if (*pstr1 == 0x20) /* Space */
            {
                /* Found candidate for line break */
                pstr2 = pstr1;
            }
            if (*pstr1 == 0x0A) /* New Line */
            {
                /* Forced line shift */
                c = 0;
                pstr2 = NULL;
            }
        }

        if (*pstr1 == 0)
        {
            /* Reached end of string */
            break;
        }

        if (*pstr1 == 0x20) /* Space */
        {
            /* Search stopped conveniently at a space character - replace with new line */
            *pstr1 = 0x0A;
            pstr1++;
            continue;
        }

        /* Not space for the word - insert new line at last space if found, unless the next word is too long to fit the column anyway */
        if ((pstr2 != NULL) && !(CsrUiUtilWordLength(pstr2 + 1) >= columns))
        {
            *pstr2 = 0x0A;
            pstr1 = pstr2 + 1;
            continue;
        }
        else
        {
            /* Very long word, reallocate string to make room for new line character */
            CsrUint16 offset;

            /* Determine relative position within string */
            offset = (CsrUint16) (pstr1 - newstring);

            /* Reallocate string */
            string = newstring;
            stringlen++;
            pstr2 = newstring = CsrPmemZalloc((stringlen + 1) * 2);

            /* Copy first part of string to new string */
            for (pstr1 = string; ((CsrUint16) (pstr1 - string)) < offset; )
            {
                *pstr2++ = *pstr1++;
            }

            /* Insert new line character and copy the rest of the string */
            *pstr2++ = 0x0A;
            while (*pstr1 != 0)
            {
                *pstr2++ = *pstr1++;
            }

            pstr1 = (CsrUint16 *) (newstring + offset);
            pstr2 = NULL;

            /* Deallocate the old string */
            CsrPmemFree(string);
        }
    }

    /* Limit string to maximum rows specified */
    c = 0;
    for (pstr1 = newstring; (*pstr1 != 0) && (c < rows); pstr1++)
    {
        if (*pstr1 == 0x0A)
        {
            if (++c == rows)
            {
                *pstr1 = 0;
                break;
            }
        }
    }

    return newstring;
}

CsrUint16 *CsrUiUtilStringCat(CsrUint16 *dest, const CsrUint16 *src)
{
    while (*dest)
    {
        dest++;
    }
    for (*dest = *src; *src; *dest = *src)
    {
        dest++;
        src++;
    }
    return dest;
}

CsrUint16 *CsrUiUtilStringCatCap(CsrUint16 *dest, const CsrUint16 *src, CsrUint32 cap)
{
    while (*dest)
    {
        dest++;
    }
    for (*dest = *src; *src; *dest = *src)
    {
        if (!(cap--))
        {
            *(dest) = 0;
            break;
        }
        dest++;
        src++;
    }
    return dest;
}
