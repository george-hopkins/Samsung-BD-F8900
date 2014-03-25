#include "config.h"
#include "HTMLSnapshotImageElement.h"

#include "Attribute.h"
#include "CSSPropertyNames.h"
#include "CSSValueKeywords.h"
#include "Event.h"
#include "EventNames.h"
#include "FrameView.h"
#include "HTMLDocument.h"
#include "HTMLFormElement.h"
#include "HTMLNames.h"
#include "HTMLParserIdioms.h"
#include "RenderImage.h"
#include "ScriptEventListener.h"
#include "Settings.h"
#include "SnapshotDatabase.h"

using namespace std;

namespace WebCore {

using namespace HTMLNames;

HTMLSnapshotImageElement::HTMLSnapshotImageElement(const QualifiedName& tagName, Document* document)
    : HTMLElement(tagName, document)
    , m_image(0)
    , ismap(false)
    , m_compositeOperator(CompositeSourceOver)
{
    ASSERT(hasTagName(snapshotimageTag));
}

PassRefPtr<HTMLSnapshotImageElement> HTMLSnapshotImageElement::create(const QualifiedName& tagName, Document* document)
{
    if (document->settings()->isTVUIEnabled() || document->url().isLocalFile())
        return adoptRef(new HTMLSnapshotImageElement(tagName, document));
    else {
        RefPtr<HTMLElement> element = HTMLElement::create(tagName, document);
        return adoptRef(reinterpret_cast<HTMLSnapshotImageElement*>(element.release().leakRef()));
    }
}

HTMLSnapshotImageElement::~HTMLSnapshotImageElement()
{
    if (m_image)
        SnapshotDatabase::getDB().releaseSnapshotForImage(m_image, this);
}

bool HTMLSnapshotImageElement::mapToEntry(const QualifiedName& attrName, MappedAttributeEntry& result) const
{
    if (attrName == widthAttr ||
        attrName == heightAttr ||
        attrName == vspaceAttr ||
        attrName == hspaceAttr ||
        attrName == valignAttr) {
        result = eUniversal;
        return false;
    }

    if (attrName == borderAttr || attrName == alignAttr) {
        result = eReplaced; // Shared with embed and iframe elements.
        return false;
    }

    return HTMLElement::mapToEntry(attrName, result);
}

void HTMLSnapshotImageElement::parseMappedAttribute(Attribute* attr)
{
    const QualifiedName& attrName = attr->name();
    if (attrName == altAttr) {
        if (renderer() && renderer()->isImage())
            toRenderImage(renderer())->updateAltText();
    } else if (attrName == srcAttr) {
        if (m_image)
            SnapshotDatabase::getDB().releaseSnapshotForImage(m_image, this);
        SnapshotDatabase::getDB().retainSnapshotForURL(attr->value(), this);
    } else if (attrName == widthAttr)
        addCSSLength(attr, CSSPropertyWidth, attr->value());
    else if (attrName == heightAttr)
        addCSSLength(attr, CSSPropertyHeight, attr->value());
    else if (attrName == borderAttr) {
        // border="noborder" -> border="0"
        addCSSLength(attr, CSSPropertyBorderWidth, attr->value().toInt() ? attr->value() : "0");
        addCSSProperty(attr, CSSPropertyBorderTopStyle, CSSValueSolid);
        addCSSProperty(attr, CSSPropertyBorderRightStyle, CSSValueSolid);
        addCSSProperty(attr, CSSPropertyBorderBottomStyle, CSSValueSolid);
        addCSSProperty(attr, CSSPropertyBorderLeftStyle, CSSValueSolid);
    } else if (attrName == vspaceAttr) {
        addCSSLength(attr, CSSPropertyMarginTop, attr->value());
        addCSSLength(attr, CSSPropertyMarginBottom, attr->value());
    } else if (attrName == hspaceAttr) {
        addCSSLength(attr, CSSPropertyMarginLeft, attr->value());
        addCSSLength(attr, CSSPropertyMarginRight, attr->value());
    } else if (attrName == alignAttr)
        addHTMLAlignment(attr);
    else if (attrName == valignAttr)
        addCSSProperty(attr, CSSPropertyVerticalAlign, attr->value());
    else if (attrName == usemapAttr) {
        if (attr->value().string()[0] == '#')
            usemap = attr->value();
        else
            usemap = document()->completeURL(stripLeadingAndTrailingHTMLSpaces(attr->value())).string();
        setIsLink(!attr->isNull());
    } else if (attrName == ismapAttr)
        ismap = true;
    else if (attrName == onabortAttr)
        setAttributeEventListener(eventNames().abortEvent, createAttributeEventListener(this, attr));
    else if (attrName == onloadAttr)
        setAttributeEventListener(eventNames().loadEvent, createAttributeEventListener(this, attr));
    else if (attrName == onbeforeloadAttr)
        setAttributeEventListener(eventNames().beforeloadEvent, createAttributeEventListener(this, attr));
    else if (attrName == compositeAttr) {
        if (!parseCompositeOperator(attr->value(), m_compositeOperator))
            m_compositeOperator = CompositeSourceOver;
    } else if (attrName == nameAttr) {
        const AtomicString& newName = attr->value();
        if (inDocument() && document()->isHTMLDocument()) {
            HTMLDocument* document = static_cast<HTMLDocument*>(this->document());
            document->removeNamedItem(m_name);
            document->addNamedItem(newName);
        }
        m_name = newName;
    } else if (isIdAttributeName(attr->name())) {
        const AtomicString& newId = attr->value();
        if (inDocument() && document()->isHTMLDocument()) {
            HTMLDocument* document = static_cast<HTMLDocument*>(this->document());
            document->removeExtraNamedItem(m_id);
            document->addExtraNamedItem(newId);
        }
        m_id = newId;
        // also call superclass
        HTMLElement::parseMappedAttribute(attr);
    } else
        HTMLElement::parseMappedAttribute(attr);
}

String HTMLSnapshotImageElement::altText() const
{
    // lets figure out the alt text.. magic stuff
    // http://www.w3.org/TR/1998/REC-html40-19980424/appendix/notes.html#altgen
    // also heavily discussed by Hixie on bugzilla
    String alt = getAttribute(altAttr);
    // fall back to title attribute
    if (alt.isNull())
        alt = getAttribute(titleAttr);
    return alt;
}

RenderObject* HTMLSnapshotImageElement::createRenderer(RenderArena* arena, RenderStyle* style)
{
    if (style->hasContent())
        return RenderObject::createObject(this, style);

    RenderImage* image = new (arena) RenderImage(this);
    image->setImageResource(RenderImageResource::create());
    return image;
}

void HTMLSnapshotImageElement::attach()
{
    HTMLElement::attach();

    if (renderer() && renderer()->isImage() && m_image) {
        RenderImage* renderImage = toRenderImage(renderer());
        RenderImageResource* renderImageResource = renderImage->imageResource();
        if (renderImageResource->hasImage())
            return;
        renderImageResource->setCachedImage(m_image);
    }
}

void HTMLSnapshotImageElement::insertedIntoDocument()
{
    if (document()->isHTMLDocument()) {
        HTMLDocument* document = static_cast<HTMLDocument*>(this->document());
        document->addNamedItem(m_name);
        document->addExtraNamedItem(m_id);
    }

    HTMLElement::insertedIntoDocument();
}

void HTMLSnapshotImageElement::removedFromDocument()
{
    if (document()->isHTMLDocument()) {
        HTMLDocument* document = static_cast<HTMLDocument*>(this->document());
        document->removeNamedItem(m_name);
        document->removeExtraNamedItem(m_id);
    }

    HTMLElement::removedFromDocument();
}

void HTMLSnapshotImageElement::insertedIntoTree(bool deep)
{
    HTMLElement::insertedIntoTree(deep);
}

void HTMLSnapshotImageElement::removedFromTree(bool deep)
{
    HTMLElement::removedFromTree(deep);
}

int HTMLSnapshotImageElement::width(bool ignorePendingStylesheets) const
{
    if (!renderer()) {
        // check the attribute first for an explicit pixel value
        bool ok;
        int width = getAttribute(widthAttr).toInt(&ok);
        if (ok)
            return width;

        // if the image is available, use its width
        if (m_image)
            return m_image->imageSize(1.0f).width();
    }

    if (ignorePendingStylesheets)
        document()->updateLayoutIgnorePendingStylesheets();
    else
        document()->updateLayout();

    RenderBox* box = renderBox();
    return box ? adjustForAbsoluteZoom(box->contentWidth(), box) : 0;
}

int HTMLSnapshotImageElement::height(bool ignorePendingStylesheets) const
{
    if (!renderer()) {
        // check the attribute first for an explicit pixel value
        bool ok;
        int height = getAttribute(heightAttr).toInt(&ok);
        if (ok)
            return height;

        // if the image is available, use its height
        if (m_image)
            return m_image->imageSize(1.0f).height();
    }

    if (ignorePendingStylesheets)
        document()->updateLayoutIgnorePendingStylesheets();
    else
        document()->updateLayout();

    RenderBox* box = renderBox();
    return box ? adjustForAbsoluteZoom(box->contentHeight(), box) : 0;
}

int HTMLSnapshotImageElement::naturalWidth() const
{
    if (!m_image)
        return 0;

    return m_image->imageSize(1.0f).width();
}

int HTMLSnapshotImageElement::naturalHeight() const
{
    if (!m_image)
        return 0;

    return m_image->imageSize(1.0f).height();
}

bool HTMLSnapshotImageElement::isURLAttribute(Attribute* attr) const
{
    return attr->name() == srcAttr
        || attr->name() == lowsrcAttr
        || attr->name() == longdescAttr
        || (attr->name() == usemapAttr && attr->value().string()[0] != '#');
}

const AtomicString& HTMLSnapshotImageElement::alt() const
{
    return getAttribute(altAttr);
}

bool HTMLSnapshotImageElement::draggable() const
{
    // Image elements are draggable by default.
    return !equalIgnoringCase(getAttribute(draggableAttr), "false");
}

void HTMLSnapshotImageElement::setHeight(int value)
{
    setAttribute(heightAttr, String::number(value));
}

KURL HTMLSnapshotImageElement::src() const
{
    return document()->completeURL(getAttribute(srcAttr));
}

void HTMLSnapshotImageElement::setSrc(const String& value)
{
    setAttribute(srcAttr, value);
}

void HTMLSnapshotImageElement::setWidth(int value)
{
    setAttribute(widthAttr, String::number(value));
}

int HTMLSnapshotImageElement::x() const
{
    RenderObject* r = renderer();
    if (!r)
        return 0;

    // FIXME: This doesn't work correctly with transforms.
    FloatPoint absPos = r->localToAbsolute();
    return absPos.x();
}

int HTMLSnapshotImageElement::y() const
{
    RenderObject* r = renderer();
    if (!r)
        return 0;

    // FIXME: This doesn't work correctly with transforms.
    FloatPoint absPos = r->localToAbsolute();
    return absPos.y();
}

bool HTMLSnapshotImageElement::complete() const
{
    if (!m_image)
        return true;

    return m_image->status() > CachedResource::Pending;
}

void HTMLSnapshotImageElement::addSubresourceAttributeURLs(ListHashSet<KURL>& urls) const
{
    HTMLElement::addSubresourceAttributeURLs(urls);
}

void HTMLSnapshotImageElement::willMoveToNewOwnerDocument()
{
    HTMLElement::willMoveToNewOwnerDocument();
}

void HTMLSnapshotImageElement::notifyFinished(CachedResource* resource)
{
    if (!document()->attached())
        return;

    if (renderer() && renderer()->isImage()) {
        RenderImage* renderImage = toRenderImage(renderer());
        RenderImageResource* renderImageResource = renderImage->imageResource();

        renderImageResource->setCachedImage(m_image);
    }

    bool errorOccurred = resource->errorOccurred();

#if USE(JSC)
    if (!errorOccurred) {
        if (!inDocument()) {
            JSC::JSLock lock(JSC::SilenceAssertionsOnly);
            JSC::JSGlobalData* globalData = JSDOMWindowBase::commonJSGlobalData();
            globalData->heap.reportExtraMemoryCost(m_image->encodedSize());
        }
    }
#endif

    dispatchEvent(Event::create(errorOccurred ? eventNames().errorEvent : eventNames().loadEvent, false, false));
}

}
