#ifndef BEPopupMenu_h
#define BEPopupMenu_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

void BEPopupMenuSetSelectedIndex(BEPageRef page, int index);

int BEPopupMenuItemsGetSize(BEPopupMenuItemsRef items);

bool BEPopupMenuItemsIsSeparator(BEPopupMenuItemsRef items, int index);
bool BEPopupMenuItemsIsEnabled(BEPopupMenuItemsRef items, int index);
bool BEPopupMenuItemsIsLabel(BEPopupMenuItemsRef items, int index);
char* BEPopupMenuItemsCopyText(BEPopupMenuItemsRef items, int index);
char* BEPopupMenuItemsCopyToolTip(BEPopupMenuItemsRef items, int index);

#ifdef __cplusplus
}
#endif

#endif /* BEPopupMenu_h */
