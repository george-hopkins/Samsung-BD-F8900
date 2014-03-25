#include "config.h"
#include "BEBackForwardList.h"
#include "BEAPICast.h"
#include "WebBackForwardList.h"

using namespace WebKit;

BEBackForwardListItemRef BEBackForwardListGetCurrentItem(BEBackForwardListRef listRef)
{
    return toBEAPI(toBEImpl(listRef)->currentItem());
}

BEBackForwardListItemRef BEBackForwardListGetBackItem(BEBackForwardListRef listRef)
{
    return toBEAPI(toBEImpl(listRef)->backItem());
}

BEBackForwardListItemRef BEBackForwardListGetForwardItem(BEBackForwardListRef listRef)
{
    return toBEAPI(toBEImpl(listRef)->forwardItem());
}

BEBackForwardListItemRef BEBackForwardListGetItemAtIndex(BEBackForwardListRef listRef, int index)
{
    return toBEAPI(toBEImpl(listRef)->itemAtIndex(index));
}

unsigned BEBackForwardListGetBackListCount(BEBackForwardListRef listRef)
{
    return toBEImpl(listRef)->backListCount();
}

unsigned BEBackForwardListGetForwardListCount(BEBackForwardListRef listRef)
{
    return toBEImpl(listRef)->forwardListCount();
}
