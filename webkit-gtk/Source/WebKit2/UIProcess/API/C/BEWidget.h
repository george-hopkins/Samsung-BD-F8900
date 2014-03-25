#ifndef BEWidget_h
#define BEWidget_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

BEViewRef BEWidgetViewCreate(BEContextRef context, const char* widgetID);

char* BEWidgetPageGetWidgetID(BEViewRef view);
bool BEWidgetPageRunJSWidgetEvent(BEPageRef page, const char* eventType, char* eventData, int keyCode);
bool BEWidgetPageRunJSWidgetAppEvent(BEPageRef page, const char* eventType, char* eventData, long type);

#ifdef __cplusplus
}
#endif

#endif /* BEWidget_h */
