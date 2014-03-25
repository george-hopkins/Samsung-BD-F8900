/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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
#include "GraphicsContext3D.h"

#if ENABLE(WEBGL)
#include <wtf/PassOwnPtr.h>
#include "Extensions3DOpenGL.h"
#include "GraphicsContext3DInternal.h"

#if ENABLE(TVGG_WEBGL)
#include <GLES2/gl2.h>
#else
#include "OpenGLShims.h"
#include "ShaderLang.h"
#endif

#include <wtf/NotFound.h>

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "LayerWebGLContents.h"
#endif

namespace WebCore {

PassRefPtr<GraphicsContext3D> GraphicsContext3D::create(GraphicsContext3D::Attributes attributes, HostWindow* hostWindow, GraphicsContext3D::RenderStyle renderStyle)
{
#if ENABLE(TVGG_WEBGL)
    OwnPtr<GraphicsContext3DInternal> internal = GraphicsContext3DInternal::create(renderStyle, hostWindow);
#else
	OwnPtr<GraphicsContext3DInternal> internal = GraphicsContext3DInternal::create();
#endif
    if (!internal)
        return 0;

    RefPtr<GraphicsContext3D> context = adoptRef(new GraphicsContext3D(attributes, hostWindow, renderStyle == RenderOffscreen));

    context->m_internal = internal.release();

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    context->m_internal->setWebGLLayer(GtkLayer::create(0));
    if (renderStyle == RenderOffscreen)
        static_cast<GtkLayer*>(context->platformLayer())->setContentsToCanvas(context.get());
#endif

    return context.release();
}

GraphicsContext3D::GraphicsContext3D(GraphicsContext3D::Attributes attributes, HostWindow* hostWindow, bool bRenderOffScreen)
    : m_currentWidth(0)
    , m_currentHeight(0)
    , m_isResourceSafe(false)
    , m_attrs(attributes)
    , m_texture(0)
    , m_fbo(0)
    , m_depthStencilBuffer(0)
    , m_boundFBO(0)
    , m_multisampleFBO(0)
    , m_multisampleDepthStencilBuffer(0)
    , m_multisampleColorBuffer(0)
{
    GraphicsContext3DInternal::addActiveGraphicsContext(this);

    validateAttributes();

    if (!bRenderOffScreen)
        return;

    makeContextCurrent();

    // Create a texture to render into.
    ::glGenTextures(1, &m_texture);
    ::glBindTexture(GL_TEXTURE_2D, m_texture);
    ::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if ENABLE(TVGG_WEBGL)
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    glGenTextures(1, &m_compositorTexture);
    glBindTexture(GraphicsContext3D::TEXTURE_2D, m_compositorTexture);
    glTexParameterf(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MAG_FILTER, GraphicsContext3D::LINEAR);
    glTexParameterf(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MIN_FILTER, GraphicsContext3D::LINEAR);
    glTexParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_S, GraphicsContext3D::CLAMP_TO_EDGE);
    glTexParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_T, GraphicsContext3D::CLAMP_TO_EDGE);
#endif // #if ENABLE(TVGG_ACCELERATED_COMPOSITING)

    ::glBindTexture(GL_TEXTURE_2D, 0);
	WEBGL_DEBUG("m_texture(%d), m_compositorTexture(%d)", m_texture, m_compositorTexture);

    // Create an FBO.
#if ENABLE(TVGG_WEBGL)
    ::glGenFramebuffers(1, &m_fbo);
	::glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
#else
    ::glGenFramebuffersEXT(1, &m_fbo);
    ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
#endif
    m_boundFBO = m_fbo;

	if (!m_attrs.antialias && (m_attrs.stencil || m_attrs.depth))
#if ENABLE(TVGG_WEBGL)
		::glGenRenderbuffers(1, &m_depthStencilBuffer);
#else
		::glGenRenderbuffersEXT(1, &m_depthStencilBuffer);
#endif

	WEBGL_DEBUG("m_fbo(%d), m_depthStencilBuffer(%d)", m_fbo, m_depthStencilBuffer);

