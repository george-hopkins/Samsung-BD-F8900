#ifndef BEFrame_h
#define BEFrame_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BEFrameLoadStateProvisional = 0,
    BEFrameLoadStateCommitted = 1,
    BEFrameLoadStateFinished = 2
} BEFrameLoadState;

bool BEFrameIsMainFrame(BEFrameRef frame);
BEFrameLoadState BEFrameGetFrameLoadState(BEFrameRef frame);
char* BEFrameCopyProvisionalURL(BEFrameRef frame);
char* BEFrameCopyURL(BEFrameRef frame);
char* BEFrameCopyUnreachableURL(BEFrameRef frame);

BEPageRef BEFrameGetPage(BEFrameRef frame);

#ifdef __cplusplus
}
#endif

#endif /* BEFrame_h */
