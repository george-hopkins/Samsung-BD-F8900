/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_file.h"
#include "csr_bt_obex_ftp_common.h"
#include "csr_bt_obex.h"

/*Delete directory and its containent*/
int recursiveRmDir(char* targetDir)
{
    int           res;
    DIR           *dir;
    struct dirent *entry;
    CsrBtStat_t    info;
    char*         dirPath;

    CsrBtChdir(targetDir);
    dirPath = CsrPmemAlloc(CSR_BT_MAX_PATH_LENGTH);
    CsrBtGetCwd(dirPath, CSR_BT_MAX_PATH_LENGTH);
    CsrStrCat(dirPath, PATH_SEP);
    /* Read directory */
    dir = CsrBtOpenDir(dirPath);
    if(dir)
    {
        while((entry = CsrBtReadDir(dir)) != NULL)
        {
        /*Special cases*/
        if(CsrStrNCmp(entry->d_name, "..", 2) && CsrStrNCmp(entry->d_name, ".", 1))
        {
        CsrMemSet(&info, 0, sizeof(CsrBtStat_t));
        if(CsrBtStat(entry->d_name, &info) == 0)
        {
            /*Is entry a directory?*/
            if((info.st_mode & S_IFDIR) == S_IFDIR)
            {
               recursiveRmDir(entry->d_name);
            }
            else
            {
                res = CsrBtRemove(entry->d_name);
                if(res != 0)
                {
                    CsrBtChdir("..");
                    CsrPmemFree(dirPath);
                    CsrBtCloseDir(dir);
                    return res;
                }
            }
        }
        }
        }
    }
    CsrBtChdir("..");
    CsrPmemFree(dirPath);
    CsrBtCloseDir(dir);
    res = CsrBtRmDir(targetDir);
    return res;
}


#ifdef CSR_BT_INSTALL_UTIL_UCS2
CsrUint8 *unicode2String(CsrUcs2String* unicode)
{
    CsrUint8 *str;

    str = CsrUcs2ByteString2Utf8(unicode);
    return str;
}
#endif
CsrFilePerms mapObexPermToFilePerm(CsrUint32 obexPermission,CsrBool *res)
{
    CsrFilePerms filePermission = 0;

    /* Convert OBEX permission to File permission */
    if ((obexPermission & CSR_BT_OBEX_USER_PERMISSIONS_READ_MASK) == CSR_BT_OBEX_USER_PERMISSIONS_READ_MASK)
    {
        filePermission |= CSR_FILE_PERMS_USER_READ;
        *res = TRUE;
    }
    else if ((obexPermission & CSR_BT_OBEX_USER_PERMISSIONS_WRITE_MASK) == CSR_BT_OBEX_USER_PERMISSIONS_WRITE_MASK)
    {
        filePermission |= CSR_FILE_PERMS_USER_WRITE;
        *res = TRUE;
    }
    else if ((obexPermission & CSR_BT_OBEX_USER_PERMISSIONS_DELETE_MASK) == CSR_BT_OBEX_USER_PERMISSIONS_DELETE_MASK)
    {
        filePermission = 0;
        *res = FALSE;
    }
    else if ((obexPermission & CSR_BT_OBEX_USER_PERMISSIONS_MODIFY_MASK) == CSR_BT_OBEX_USER_PERMISSIONS_MODIFY_MASK)
    {
        filePermission = 0;
        *res = FALSE;
    }
    else if ((obexPermission & CSR_BT_OBEX_GROUP_PERMISSIONS_READ_MASK) == CSR_BT_OBEX_GROUP_PERMISSIONS_READ_MASK)
    {
        filePermission |= CSR_FILE_PERMS_GROUP_READ;
        *res = TRUE;
    }
    else if ((obexPermission & CSR_BT_OBEX_GROUP_PERMISSIONS_WRITE_MASK) == CSR_BT_OBEX_GROUP_PERMISSIONS_WRITE_MASK)
    {
        filePermission |= CSR_FILE_PERMS_GROUP_WRITE;
        *res = TRUE;
    }
    else if ((obexPermission & CSR_BT_OBEX_GROUP_PERMISSIONS_DELETE_MASK) == CSR_BT_OBEX_GROUP_PERMISSIONS_DELETE_MASK)
    {
        filePermission = 0;
        *res = FALSE;
    }
    else if ((obexPermission & CSR_BT_OBEX_GROUP_PERMISSIONS_MODIFY_MASK) == CSR_BT_OBEX_GROUP_PERMISSIONS_MODIFY_MASK)
    {
        filePermission = 0;
        *res = FALSE;
    }
    else  if ((obexPermission & CSR_BT_OBEX_OTHER_PERMISSIONS_READ_MASK) == CSR_BT_OBEX_OTHER_PERMISSIONS_READ_MASK)
    {
        filePermission |= CSR_FILE_PERMS_OTHERS_READ;
        *res = TRUE;
    }
    else if ((obexPermission & CSR_BT_OBEX_OTHER_PERMISSIONS_WRITE_MASK) == CSR_BT_OBEX_OTHER_PERMISSIONS_WRITE_MASK)
    {
        filePermission |= CSR_FILE_PERMS_OTHERS_WRITE;
        *res = TRUE;
    }
    else if ((obexPermission & CSR_BT_OBEX_OTHER_PERMISSIONS_DELETE_MASK) == CSR_BT_OBEX_OTHER_PERMISSIONS_DELETE_MASK)
    {
        filePermission = 0;
        *res = FALSE;
    }
    else if ((obexPermission & CSR_BT_OBEX_OTHER_PERMISSIONS_MODIFY_MASK) == CSR_BT_OBEX_USER_PERMISSIONS_MODIFY_MASK)
    {
        filePermission = 0;
        *res = FALSE;
    }

    return(filePermission);
}
