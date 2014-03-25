#include "config.h"
#include "BEFrame.h"
#include "BEAPICast.h"
#include "WebFrameProxy.h"

using namespace WebKit;

bool BEFrameIsMainFrame(BEFrameRef frameRef)
{
    return toBEImpl(frameRef)->isMainFrame();
}

BEFrameLoadState BEFrameGetFrameLoadState(BEFrameRef frameRef)
{
    WebFrameProxy* frame = toBEImpl(frameRef);
    switch (frame->loadState()) {
        case WebFrameProxy::LoadStateProvisional:
            return BEFrameLoadStateProvisional;
        case WebFrameProxy::LoadStateCommitted:
            return BEFrameLoadStateCommitted;
        case WebFrameProxy::LoadStateFinished:
            return BEFrameLoadStateFinished;
    }

    ASSERT_NOT_REACHED();
    return BEFrameLoadStateFinished;
}

char* BEFrameCopyProvisionalURL(BEFrameRef frameRef)
{
    return toCopiedBEAPI(toBEImpl(frameRef)->provisionalURL());
}

char* BEFrameCopyURL(BEFrameRef frameRef)
{
    return toCopiedBEAPI(toBEImpl(frameRef)->url());
}

char* BEFrameCopyUnreachableURL(BEFrameRef frameRef)
{
    return toCopiedBEAPI(toBEImpl(frameRef)->unreachableURL());
}

BEPageRef BEFrameGetPage(BEFrameRef frameRef)
{
    return toBEAPI(toBEImpl(frameRef)->page());
}
