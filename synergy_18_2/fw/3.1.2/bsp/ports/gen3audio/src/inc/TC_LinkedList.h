/****************************************************************************
 *   FileName    : TC_LinkedList.h
 *   Description : Double Linked List Header File
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/
#ifndef _TC_LINKED_LIST_
#define _TC_LINKED_LIST_

/****************************************************************************
 *   Data Structures
 ****************************************************************************/
typedef struct
{
	void		*PrevEntry;
	void		*NextEntry;
	void		*pDataStr;
} TC_LL_Entry;

typedef struct
{
	TC_LL_Entry	*FirstEntry;
	TC_LL_Entry	*LastEntry;
} TC_LL_List;

/****************************************************************************
 *   External Functions
 ****************************************************************************/
extern void TC_LL_InsertAfter(TC_LL_List *list, TC_LL_Entry *node, TC_LL_Entry *newNode);
extern void TC_LL_InsertBefore(TC_LL_List *list, TC_LL_Entry *node, TC_LL_Entry *newNode);
extern void TC_LL_InsertFirst(TC_LL_List *list, TC_LL_Entry *newNode);
extern void TC_LL_InsertLast(TC_LL_List *list, TC_LL_Entry *newNode);
extern void TC_LL_Remove(TC_LL_List *list, TC_LL_Entry *node);

#endif /* _TC_LINKED_LIST_ */