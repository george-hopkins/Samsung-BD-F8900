/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_app.h"
#include "csr_bt_platform.h"

static int argvIndex = 1;
static char *optionArgument = NULL;

/* List available serial ports on Windows */
#ifdef _WIN32
#include <windows.h>
static void csrBtAppListAvailableSerialPorts(void)
{
    HKEY handle;
    printf("\nList of available serial-ports:\n");

    /* Open registry key */
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                    "HARDWARE\\DEVICEMAP\\SERIALCOMM",
                    0, KEY_READ, &handle)
       == ERROR_SUCCESS)
    {
        /* Determine maximum buffer sizes by key query */
        CsrUint32 maxNameLength;
        CsrUint32 maxDataLength;
        if (RegQueryInfoKey(handle, 0, 0, 0, 0, 0, 0, 0,
                            &maxNameLength,
                            &maxDataLength, 0, 0)
            == ERROR_SUCCESS)
        {
            /* Extract sub-keys and their value */
            CsrUint16 index = 0;
            CsrUint8 * nameBuffer;
            CsrUint16 * dataBuffer;
            CsrUint8 run = TRUE;
            CsrUint32 result;
            CsrUint32 type;
            CsrUint32 nameBufferLength = maxNameLength;
            CsrUint32 dataBufferLength = maxDataLength;

            /*
               Change these increments if you get the
               'Buffers too smaal, needed X' error
             */
            maxNameLength = maxNameLength+1;
            nameBufferLength = maxNameLength;
            maxDataLength = maxDataLength+0;
            dataBufferLength = maxDataLength;


            nameBuffer = CsrPmemAlloc(sizeof(CsrUint8)*maxNameLength);
            dataBuffer = CsrPmemAlloc(sizeof(CsrUint16)*maxDataLength);

            while(run){
                result = RegEnumValue(handle, index++,
                                      nameBuffer,
                                      &nameBufferLength,
                                      0, &type,
                                      (BYTE *) dataBuffer,
                                      &dataBufferLength);
                if (result  == ERROR_SUCCESS ){

                    /* Write to screen */
                    if(type == REG_SZ){
                        printf("\tPort: %s\tLocation: %s \n", (CsrUint8 *) dataBuffer, nameBuffer);
                        /* Restore the buffer lengths */
                        nameBufferLength = maxNameLength;
                        dataBufferLength = maxDataLength;
                    }
                } else if (result == ERROR_MORE_DATA){
                    printf("\tBuffers too small, needed: %d\n", dataBufferLength);
                } else if (result == ERROR_NO_MORE_ITEMS){
                    /* printf("\t(No more data)\n"); */
                    run = FALSE;

                } else {
                    /* printf("\tresult code: %d\n", result); */
                    run = FALSE;
                }
            }
            CsrPmemFree(nameBuffer);
            CsrPmemFree(dataBuffer);
        }
    }

    /* Close key */
    RegCloseKey(handle);
}
#endif

/* Convert BD_ADDR string to structure */
static CsrBool csrBtAppAddrFromString(char *strAddr, BD_ADDR_T *bdAddr)
{
    int uap,lap,nap;

    if(sscanf(strAddr, "%4x:%2x:%6x" , &nap, &uap, &lap) != 3)
    {
        return FALSE;
    }
    bdAddr->uap = (CsrUint8) (uap & 0x00FF);
    bdAddr->lap = lap & 0x00FFFFFF;
    bdAddr->nap = nap & 0xFFFF;
    return TRUE;
}


