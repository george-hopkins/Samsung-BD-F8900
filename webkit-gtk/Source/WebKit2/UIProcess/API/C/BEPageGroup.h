#ifndef BEPageGroup_h
#define BEPageGroup_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

BEPageGroupRef BEPageGroupCreateWithIdentifier(const char* identifier);

void BEPageGroupSetPreferences(BEPageGroupRef pageGroup, BEPreferencesRef preferences);
BEPreferencesRef BEPageGroupGetPreferences(BEPageGroupRef pageGroup);

#ifdef __cplusplus
}
#endif

#endif /* BEPageGroup_h */
