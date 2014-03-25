/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2011 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "BackingStore.h"

#include "ShareableBitmap.h"
#include "UpdateInfo.h"
#include "WebPageProxy.h"
#include <WebCore/GraphicsContext.h>
#include <cairo/cairo.h>
#include <gtk/gtk.h>

#if ENABLE(TVGG_BROWSER)
#if ENABLE(SEF)
#include "MMManager.h"
#endif
#include "WebPage.h"
#include "WebProcess.h"
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "CairoUtilities.h"
#include "GraphicsLayerRenderer.h"
#endif

using namespace WebCore;

namespace WebKit {

void BackingStore::paint(cairo_t* context, const IntRect& rect)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (!m_backingStore && !m_imageBackingStore)
        return;
    ASSERT(m_backingStore || m_imageBackingStore);
#else
    if (!m_backingStore)
    ASSERT(m_backingStore);
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (m_imageBackingStore) {
        cairo_set_operator(context, CAIRO_OPERATOR_CLEAR);
        cairo_rectangle(context, rect.x(), rect.y(), rect.width(), rect.height());
        cairo_fill(context);
    } else
#endif
    {
        cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
        cairo_set_source_surface(context, m_backingStore->cairoSurface(), 0, 0);
        cairo_rectangle(context, rect.x(), rect.y(), rect.width(), rect.height());
        cairo_fill(context);
    }

// Clear PIP area.
#if ENABLE(SEF)
    if (MMManager::getInstance()->isPipEnabled()) {
        IntRect pipRect = MMManager::getInstance()->getPipRect();
        pipRect.move(-MMManager::getInstance()->pipLocationOffset().x(), -MMManager::getInstance()->pipLocationOffset().y());
        cairo_set_operator(context, CAIRO_OPERATOR_CLEAR);
        cairo_rectangle(context, pipRect.x(), pipRect.y(), pipRect.width(), pipRect.height());
        cairo_fill(context);
    }
#endif
}

void BackingStore::incorporateUpdate(ShareableBitmap* bitmap, const UpdateInfo& updateInfo)
{
#if ENABLE(TVGG_BROWSER)
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (!m_backingStore && !m_imageBackingStore)
#else
    if (!m_backingStore)
#endif
        createBuffer();
#else
    if (!m_backingStore)
        m_backingStore = WidgetBackingStore::create(m_webPageProxy->viewWidget(), size());
#endif

    scroll(updateInfo.scrollRect, updateInfo.scrollOffset);

    // Paint all update rects.
    IntPoint updateRectLocation = updateInfo.updateRectBounds.location();
    RefPtr<cairo_t> context(adoptRef(cairo_create(m_backingStore->cairoSurface())));
    GraphicsContext graphicsContext(context.get());
    for (size_t i = 0; i < updateInfo.updateRects.size(); ++i) {
        IntRect updateRect = updateInfo.updateRects[i];
        IntRect srcRect = updateRect;
#if ENABLE(TVGG_BROWSER)
        if (m_webPageProxy->backgroundColor().hasAlpha())
            graphicsContext.clearRect(updateRect);
#endif
        srcRect.move(-updateRectLocation.x(), -updateRectLocation.y());
        bitmap->paint(graphicsContext, updateRect.location(), srcRect);
    }
}

void BackingStore::scroll(const IntRect& scrollRect, const IntSize& scrollOffset)
{
    if (scrollOffset.isZero())
        return;

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (m_imageBackingStore) {
        m_imageBackingStore->scroll(scrollRect, scrollOffset);
        return;
    }
#endif
    ASSERT(m_backingStore);
    m_backingStore->scroll(scrollRect, scrollOffset);
}

#if ENABLE(TVGG_BROWSER)
void BackingStore::createBuffer()
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (!m_backingStore && !m_imageBackingStore) {
#else
    if (!m_backingStore) {
#endif
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        WebPage* page = WebProcess::shared().mainUIWebPage();
        if (page && page->getRenderer()) {
            IntSize backingStoreSize;
            if (!page->getRenderer()->getBackingStoreBuffer(&backingStoreSize) || backingStoreSize != size())
                page->getRenderer()->initializeBackingStoreBuffer(size());

            void* ptr = page->getRenderer()->getBackingStoreBuffer(&backingStoreSize);
            ASSERT(ptr);
            if (!ptr) {
                printf("Cannot get buffer pointer. : %s %d\n", __func__, __LINE__);
                return;
            }

            m_imageBackingStore = WebCore::ImageBackingStore::create(ptr, size());
        } else
#endif
        m_backingStore = WebCore::WidgetBackingStore::create(m_webPageProxy->viewWidget(), size());

        RefPtr<cairo_t> context;
        if (m_backingStore)
            context = adoptRef(cairo_create(m_backingStore->cairoSurface()));
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        else if (m_imageBackingStore)
            context = adoptRef(cairo_create(m_imageBackingStore->cairoSurface()));
#endif
        GraphicsContext graphicsContext(context.get());

        const Color& backgroundColor = m_webPageProxy->backgroundColor();
        if (backgroundColor.alpha())
            graphicsContext.fillRect(FloatRect(0, 0, m_size.width(), m_size.height()), backgroundColor, ColorSpaceDeviceRGB);
        else
            graphicsContext.clearRect(FloatRect(0, 0, m_size.width(), m_size.height()));
    }
}
#endif

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
WebCore::ImageBackingStore* BackingStore::getImageBuffer()
{
    if (!m_imageBackingStore)
        createBuffer();
    return m_imageBackingStore.get();
}
#endif
WebCore::WidgetBackingStore* BackingStore::getBuffer()
{
    if (!m_backingStore)
        createBuffer();
    return m_backingStore.get();
}
#endif

} // namespace WebKit
