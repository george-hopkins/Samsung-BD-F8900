#ifndef BEFramePolicyListener_h
#define BEFramePolicyListener_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

void BEFramePolicyListenerUse(BEFramePolicyListenerRef policyListener);
void BEFramePolicyListenerDownload(BEFramePolicyListenerRef policyListener);
void BEFramePolicyListenerIgnore(BEFramePolicyListenerRef policyListener);

#ifdef __cplusplus
}
#endif

#endif /* BEFramePolicyListener_h */
