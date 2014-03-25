/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_util.h"

static CsrUint32 localArgc = 0;
static CsrCharString **localArgv = NULL;

void CsrArgSearchInit(CsrUint32 argc, CsrCharString *argv[])
{
    localArgc = argc;
    localArgv = argv;
}

CsrBool CsrArgSearchValidate(void)
{
    CsrBool param = FALSE;
    CsrUint32 argi;
    for (argi = 1; argi < localArgc; ++argi)
    {
        if (CsrStrNCmp("--", localArgv[argi], 2) == 0)
        {
            param = TRUE;
        }
        else
        {
            if (param)
            {
                param = FALSE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

CsrCharString *CsrArgSearch(CsrUint32 *argi, const CsrCharString *prefix, CsrCharString **parameter, CsrCharString **value)
{
    CsrBool exact = FALSE;
    CsrUint32 index = 1;
    CsrSize prefixLen = CsrStrLen(prefix);

    if (argi == NULL)
    {
        exact = TRUE;
        argi = &index;
    }

    for ( ; *argi < localArgc; ++*argi)
    {
        if ((exact && (CsrStrCmp(prefix, localArgv[*argi]) == 0)) ||
            (!exact && (CsrStrNCmp(prefix, localArgv[*argi], prefixLen) == 0)))
        {
            if (parameter != NULL)
            {
                *parameter = localArgv[*argi];
            }
            ++*argi;
            if ((*argi < localArgc) && (CsrStrNCmp("--", localArgv[*argi], 2) != 0))
            {
                if (value != NULL)
                {
                    *value = localArgv[*argi];
                }
                ++*argi;
                return localArgv[*argi - 1];
            }
            if (value != NULL)
            {
                *value = NULL;
            }
            return localArgv[*argi - 1];
        }
    }
    return NULL;
}
