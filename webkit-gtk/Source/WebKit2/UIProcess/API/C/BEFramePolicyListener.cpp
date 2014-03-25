#include "config.h"
#include "BEFramePolicyListener.h"
#include "BEAPICast.h"
#include "WebFramePolicyListenerProxy.h"

using namespace WebKit;

void BEFramePolicyListenerUse(BEFramePolicyListenerRef policyListenerRef)
{
    toBEImpl(policyListenerRef)->use();
}

void BEFramePolicyListenerDownload(BEFramePolicyListenerRef policyListenerRef)
{
    toBEImpl(policyListenerRef)->download();
}

void BEFramePolicyListenerIgnore(BEFramePolicyListenerRef policyListenerRef)
{
    toBEImpl(policyListenerRef)->ignore();
}
