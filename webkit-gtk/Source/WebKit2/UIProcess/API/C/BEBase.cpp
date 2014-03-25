#include "config.h"
#include "BEBase.h"
#include "BELog.h"

#include "APIObject.h"
#include "BEAPICast.h"
#include "CString.h"
#include "ShareableBitmap.h"
#include "WebImage.h"
#include "WebSerializedScriptValue.h"
#include "WebURLRequest.h"

#include <WebCore/PlatformContextCairo.h>
#include <WebCore/SerializedScriptValue.h>

using namespace WebKit;

BETypeID BEGetTypeID(BETypeRef typeRef)
{
    return toBEAPI(static_cast<APIObject*>(const_cast<void*>(typeRef))->type());
}

BETypeRef BERetain(BETypeRef typeRef)
{
    static_cast<APIObject*>(const_cast<void*>(typeRef))->ref();
    return typeRef;
}

void BERelease(BETypeRef typeRef)
{
    static_cast<APIObject*>(const_cast<void*>(typeRef))->deref();
}

char* BEURLRequestCopyURL(BEURLRequestRef requestRef)
{
    return toCopiedBEAPI(toBEImpl(requestRef)->url());
}

char* BEURLResponseCopyMimeType(BEURLResponseRef responseRef)
{
    return toCopiedBEAPI(toBEImpl(responseRef)->resourceResponse().mimeType());
}

BEStringRef BEStringCreateWithUTF8CString(const char* string)
{
    RefPtr<WebString> webString = WebString::createFromUTF8String(string);
    return toBEAPI(webString.release().leakRef());
}

size_t BEStringGetMaximumUTF8CStringSize(BEStringRef stringRef)
{
    return toBEImpl(stringRef)->maximumUTF8CStringSize();
}

size_t BEStringGetUTF8CString(BEStringRef stringRef, char* buffer, size_t bufferSize)
{
    return toBEImpl(stringRef)->getUTF8CString(buffer, bufferSize);
}

char* BEErrorCopyDomain(BEErrorRef errorRef)
{
    return toCopiedBEAPI(toBEImpl(errorRef)->domain());
}

int BEErrorGetErrorCode(BEErrorRef errorRef)
{
    return toBEImpl(errorRef)->errorCode();
}

char* BEErrorCopyFailingURL(BEErrorRef errorRef)
{
    return toCopiedBEAPI(toBEImpl(errorRef)->failingURL());
}

char* BESerializedScriptValueToString(BESerializedScriptValueRef scriptValueRef)
{
    WebCore::SerializedScriptValue* scriptValue = static_cast<WebCore::SerializedScriptValue*>(toBEImpl(scriptValueRef)->internalRepresentation());
    if (!scriptValue)
        return 0;

    return toCopiedBEAPI(scriptValue->toString());
}

cairo_surface_t* BEImageCreateCairoSurface(BEImageRef imageRef)
{
    return toBEImpl(imageRef)->bitmap()->createCairoSurface().leakRef();
}

void BESetLogLevel(unsigned int level)
{
    BELogSetLevel(level);
}
