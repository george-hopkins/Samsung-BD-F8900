#include "config.h"
#include "BEPageGroup.h"
#include "BEAPICast.h"
#include "WebPageGroup.h"

using namespace WebKit;

BEPageGroupRef BEPageGroupCreateWithIdentifier(const char* identifier)
{
    RefPtr<WebPageGroup> pageGroup = WebPageGroup::create(String::fromUTF8(identifier));
    return toBEAPI(pageGroup.release().leakRef());
}

void BEPageGroupSetPreferences(BEPageGroupRef pageGroupRef, BEPreferencesRef preferencesRef)
{
    toBEImpl(pageGroupRef)->setPreferences(toBEImpl(preferencesRef));
}

BEPreferencesRef BEPageGroupGetPreferences(BEPageGroupRef pageGroupRef)
{
    return toBEAPI(toBEImpl(pageGroupRef)->preferences());
}