/* Print command line usage */
static void csrBtAppCmdlineUsage(char *programName, char *error)
{
    char *col;
    char *des;
    char *opt;

    /* Alloc and initialise collection buffer, decription buffer and option */
    col = CsrPmemZalloc(2048);
    des = CsrPmemZalloc(2048);
    opt = CsrPmemZalloc(128);

    /* Help */
    strcat(col, "[-H ]");
    sprintf(opt, "\t-H\t\t\tDisplay help/usage\n");
    strcat(des, opt);

    /* Transport */
    strcat(col, "[-T <transport>]");
    sprintf(opt, "\t-T <transport>\t\tTransport to use, either BCSP or USB, default: BCSP\n");
    strcat(des, opt);

    /* Serial port */
    if (transportType == TRANSPORT_TYPE_BCSP ||
        transportType == TRANSPORT_TYPE_H4_UART ||
        transportType == TRANSPORT_TYPE_H5_THREE_WIRE ||
        transportType == TRANSPORT_TYPE_H4_I ||
        transportType == TRANSPORT_TYPE_H4_DS)
    {
        strcat(col, "[-C <dev>] ");
        sprintf(opt, "\t-C <dev>\t\tSerial port for the Casira, default: '%s'\n",
                casira_ComPort);
        strcat(des, opt);

        strcat(col, "[-B <baud>] ");
        sprintf(opt, "\t-B <baud>\t\tBaud rate for the Casira serial port, default: '%s'\n",
                baudRate);
        strcat(des, opt);

#ifdef _WIN32
        strcat(col, "[-P] ");
        sprintf(opt, "\t-P\t\t\tList available serial (COM) ports\n");
        strcat(des, opt);
#endif
    }
    else if (transportType == TRANSPORT_TYPE_USB)
    {
        strcat(col, "[-C <dev>] ");
        sprintf(opt, "\t-C <dev>\t\tUSB port for the Nanosira, default: '%s'\n",
                "\\\\\\\\.\\\\csr0");
        strcat(des, opt);
    }

    /* Peer address */
    strcat(col, "[-A <addr>] ");
    sprintf(opt, "\t-A <addr>\t\tRemote Bluetooth target address, default: '%04x:%02x:%06x'\n",
            defGlobalBdAddr.nap, defGlobalBdAddr.uap, defGlobalBdAddr.lap);
    strcat(des, opt);

    /* Bootstrap PSR file support */
    strcat(col, "[-S <psrfile>] ");
    sprintf(opt, "\t-S <psrfile>\t\tPSR bootstrap/patch bundle file\n");
    strcat(des, opt);

    /* Synergy BT logging */
#ifdef CSR_LOG_ENABLE
    strcat(col, "[-L <level>] ");
    sprintf(opt, "\t-L <level>\t\tSynergy task log level in hex, default: '0x%x'\n", logTaskLevel);
    strcat(des, opt);

    strcat(col, "[-E <level>] ");
    sprintf(opt, "\t-E <level>\t\tSynergy environment log level in hex, default: '0x%x'\n", logEnvLevel);
    strcat(des, opt);

#ifdef CSR_LOGTRANSPORT_HAVE_WSPIPE
    strcat(col, "[-W ] ");
    sprintf(opt, "\t-W\t\t\tWireshark live logging\n");
    strcat(des, opt);
#endif
#ifdef CSR_LOGTRANSPORT_HAVE_FTSPIPE
    strcat(col, "[-V ] ");
    sprintf(opt, "\t-V\t\t\tFTS live logging\n");
    strcat(des, opt);
#endif
#endif

    /* BD address for bootstrap */
    strcat(col, "[-X <addr>] ");
    sprintf(opt, "\t-X <addr>\t\tBootstrap Bluetooth address for ROM builds\n");
    strcat(des, opt);

    /* Crystal frequency */
    strcat(col, "[-F <freq>] ");
    sprintf(opt, "\t-F <freq>\t\tCrystal frequency for ROM builds\n");
    strcat(des, opt);

    /* Baud rate */
    strcat(col, "[-U <baud>] ");
    sprintf(opt, "\t-U <baud>\t\tRe-connect baud rate for ROM builds\n");
    strcat(des, opt);

    applicationUsage(col,des,opt);

    /* Print to stderr */
    if(NULL != error ){
        fprintf(stderr, "Command line error: %s\n\nUsage:\n  %s %s\n\nWhere:\n%s",
                error,
                programName,
                col,
                des);
#ifdef _WIN32
        csrBtAppListAvailableSerialPorts();
#endif

    }
    else
    {
        fprintf(stderr, "Usage:\n  %s %s\n\nWhere:\n%s",
                programName,
                col,
                des);
    }

    CsrPmemFree(col);
    CsrPmemFree(des);
    CsrPmemFree(opt);

    exit(0);
}



/* Parse commandline options and return the next valid option char.
 * Upon an error in the options: returns -1 returns '?' when something
 * is specified wrong on the cmdline
 */
