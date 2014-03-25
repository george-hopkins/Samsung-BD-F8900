#ifndef HTMLSnapshotImageElement_h
#define HTMLSnapshotImageElement_h

#include "GraphicsTypes.h"
#include "HTMLElement.h"
#include "HTMLImageLoader.h"

namespace WebCore {

class HTMLSnapshotImageElement : public HTMLElement, public CachedResourceClient {
public:
    static PassRefPtr<HTMLSnapshotImageElement> create(const QualifiedName&, Document*);

    virtual ~HTMLSnapshotImageElement();

    int width(bool ignorePendingStylesheets = false) const;
    int height(bool ignorePendingStylesheets = false) const;

    int naturalWidth() const;
    int naturalHeight() const;

    bool isServerMap() const { return ismap && usemap.isEmpty(); }

    String altText() const;

    CompositeOperator compositeOperator() const { return m_compositeOperator; }

    const AtomicString& alt() const;

    void setImage(CachedImage* image) { m_image = image; }

    void setHeight(int);

    KURL src() const;
    void setSrc(const String&);

    void setWidth(int);

    int x() const;
    int y() const;

    bool complete() const;

    virtual bool canContainRangeEndPoint() const { return false; }

protected:
    HTMLSnapshotImageElement(const QualifiedName&, Document*);

    virtual void willMoveToNewOwnerDocument();

private:
    virtual bool mapToEntry(const QualifiedName& attrName, MappedAttributeEntry& result) const;
    virtual void parseMappedAttribute(Attribute*);

    virtual void attach();
    virtual RenderObject* createRenderer(RenderArena*, RenderStyle*);

    virtual bool canStartSelection() const { return false; }

    virtual bool isURLAttribute(Attribute*) const;

    virtual bool draggable() const;

    virtual void addSubresourceAttributeURLs(ListHashSet<KURL>&) const;

    virtual void insertedIntoDocument();
    virtual void removedFromDocument();
    virtual void insertedIntoTree(bool deep);
    virtual void removedFromTree(bool deep);

    virtual void notifyFinished(CachedResource*);

    CachedImage* m_image;
    String usemap;
    bool ismap;
    AtomicString m_name;
    AtomicString m_id;
    CompositeOperator m_compositeOperator;
};

} //namespace

#endif
