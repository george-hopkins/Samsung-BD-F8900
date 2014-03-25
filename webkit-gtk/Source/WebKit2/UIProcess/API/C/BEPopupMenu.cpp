#include "config.h"
#include "BEPopupMenu.h"

#include "BEAPICast.h"
#include "WebPageProxy.h"
#include "WebPopupItem.h"

#include <wtf/Vector.h>

using namespace WebKit;

void BEPopupMenuSetSelectedIndex(BEPageRef pageRef, int index)
{
    toBEImpl(pageRef)->setSelectedIndexForActivePopupMenu(index);
}

int BEPopupMenuItemsGetSize(BEPopupMenuItemsRef itemsRef)
{
    const Vector<WebPopupItem>* items = static_cast<const Vector<WebPopupItem>*>(itemsRef);
    return items->size();
}

bool BEPopupMenuItemsIsSeparator(BEPopupMenuItemsRef itemsRef, int index)
{
    const Vector<WebPopupItem>* items = static_cast<const Vector<WebPopupItem>*>(itemsRef);
    return (*items)[index].m_type == WebPopupItem::Separator;
}

bool BEPopupMenuItemsIsEnabled(BEPopupMenuItemsRef itemsRef, int index)
{
    const Vector<WebPopupItem>* items = static_cast<const Vector<WebPopupItem>*>(itemsRef);
    return (*items)[index].m_isEnabled;
}

bool BEPopupMenuItemsIsLabel(BEPopupMenuItemsRef itemsRef, int index)
{
    const Vector<WebPopupItem>* items = static_cast<const Vector<WebPopupItem>*>(itemsRef);
    return (*items)[index].m_isLabel;
}

char* BEPopupMenuItemsCopyText(BEPopupMenuItemsRef itemsRef, int index)
{
    const Vector<WebPopupItem>* items = static_cast<const Vector<WebPopupItem>*>(itemsRef);
    return toCopiedBEAPI((*items)[index].m_text);
}

char* BEPopupMenuItemsCopyToolTip(BEPopupMenuItemsRef itemsRef, int index)
{
    const Vector<WebPopupItem>* items = static_cast<const Vector<WebPopupItem>*>(itemsRef);
    return toCopiedBEAPI((*items)[index].m_toolTip);
}