static CsrInt32 csrBtAppGetOption(CsrInt32 nargc,
                                  char * const *nargv,
                                  char *defaultOptions,
                                  char *extraOptions)
{
    char *defaultOptionsIndex;
    char *extraOptionsIndex;
    char *optionsIndex;
    int optionChar;

    static char *place = ""; /* option character processing pointer - default empty message */
    const char requiredArgumentChar[] = "Command line error: argument required for option -%c\n\n";
    const char noArgument[] = "Command line error: option -%c does not take an argument\n\n";
    const char unknownOptionChar[] = "Command line error: unknown option: %c\n\n";
    const char missingWhitesp[] = "Command line error: No white space after option: -%c\n\n";

    while(!*place)
    {
        if (argvIndex >= nargc)
        {
            /* no more arguments in argv */
            place = "";
            return -1;
        }

        /* Do we have an option? */
        if (*(place = nargv[argvIndex]) != '-' )
        {
            /* first application argument must be '-' */
            place = "";
            fprintf(stderr, "Command line error: Missing option specifier '-'\n");
            return '?';
        }

        /* we have '-', proceed */
    }

    /* character following '-' */
    place++;
    optionChar = (int)*place;

    /* Check if we have it in default or extra options */
    defaultOptionsIndex = strchr(defaultOptions, optionChar);
    extraOptionsIndex = strchr(extraOptions, optionChar);
    if ((defaultOptionsIndex  == NULL) & ( extraOptionsIndex == NULL) )
    {
        /* specified option not a legal option */
        fprintf(stderr,unknownOptionChar, optionChar);
        return (int)'?';
    }
    if(extraOptionsIndex == NULL)
    {
        /* we have a default option */
        optionsIndex = defaultOptionsIndex;
    }
    else
    {
        /* we have extra option supplied by app.*/
        optionsIndex = extraOptionsIndex;
    }

    /* look for argument following option */
    place++;
    if (*++optionsIndex != ':')
    {
        /* no argument for this option */
        if(!*place)
        {
            /* We are at the end of argv[argvIndex] - move to next */
            ++argvIndex;
            if(argvIndex < nargc && *nargv[argvIndex] != '-')
            {
                /* argument given to non-arg option */
                fprintf(stderr,noArgument, optionChar );
                return (int)'?';
            }
        }
        else if(*place != ' ')
        {
            /*character following option character*/
            fprintf(stderr,noArgument, optionChar );
            return (int)'?';
        }
    }
    else
    {
        /* (optional) argument for this option */
        optionArgument = NULL;
        if (*place)
        {
            /* Error: no white space: -BARG*/
            fprintf(stderr, missingWhitesp,optionChar);
            return (int)'?';
        }
        else if (optionsIndex[1] != ':')
        {
            /* not optional argument */
            if (++argvIndex >= nargc)
            {
                /* no argument provided */
                place = "";
                fprintf(stderr, requiredArgumentChar, optionChar);
                return '?';
            }
            else
            {
                /* argument provided */
                optionArgument = nargv[argvIndex];
            }
        }
        place = "";
        ++argvIndex;
    }
    /* Return the option character */
    place = "";
    return (optionChar);
}

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
#ifdef _WIN32
        "pP"
#endif
#ifdef CSR_LOG_ENABLE
        "l:L:e:E:wWvV"
