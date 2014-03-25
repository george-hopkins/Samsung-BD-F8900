/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_app.h"
#include "csr_bt_platform.h"

#ifndef _WIN32

void CsrBtAppCmdlineParse(int argc, char *argv[], char *extraOptions)
{
    /*Specify all options, e.g. "a:" for option 'a' with required option argument
     * "a" for option 'a' without option argument
     * "a::" for option 'a' with optional argument
     * we use both upper and lower case as one option for now,
     * so both needs to be specified
     */
    CsrBool bdAddrSet = FALSE;
    char defaultOptions[] =
#ifdef CSR_LOG_ENABLE
        "l:e:wv"
#endif
        "f:u:x:";

    CsrInt32 ch;
	CsrUint16 idx=0;
	
    while((ch = defaultOptions[idx])  != '\0' )
    {
        switch(ch)
        {

            case 'F':
            case 'f':
                {
                    CsrBtBootstrapSetFrequency(NULL, romCrystalFreq);
                    break;
                }
#ifdef CSR_LOG_ENABLE
#ifdef CSR_LOGTRANSPORT_HAVE_WSPIPE
            case 'W':
            case 'w':
                {
                    CsrBtAppLogWireshark();
                    break;
                }
#endif
#ifdef CSR_LOGTRANSPORT_HAVE_FTSPIPE
            case 'V':
            case 'v':
                {
                    CsrBtAppLogFrontline();
                    break;
                }
#endif
#endif /* CSR_LOG_ENABLE */
            case 'U':
            case 'u':
                {
                    CsrBtBootstrapSetBitRate(NULL, romBaud);

                    break;
                }
            case 'X':
            case 'x':
                {

                    CsrBtBootstrapSetLocalBtAddress(NULL, &romBdAddress);
                    bdAddrSet = TRUE;
                    break;
                }
            default:
                {
                    break;
                }
        }
		idx++;
    }

    /* Warn if address isn't set */
    if(!bdAddrSet)
    {
        CL_API_LOGPrintf("\n\n  Warning: The local Bluetooth address has not been specified with -X option.\n");
        CL_API_LOGPrintf("  The default Bluetooth address will be used.\n\n");

#ifndef __arm
        CsrBtSleep(5);
#endif
    }
}
#endif