#if !ENABLE(TVGG_WEBGL)
    // Create a multisample FBO.
    if (m_attrs.antialias) {
        ::glGenFramebuffersEXT(1, &m_multisampleFBO);
        ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_multisampleFBO);

        m_boundFBO = m_multisampleFBO;
        ::glGenRenderbuffersEXT(1, &m_multisampleColorBuffer);
        if (m_attrs.stencil || m_attrs.depth)
            ::glGenRenderbuffersEXT(1, &m_multisampleDepthStencilBuffer);
    }

    // ANGLE initialization.
    ShBuiltInResources ANGLEResources;
    ShInitBuiltInResources(&ANGLEResources);

    getIntegerv(GraphicsContext3D::MAX_VERTEX_ATTRIBS, &ANGLEResources.MaxVertexAttribs);
    getIntegerv(GraphicsContext3D::MAX_VERTEX_UNIFORM_VECTORS, &ANGLEResources.MaxVertexUniformVectors);
    getIntegerv(GraphicsContext3D::MAX_VARYING_VECTORS, &ANGLEResources.MaxVaryingVectors);
    getIntegerv(GraphicsContext3D::MAX_VERTEX_TEXTURE_IMAGE_UNITS, &ANGLEResources.MaxVertexTextureImageUnits);
    getIntegerv(GraphicsContext3D::MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ANGLEResources.MaxCombinedTextureImageUnits);
    getIntegerv(GraphicsContext3D::MAX_TEXTURE_IMAGE_UNITS, &ANGLEResources.MaxTextureImageUnits);
    getIntegerv(GraphicsContext3D::MAX_FRAGMENT_UNIFORM_VECTORS, &ANGLEResources.MaxFragmentUniformVectors);

    // Always set to 1 for OpenGL ES.
    ANGLEResources.MaxDrawBuffers = 1;
    m_compiler.setResources(ANGLEResources);

    ::glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    ::glEnable(GL_POINT_SPRITE);
#endif
    ::glClearColor(0, 0, 0, 0);
}

GraphicsContext3D::~GraphicsContext3D()
{
    GraphicsContext3DInternal::removeActiveGraphicsContext(this);
    if (!m_internal->m_context)
        return;
	
	WEBGL_DEBUG("m_texture(%d), m_compositorTexture(%d)", m_texture, m_compositorTexture);
	WEBGL_DEBUG("m_fbo(%d), m_depthStencilBuffer(%d)", m_fbo, m_depthStencilBuffer);

    makeContextCurrent();
#if ENABLE(TVGG_WEBGL)
	if(m_texture)
#endif
	    ::glDeleteTextures(1, &m_texture);

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (m_compositorTexture)
        glDeleteTextures(1, &m_compositorTexture);
#endif // #if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#if ENABLE(TVGG_WEBGL)
	if (m_depthStencilBuffer)
		::glDeleteRenderbuffers(1, &m_depthStencilBuffer);
	if(m_fbo)
	    ::glDeleteFramebuffers(1, &m_fbo);
#else
    if (m_attrs.antialias) {
		::glDeleteRenderbuffersEXT(1, &m_multisampleColorBuffer);
        if (m_attrs.stencil || m_attrs.depth)
            ::glDeleteRenderbuffersEXT(1, &m_multisampleDepthStencilBuffer);
        ::glDeleteFramebuffersEXT(1, &m_multisampleFBO);
    } else {
        if (m_attrs.stencil || m_attrs.depth)
            ::glDeleteRenderbuffersEXT(1, &m_depthStencilBuffer);
	}
	::glDeleteFramebuffersEXT(1, &m_fbo);
#endif
}

void GraphicsContext3D::makeContextCurrent()
{
    if (!m_internal)
        return;
    m_internal->makeContextCurrent();
}

PlatformGraphicsContext3D GraphicsContext3D::platformGraphicsContext3D()
{
    return m_internal->m_context;
}

bool GraphicsContext3D::isGLES2Compliant() const
{
    return true;
}

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
void GraphicsContext3D::swapBuffers() const
{
    if (m_internal)
        m_internal->swapBuffers();
}
unsigned int GraphicsContext3D::frontBufferID()
{
    if (m_internal)
        return m_internal->frontBufferID();

    return 0;
}

unsigned int GraphicsContext3D::backBufferID()
{
    if (m_internal)
        return m_internal->backBufferID();

    return 0;
}

GtkLayer* GraphicsContext3D::platformLayer()
{
    if (m_internal)
        return m_internal->webGLLayer();

    return 0;
}

HostWindow* GraphicsContext3D::hostWindow()
{
    if (m_internal)
        return m_internal->hostWindow();

    return 0;
}

#endif

}

#endif // ENABLE(WEBGL)
