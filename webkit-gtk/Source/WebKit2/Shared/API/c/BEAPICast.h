#ifndef BEAPICast_h
#define BEAPICast_h

#include "WKAPICast.h"
#include "BEBase.h"
#include <wtf/text/WTFString.h>
#include <wtf/unicode/UTF8.h>

typedef struct _WebKitWebViewBase WebKitWebViewBase;

namespace WebKit {

class WebApplicationCacheManagerProxy;
class WebBackForwardList;
class WebBackForwardListItem;
class WebContext;
class WebCookieManagerProxy;
class WebDatabaseManagerProxy;
class WebError;
class WebFrameProxy;
class WebFramePolicyListenerProxy;
class WebGeolocationManagerProxy;
class WebIconDatabase;
class WebKeyValueStorageManagerProxy;
class WebMediaCacheManagerProxy;
class WebPageGroup;
class WebPageProxy;
class WebPluginSiteDataManager;
class WebPreferences;
class WebResourceCacheManagerProxy;
class WebURLRequest;

template<typename APIType> struct BEAPITypeInfo { };
template<typename ImplType> struct BEImplTypeInfo { };

#define BE_ADD_API_MAPPING(TheAPIType, TheImplType) \
    template<> struct BEAPITypeInfo<TheAPIType> { typedef TheImplType* ImplType; }; \
    template<> struct BEImplTypeInfo<TheImplType*> { typedef TheAPIType APIType; };

BE_ADD_API_MAPPING(BESerializedScriptValueRef, WebSerializedScriptValue)
BE_ADD_API_MAPPING(BEURLRequestRef, WebURLRequest)
BE_ADD_API_MAPPING(BEURLResponseRef, WebURLResponse)

BE_ADD_API_MAPPING(BEApplicationCacheManagerRef, WebApplicationCacheManagerProxy)
BE_ADD_API_MAPPING(BEBackForwardListRef, WebBackForwardList)
BE_ADD_API_MAPPING(BEBackForwardListItemRef, WebBackForwardListItem)
BE_ADD_API_MAPPING(BEContextRef, WebContext)
BE_ADD_API_MAPPING(BECookieManagerRef, WebCookieManagerProxy)
BE_ADD_API_MAPPING(BEDatabaseManagerRef, WebDatabaseManagerProxy)
BE_ADD_API_MAPPING(BEErrorRef, WebError)
BE_ADD_API_MAPPING(BEFrameRef, WebFrameProxy)
BE_ADD_API_MAPPING(BEFramePolicyListenerRef, WebFramePolicyListenerProxy)
BE_ADD_API_MAPPING(BEGeolocationManagerRef, WebGeolocationManagerProxy)
BE_ADD_API_MAPPING(BEIconDatabaseRef, WebIconDatabase)
BE_ADD_API_MAPPING(BEImageRef, WebImage)
BE_ADD_API_MAPPING(BEKeyValueStorageManagerRef, WebKeyValueStorageManagerProxy)
BE_ADD_API_MAPPING(BEMediaCacheManagerRef, WebMediaCacheManagerProxy)
BE_ADD_API_MAPPING(BEPageRef, WebPageProxy)
BE_ADD_API_MAPPING(BEPageGroupRef, WebPageGroup)
BE_ADD_API_MAPPING(BEPluginSiteDataManagerRef, WebPluginSiteDataManager)
BE_ADD_API_MAPPING(BEPreferencesRef, WebPreferences)
BE_ADD_API_MAPPING(BEResourceCacheManagerRef, WebResourceCacheManagerProxy)
BE_ADD_API_MAPPING(BEStringRef, WebString)
BE_ADD_API_MAPPING(BEViewRef, WebKitWebViewBase)

template<typename T>
inline typename BEAPITypeInfo<T>::ImplType toBEImpl(T t)
{
    // An example of the conversions that take place:
    // const struct OpaqueWKArray* -> const struct OpaqueWKArray -> struct OpaqueWKArray -> struct OpaqueWKArray* -> ImmutableArray*

    typedef typename WTF::RemovePointer<T>::Type PotentiallyConstValueType;
    typedef typename WTF::RemoveConst<PotentiallyConstValueType>::Type NonConstValueType;

    return reinterpret_cast<typename BEAPITypeInfo<T>::ImplType>(const_cast<NonConstValueType*>(t));
}

template<typename T>
inline typename BEImplTypeInfo<T>::APIType toBEAPI(T t)
{
    return reinterpret_cast<typename BEImplTypeInfo<T>::APIType>(t);
}

inline char* toCopiedBEAPI(const String& string)
{
    size_t bufferSize = string.length() * 3 + 1;
    char* buffer = static_cast<char*>(malloc(bufferSize));
    if (!buffer)
        return 0;
    char* p = buffer;
    const UChar* d = string.characters();
    WTF::Unicode::ConversionResult result = WTF::Unicode::convertUTF16ToUTF8(&d, d + string.length(), &p, p + bufferSize - 1, /* strict */ true);
    *p++ = '\0';
    if (result != WTF::Unicode::conversionOK && result != WTF::Unicode::targetExhausted)
        return 0;
    return buffer;
}

inline BETypeID toBEAPI(APIObject::Type type)
{
    return static_cast<BETypeID>(type);
}

}

#endif // BEAPICast_h