#endif
        "f:F:u:U:x:X:h:H:a:A:b:B:c:C:t:T:s:S:";

    CsrInt32 ch;

    /* We want both lower and upper case as one option for now
     * Cmdline is sanitized according to rules in defaultOptions/extraOptions
     * by getOptions()
     * When an option argument is specified it is set in optionArgument
     */
    while((ch = csrBtAppGetOption(argc, argv, defaultOptions, extraOptions)) != -1 )
    {
        switch(ch)
        {
            case 'A':
            case 'a':
                if (!csrBtAppAddrFromString(optionArgument, &defGlobalBdAddr))
                {
                    csrBtAppCmdlineUsage(argv[0], "'A'/'a' argument must follow the format 'xxxx:xx:xxxxxx'");
                }
                break;
            case 'B':
            case 'b':
                {
                    if(sscanf(optionArgument, "%s", baudRate) != 1)
                    {
                        csrBtAppCmdlineUsage(argv[0], "Invalid 'B'/'b' argument");
                    }
                }
                break;
            case 'C':
            case 'c':
                {
                    if(sscanf(optionArgument, "%s", casira_ComPort) != 1)
                    {
                        csrBtAppCmdlineUsage(argv[0], "Invalid 'C'/'c' argument");
                    }

                    break;
                }
#ifdef _WIN32
            case 'P':
            case 'p':
                {
                    csrBtAppListAvailableSerialPorts();
                    exit(0);
                }
#endif
            case 'F':
            case 'f':
                {
                    if (sscanf(optionArgument, "%hu", &romCrystalFreq) != 1)
                    {
                        csrBtAppCmdlineUsage(argv[0], "Invalid 'F'/'f' argument");
                    }
                    else
                    {
                        CsrBtBootstrapSetFrequency(NULL, romCrystalFreq);
                    }
                    break;
                }
            case 'H':
            case 'h':
                {
                    csrBtAppCmdlineUsage(argv[0], NULL);
                }
#ifdef CSR_LOG_ENABLE
            case 'L':
            case 'l':
                {
                    if(sscanf(optionArgument, "0x%4x", &logTaskLevel) != 1)
                    {
                        csrBtAppCmdlineUsage(argv[0], "Option 'L'/'l' - Invalid log level format, use '0xXXXX'");
                    }
                    break;
                }
            case 'E':
            case 'e':
                {
                    if(sscanf(optionArgument, "0x%4x", &logEnvLevel) != 1)
                    {
                        csrBtAppCmdlineUsage(argv[0], "Option 'E'/'e' - Invalid log level format, use '0xXXXX'");
                    }
                    break;
                }
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
            case 'T':
            case 't':
                {
                    if (CsrStrCmp(optionArgument, "BCSP")==0 || CsrStrCmp(optionArgument, "bcsp")==0)
                    {
                        transportType = TRANSPORT_TYPE_BCSP;
                    }
                    else if (CsrStrCmp(optionArgument, "USB")==0 || CsrStrCmp(optionArgument, "usb")==0)
                    {
                        transportType = TRANSPORT_TYPE_USB;
                    }
                    else if (CsrStrCmp(optionArgument, "H4")==0 || CsrStrCmp(optionArgument, "h4")==0)
                    {
                        transportType = TRANSPORT_TYPE_H4_DS;
                    }
                    else if(CsrStrCmp(optionArgument,"H4i") == 0 || CsrStrCmp(optionArgument, "h4i") == 0)
                    {
                        transportType = TRANSPORT_TYPE_H4_I;
                    }
                    else
                    {
                        csrBtAppCmdlineUsage(argv[0], "Invalid transport specified");
                    }
                    break;
                }
            case 'U':
            case 'u':
                {
                    if (sscanf(optionArgument, "%u", &romBaud) != 1)
                    {
                        csrBtAppCmdlineUsage(argv[0], "Invalid 'U'/'u' argument");
                    }
                    else
                    {
                        CsrBtBootstrapSetBitRate(NULL, romBaud);
                    }
                    break;
                }
            case 'X':
            case 'x':
                {
                    if (!csrBtAppAddrFromString(optionArgument, &romBdAddress))
                    {
                        csrBtAppCmdlineUsage(argv[0], "Option 'X'/'x' argument must use format 'xxxx:xx:xxxxxx'");
                    }
                    else
                    {
                        CsrBtBootstrapSetLocalBtAddress(NULL, &romBdAddress);
                        bdAddrSet = TRUE;
                    }
                    break;
                }
            case 'S':
            case 's':
                {
                    CsrCharString psr[100];
                    CsrMemSet(psr, 0, sizeof(psr));
                    if( sscanf(optionArgument, "%s", psr) != 1)
                    {
                        csrBtAppCmdlineUsage(argv[0], "Invalid PSR bootstrap file argument");
                    }
                    else
                    {
                        CsrBtBootstrapParsePsrFile(NULL, psr);
                    }
                    break;
                }
            case '?':
                {
                    /* Arguments not specified correctly */
                    csrBtAppCmdlineUsage(argv[0], NULL);
                    break;
                }
            default:
                {
                    char * errMsg;
                    /* extra arguments from application - pass on
                     * if there are errors let application call usage */
                    if((errMsg = applicationCmdLineParse(ch, optionArgument)) == NULL)
                    {
                        /* app parsed argument */
                        break;
                    }
                    csrBtAppCmdlineUsage(argv[0], errMsg);
                    break;
                }
        }
    }

    /* Warn if address isn't set */
    if(!bdAddrSet)
    {
        printf("\n\n  Warning: The local Bluetooth address has not been specified with -X option.\n");
        printf("  The default Bluetooth address will be used.\n\n");

#ifndef __arm
        CsrBtSleep(5);
#endif
    }
}
