/*
 * Copyright (C) 2011, Samsung Electronics
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ImageBackingStore_h
#define ImageBackingStore_h

#include "IntRect.h"
#include "IntSize.h"
#include <wtf/FastAllocBase.h>
#include <wtf/Noncopyable.h>
#include <wtf/PassOwnPtr.h>

namespace WebCore {

class ImageBackingStorePrivate;
class UIViewWindow;

class ImageBackingStore {
    WTF_MAKE_NONCOPYABLE(ImageBackingStore);
    WTF_MAKE_FAST_ALLOCATED;

public:
    static PassOwnPtr<ImageBackingStore> create(void* ptr, const IntSize&);

    ~ImageBackingStore();
    cairo_surface_t* cairoSurface();
    void scroll(const IntRect& scrollRect, const IntSize& scrollOffset);

private:
    ImageBackingStore(void* ptr, const IntSize&);

    OwnPtr<ImageBackingStorePrivate> m_private;
};

} // namespace WebCore

#endif // ImageBackingStore_h
