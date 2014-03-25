/****************************************************************************
 *   FileName    : TCDB_LISTDB.H
 *   Description :
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/

#ifndef __TCC_LISTDB_H_INCLUDED
#define __TCC_LISTDB_H_INCLUDED

#if 0
#ifndef _BROWSE_H_
#include "browse/browse.h"
#endif
#endif

typedef enum
{
	FILE_TYPE = 0,
	FOLDER_TYPE
} _DB_ENUM_TYPE;


extern _FILE_LIST_TYPE *TCDB_LIST_GetFileList(unsigned int iIndex);
extern _FOLDER_LIST_TYPE * TCDB_LIST_GetFolderList(unsigned int iIndex);
#ifdef MULTI_DB_INCLUDE
extern unsigned char *TCDB_LIST_GetDBNameBuffer(unsigned int iIndex, unsigned char ucAssociation);
#endif

extern unsigned char *TCDB_LIST_AllocFileListAddr(unsigned int iIndex);
extern unsigned char *TCDB_LIST_AllocFolderListAddr(unsigned int iIndex);


/* To set DB List attribute Function */
extern void TCDB_LIST_SetFileCodec(unsigned int iIndex, unsigned char ucCodec);
extern void TCDB_LIST_SetMotherIndex(unsigned int iIndex, unsigned int iMotherIndex, unsigned char ucAssociation);
extern void TCDB_LIST_SetChildFileCount( unsigned int iIndex, int FileCount );
extern void TCDB_LIST_SetChildFolderCount( unsigned int iIndex, int FolderCount );
extern void TCDB_LIST_SetPartID(unsigned int iIndex, int iPartID, unsigned char ucAssociation);
extern void TCDB_LIST_SetCluster(unsigned int iIndex, unsigned int ulCluster, unsigned char ucAssociation);
extern void TCDB_LIST_SetRealFolderIndex (unsigned int iIndex, unsigned int iRealIndex );
extern void TCDB_LIST_SetRealFileIndex (unsigned int iIndex, unsigned int iRealIndex);
extern void TCDB_LIST_SetFileAttr(unsigned int iIndex, unsigned char ucAttr);

extern void TCDB_LIST_SetSubIndex(unsigned int iIndex, unsigned short usSetIndex, unsigned char ucAssociation);
extern void TCDB_LIST_SetNextIndex(unsigned int iIndex, unsigned short usSetIndex, unsigned char ucAssociation);
#if 0 
extern void TCDB_SetFileDBUsed(unsigned int iIndex);
extern void TCDB_SetFolderDBUsed(unsigned int iIndex);
#endif

/* To get DB List attribute Function */
extern unsigned char TCDB_LIST_GetCurrentCodec (unsigned int iIndex );
extern unsigned int TCDB_LIST_GetMotherIndex(unsigned int iIndex, unsigned char ucAssociation);
extern unsigned int TCDB_LIST_GetChildFileCount( unsigned int iIndex );
extern unsigned int TCDB_LIST_GetChildFolderCount( unsigned int iIndex );
extern unsigned int TCDB_LIST_GetRealFolderIndex (unsigned int iIndex );
extern unsigned int TCDB_LIST_GetRealFileIndex (unsigned int iIndex );
extern unsigned int TCDB_LIST_GetPartID(unsigned int iIndex, unsigned char ucAssociation);
#if 0 
extern int TCDB_GetIsFileDBUsed(unsigned int iIndex);
extern int TCDB_GetIsFolderDBUsed(unsigned int iIndex);
#endif
extern unsigned int TCDB_LIST_GetCluster(unsigned int iIndex, unsigned char ucAssociation);
extern unsigned char TCDB_LIST_GetFileAttr(unsigned int iIndex);

extern unsigned short TCDB_LIST_GetSubIndex(unsigned int iIndex, unsigned char ucAssociation);
extern unsigned short TCDB_LIST_GetNextIndex(unsigned int iIndex,  unsigned char ucAssociation);
extern unsigned char TCDB_LIST_GetHeader(unsigned int iIndex, unsigned char ucAssociation);
#ifdef MTP_HOST_INCLUDE
extern unsigned int TCDB_LIST_GetMTPHostHandle(unsigned int iIndex,  unsigned char ucAssociation);
#endif
extern unsigned int TCDB_LIST_GetListSize(unsigned char ucAssociation);

/* To clear DB List Function */
extern void TCDB_LIST_ClearListArray(unsigned int iIndex, unsigned int iNumOfList, unsigned char ucAssociation);


/* To increase/decrease DB List count Function */
extern void TCDB_LIST_IncreaseChildFolderCount( unsigned int iIndex );
extern void TCDB_LIST_DecreaseChildFolderCount( unsigned int iIndex );
extern void TCDB_LIST_IncreaseChildFileCount( unsigned int iIndex );
extern void TCDB_LIST_DecreaseChildFileCount( unsigned int iIndex );

#ifdef CD_INCLUDE
extern _FILE_LIST_TYPE *TCDB_LIST_GetCDUpdateFileInfo(int iIndex);
#endif

#endif	//__TCC_LISTDB_H_INCLUDED

/* end of file */

