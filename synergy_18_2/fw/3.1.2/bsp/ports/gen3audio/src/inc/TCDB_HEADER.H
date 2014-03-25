/****************************************************************************
 *   FileName    : TCDB_HEADER.H
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#ifndef __TCDB_H_INCLUDED
#define __TCDB_H_INCLUDED

#define BIT_TCDB_DB_USED					(0x01U)
#define BIT_TCDB_DB_BE_DELETED				(0x02U)
#define BIT_TCDB_NORMAL_FLAG				(0x04U)
#define BIT_TCDB_CD_FLAG					(0x08U)
#define BIT_TCDB_MTP_HOST_FLAG				(0x10U)

typedef enum
{
	INTERNAL_STORAGE = 0,
	EXTERNAL_STORAGE
} STORAGE_TYPE;

extern int TCDB_GetIsFileDBUsed(unsigned int iIndex);

extern int TCDB_GetIsFolderDBUsed(unsigned int iIndex);

extern int TCDB_GetIsFileDBBeDeleted(unsigned int iIndex);

extern int TCDB_GetIsFolderDBBeDeleted(unsigned int iIndex);

extern int TCDB_GetIsFileDBNotUsed(unsigned int iIndex);

extern int TCDB_GetIsFolderDBNotUsed(unsigned int iIndex);

extern void TCDB_SetFileDBUsed(unsigned int iIndex);

extern void TCDB_SetFolderDBUsed(unsigned int iIndex);

extern void TCDB_SetFileDBBeDeleted(unsigned int iIndex);

extern void TCDB_SetFolderDBBeDeleted(unsigned int iIndex);

#endif	//__TCDB_H_INCLUDED

/* end of file */

