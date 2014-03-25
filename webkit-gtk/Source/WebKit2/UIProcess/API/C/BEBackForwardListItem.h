#ifndef BEBackForwardListItem_h
#define BEBackForwardListItem_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

char* BEBackForwardListItemCopyURL(BEBackForwardListItemRef item);
char* BEBackForwardListItemCopyTitle(BEBackForwardListItemRef item);

#ifdef __cplusplus
}
#endif

#endif /* BEBackForwardListItem_h */
