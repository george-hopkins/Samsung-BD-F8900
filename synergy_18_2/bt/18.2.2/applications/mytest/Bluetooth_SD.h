/**********************************************************************
* File Name   : Bluetooth_SD.h
* Description : This file define functions and variables for Service Discovery.
* Author 	  :	Kim Jin Cheol	(Connectivity Team, HYUNDAI MOBIS)
* Updates			:
***********************************************************************/

#ifndef _BLUETOOTH_SD_H_
#define _BLUETOOTH_SD_H_

#include "csr_types.h"


#define MAX_SEARCH_LIST_NUM	10

typedef struct {
	CsrUint16		qid;

	void 			*recvMsgP;
	
	CsrUint32	searchFilter;
	CsrUint8		searchTime;
	CsrUint8		searchNumMax;

	CsrUint8		selectedIndex;  
	CsrUint8 	searchListNum; //count  of search result
	//devicePropertiesType	searchList[MAX_SEARCH_LIST_NUM];
} SdAppInst;


extern void init_sd(void **gash);
extern void deinit_sd(void **gash);
extern void handleSdPrim(SdAppInst  *inst);



#endif /*_BLUETOOTH_SD_H_*/
