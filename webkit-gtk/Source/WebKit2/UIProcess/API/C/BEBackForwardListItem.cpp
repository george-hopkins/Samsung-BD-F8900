#include "config.h"
#include "BEBackForwardListItem.h"
#include "BEAPICast.h"
#include "WebBackForwardListItem.h"

using namespace WebKit;

char* BEBackForwardListItemCopyURL(BEBackForwardListItemRef itemRef)
{
    return toCopiedBEAPI(toBEImpl(itemRef)->url());
}

char* BEBackForwardListItemCopyTitle(BEBackForwardListItemRef itemRef)
{
    return toCopiedBEAPI(toBEImpl(itemRef)->title());
}
