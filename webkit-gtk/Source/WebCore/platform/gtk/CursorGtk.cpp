/*
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
 * Copyright (C) 2007 Christian Dywan <christian@twotoasts.de>
 * Copyright (C) 2010 Igalia S.L.
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CursorGtk.h"
#include "GtkVersioning.h"

#include "Image.h"
#include "IntPoint.h"
#include "RefPtrCairo.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <wtf/Assertions.h>

#if ENABLE(TVGG_CUSTOM_CURSOR)
#include <string>
#endif

namespace WebCore {

#if ENABLE(TVGG_CUSTOM_CURSOR)
static bool isUseSmallCursor = false;
#endif

static GRefPtr<GdkCursor> createNamedCursor(CustomCursorType cursorType)
{
    CustomCursor cursor = CustomCursors[cursorType];
#if ENABLE(TVGG_CUSTOM_CURSOR)
    if (cursorType <= CustomCursorGrabbing) {
#endif // TVGG_CUSTOM_CURSOR
    GRefPtr<GdkCursor> c = adoptGRef(gdk_cursor_new_from_name(gdk_display_get_default(), cursor.name));
    if (c)
        return c;
#if ENABLE(TVGG_CUSTOM_CURSOR)
    }

    static const char* base = g_getenv("UI_URL");

    std::string imageDir;
    std::string imagePath;
    bool isHighResolution = false;

    const char* end;
    if (base && (end = strrchr(base, '/'))) {
        int baseLength = end - base + 1;
        imageDir.append(base, baseLength);
        if (imageDir.find("file://") == 0)
            imageDir.replace(0, strlen("file://"), "");

        if (gdk_screen_get_height(gdk_screen_get_default()) < 1080) {
            imageDir += "Themes/1280x720/Classic/Images/";
            isHighResolution = false;
        } else {
            imageDir += "Themes/1920x1080/Classic/Images/";
            isHighResolution = true;
        }
    }

    switch (cursorType) {
    case CustomCursorPointer:
        if (isUseSmallCursor)
            imagePath = imageDir + "mouse_03_small.png";
        else
            imagePath = imageDir + "mouse_03.png";

        cursor.hot_x  = 0;
        cursor.hot_y = 0;
        break;

    case CustomCursorHand:
        if (isUseSmallCursor) {
            imagePath = imageDir + "mouse_01_small.png";
            cursor.hot_x = 9;
            cursor.hot_y = 0;
        }
        else {
            imagePath = imageDir + "mouse_01.png";
            if (isHighResolution) {
                cursor.hot_x = 18;
                cursor.hot_y = 5;
            } else {
                cursor.hot_x = 12;
                cursor.hot_y = 3;
            }
        }
        break;

    case CustomCursorIBeam:
        if (isUseSmallCursor) {
            imagePath = imageDir + "mouse_02_small.png";
            cursor.hot_x = 9;
            cursor.hot_y = 12;
        }
        else {
            imagePath = imageDir + "mouse_02.png";
            if (isHighResolution) {
                cursor.hot_x = 25;
                cursor.hot_y = 35;
            } else {
                cursor.hot_x = 17;
                cursor.hot_y = 22;
            }
        }
        break;

    case CustomCursorMove:
        if (isUseSmallCursor)
            imagePath = imageDir + "mouse_05_small.png";
        else
            imagePath = imageDir + "mouse_05.png";

        cursor.hot_x = 0;
        cursor.hot_y = 0;
        break;

    default:
        return (GdkCursor*)NULL;
    }
                
    GError *err = NULL;
    GdkPixbuf* pixbuf_tmp = gdk_pixbuf_new_from_file(imagePath.data(), &err);

    if (!pixbuf_tmp) {
        printf("WebkitEngine >> Fail to load cursor image\n");
        if (err) {
            printf("WebkitEngine >> error string : %s\n", err->message);
            g_error_free(err);
            err = NULL;
        }
        switch (cursorType) {
        case CustomCursorPointer:
            return gdk_cursor_new(GDK_LEFT_PTR);
        case CustomCursorHand:
            return gdk_cursor_new(GDK_HAND2);
        case CustomCursorIBeam:
            return gdk_cursor_new(GDK_XTERM);
        case CustomCursorMove:
            return gdk_cursor_new(GDK_FLEUR);
        default:
            return (GdkCursor*)NULL;
        }
    }

    if (err)
        g_error_free(err);

    GdkCursor* c = gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pixbuf_tmp, cursor.hot_x, cursor.hot_y);
    if (!c)
        g_object_unref(pixbuf_tmp);

    return c;
#endif // TVGG_CUSTOM_CURSOR

    IntSize cursorSize = IntSize(32, 32);
    RefPtr<cairo_surface_t> source = adoptRef(cairo_image_surface_create_for_data(const_cast<unsigned char*>(cursor.bits), CAIRO_FORMAT_A1, 32, 32, 4));
    RefPtr<cairo_surface_t> mask = adoptRef(cairo_image_surface_create_for_data(const_cast<unsigned char*>(cursor.mask_bits), CAIRO_FORMAT_A1, 32, 32, 4));
    RefPtr<cairo_surface_t> surface = adoptRef(cairo_image_surface_create(CAIRO_FORMAT_A1, 32, 32));
    RefPtr<cairo_t> cr = adoptRef(cairo_create(surface.get()));

    cairo_set_source_surface(cr.get(), source.get(), cursor.hot_x, cursor.hot_y);
    cairo_mask_surface(cr.get(), mask.get(), cursor.hot_x, cursor.hot_y);

    GRefPtr<GdkPixbuf> pixbuf = adoptGRef(gdk_pixbuf_get_from_surface(surface.get(), 0, 0, 32, 32));
    return adoptGRef(gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pixbuf.get(), 0, 0));
}

static GRefPtr<GdkCursor> createCustomCursor(Image* image, const IntPoint& hotSpot)
{
    IntPoint effectiveHotSpot = determineHotSpot(image, hotSpot);
    GRefPtr<GdkPixbuf> pixbuf = adoptGRef(image->getGdkPixbuf());
    return adoptGRef(gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pixbuf.get(), effectiveHotSpot.x(), effectiveHotSpot.y()));
}

void Cursor::ensurePlatformCursor() const
{
#if ENABLE(TVGG_CUSTOM_CURSOR)
    if (m_platformCursor)
#else
    if (m_platformCursor || m_type == Cursor::Pointer)
#endif
        return;

    switch (m_type) {
    case Cursor::Pointer:
        // A null GdkCursor is the default cursor for the window.
#if ENABLE(TVGG_CUSTOM_CURSOR)
        m_platformCursor = createNamedCursor(CustomCursorPointer);
#else // TVGG_CUSTOM_CURSOR
        m_platformCursor = 0;
#endif // TVGG_CUSTOM_CURSOR
        break;
    case Cursor::Cross:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_CROSS));
        break;
    case Cursor::Hand:
#if ENABLE(TVGG_CUSTOM_CURSOR)
        m_platformCursor = createNamedCursor(CustomCursorHand);
#else // TVGG_CUSTOM_CURSOR
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_HAND2));
#endif // TVGG_CUSTOM_CURSOR
        break;
    case Cursor::IBeam:
#if ENABLE(TVGG_CUSTOM_CURSOR)
        m_platformCursor = createNamedCursor(CustomCursorIBeam);
#else // TVGG_CUSTOM_CURSOR
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_XTERM));
#endif // TVGG_CUSTOM_CURSOR
        break;
    case Cursor::Wait:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_WATCH));
        break;
    case Cursor::Help:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_QUESTION_ARROW));
        break;
    case Cursor::Move:
    case Cursor::MiddlePanning:
#if ENABLE(TVGG_CUSTOM_CURSOR)
        m_platformCursor = createNamedCursor(CustomCursorMove);
#else
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_FLEUR));
#endif
        break;
    case Cursor::EastResize:
    case Cursor::EastPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_RIGHT_SIDE));
        break;
    case Cursor::NorthResize:
    case Cursor::NorthPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_TOP_SIDE));
        break;
    case Cursor::NorthEastResize:
    case Cursor::NorthEastPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_LEFT_SIDE));
        break;
    case Cursor::NorthWestResize:
    case Cursor::NorthWestPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_TOP_LEFT_CORNER));
        break;
    case Cursor::SouthResize:
    case Cursor::SouthPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_BOTTOM_SIDE));
        break;
    case Cursor::SouthEastResize:
    case Cursor::SouthEastPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_BOTTOM_RIGHT_CORNER));
        break;
    case Cursor::SouthWestResize:
    case Cursor::SouthWestPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_BOTTOM_LEFT_CORNER));
        break;
    case Cursor::WestResize:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_LEFT_SIDE));
        break;
    case Cursor::NorthSouthResize:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_TOP_TEE));
        break;
    case Cursor::EastWestResize:
    case Cursor::WestPanning:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_LEFT_SIDE));
        break;
    case Cursor::NorthEastSouthWestResize:
    case Cursor::NorthWestSouthEastResize:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_SIZING));
        break;
    case Cursor::ColumnResize:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_SB_H_DOUBLE_ARROW));
        break;
    case Cursor::RowResize:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_SB_V_DOUBLE_ARROW));
        break;
    case Cursor::VerticalText:
        m_platformCursor = createNamedCursor(CustomCursorVerticalText);
        break;
    case Cursor::Cell:
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_PLUS));
        break;
    case Cursor::ContextMenu:
        m_platformCursor = createNamedCursor(CustomCursorContextMenu);
        break;
    case Cursor::Alias:
        m_platformCursor = createNamedCursor(CustomCursorAlias);
        break;
    case Cursor::Progress:
        m_platformCursor = createNamedCursor(CustomCursorProgress);
        break;
    case Cursor::NoDrop:
    case Cursor::NotAllowed:
        m_platformCursor = createNamedCursor(CustomCursorNoDrop);
        break;
    case Cursor::Copy:
        m_platformCursor = createNamedCursor(CustomCursorCopy);
        break;
    case Cursor::None:
#if ENABLE(TVGG_USE_BLANK_CURSOR_TO_HIDE_CURSOR)
        m_platformCursor = adoptGRef(gdk_cursor_new(GDK_BLANK_CURSOR));
#else // TVGG_USE_BLANK_CURSOR_TO_HIDE_CURSOR
        m_platformCursor = createNamedCursor(CustomCursorNone);
#endif //TVGG_USE_BLANK_CURSOR_TO_HIDE_CURSOR
        break;
    case Cursor::ZoomIn:
        m_platformCursor = createNamedCursor(CustomCursorZoomIn);
        break;
    case Cursor::ZoomOut:
        m_platformCursor = createNamedCursor(CustomCursorZoomOut);
        break;
    case Cursor::Grab:
        m_platformCursor = createNamedCursor(CustomCursorGrab);
        break;
    case Cursor::Grabbing:
        m_platformCursor = createNamedCursor(CustomCursorGrabbing);
        break;
    case Cursor::Custom:
        m_platformCursor = createCustomCursor(m_image.get(), m_hotSpot);
        break;
    }
}

Cursor::Cursor(const Cursor& other)
    : m_type(other.m_type)
    , m_image(other.m_image)
    , m_hotSpot(other.m_hotSpot)
    , m_platformCursor(other.m_platformCursor)
{
}

Cursor& Cursor::operator=(const Cursor& other)
{
    m_type = other.m_type;
    m_image = other.m_image;
    m_hotSpot = other.m_hotSpot;
    m_platformCursor = other.m_platformCursor;
    return *this;
}

Cursor::~Cursor()
{
}

#if ENABLE(TVGG_CUSTOM_CURSOR)
void Cursor::clearCursor(Cursor::Type type)
{
    fromType(type).m_platformCursor.clear();
    fromType(type).m_platformCursor = 0;
}

void Cursor::setCursorSize(bool isSmallCursor)
{
    isUseSmallCursor = isSmallCursor;
}

bool Cursor::isSmallCursor()
{
    return isUseSmallCursor;
}
#endif

#if ENABLE(TVGG_BROWSER)
#include "BELog.h"
const Cursor& Cursor::fromPlatformCursorType(int gdkCursorType)
{
    Cursor::Type cursorType = Cursor::Pointer;
    switch (gdkCursorType) {
    case GDK_LEFT_PTR:
        cursorType = Cursor::Pointer;
        break;
    case GDK_HAND2:
        cursorType = Cursor::Hand;
        break;
    case GDK_XTERM:
        cursorType = Cursor::IBeam;
        break;
    case 1000:
    case 1001:
        BELog("gdkCursorType : %d\n", gdkCursorType);
    default:
        cursorType = Cursor::Pointer;
    }
    return fromType(cursorType);
}
#endif
}
