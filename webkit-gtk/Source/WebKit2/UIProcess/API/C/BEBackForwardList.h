#ifndef BEBackForwardList_h
#define BEBackForwardList_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

BEBackForwardListItemRef BEBackForwardListGetCurrentItem(BEBackForwardListRef list);
BEBackForwardListItemRef BEBackForwardListGetBackItem(BEBackForwardListRef list);
BEBackForwardListItemRef BEBackForwardListGetForwardItem(BEBackForwardListRef list);
BEBackForwardListItemRef BEBackForwardListGetItemAtIndex(BEBackForwardListRef list, int index);

unsigned BEBackForwardListGetBackListCount(BEBackForwardListRef list);
unsigned BEBackForwardListGetForwardListCount(BEBackForwardListRef list);

#ifdef __cplusplus
}
#endif

#endif /* BEBackForwardList_h */
