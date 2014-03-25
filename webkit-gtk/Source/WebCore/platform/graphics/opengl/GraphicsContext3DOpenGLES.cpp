/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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

#if ENABLE(WEBGL)

#include "GraphicsContext3D.h"

#include "ArrayBuffer.h"
#include "ArrayBufferView.h"
#include "WebGLObject.h"
#include "CanvasRenderingContext.h"
#include "Extensions3DOpenGL.h"
#include "Float32Array.h"
#include "GraphicsContext.h"
#include "HTMLCanvasElement.h"
#include "ImageBuffer.h"
#include "ImageData.h"
#include "Int32Array.h"
#include "NotImplemented.h"
#include "Uint8Array.h"
#include <cstring>
#include <wtf/UnusedParam.h>
#include <wtf/text/CString.h>

#include <GLES2/gl2.h>

#if PLATFORM(MAC)
#include <OpenGL/gl.h>
#endif

#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
#include <sys/types.h>
#include <signal.h>
#endif

#include "BELog.h"

//#undef  BELog
//#define BELog(...) ((void)0)

//#define TVGG_PRINT_GLES_ERROR

#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
static void exitWebBrowser()
{
    fprintf(stderr, "Exit Browser Process => Out of mali internal memory\n");
    pid_t pid = getpid();
    ::kill(pid, SIGKILL);
}

static void checkGLOOMandExit()
{
    if (glGetError() == GL_OUT_OF_MEMORY)
        exitWebBrowser();
}
#endif

namespace WebCore {

void GraphicsContext3D::validateAttributes()
{
    WEBGL_DEBUG("");

    Extensions3D* extensions = getExtensions();
    if (m_attrs.stencil) {
        if (extensions->supports("GL_EXT_packed_depth_stencil")) {
            extensions->ensureEnabled("GL_EXT_packed_depth_stencil");
            // Force depth if stencil is true.
            m_attrs.depth = true;
        } else
            m_attrs.stencil = false;
    }
    if (m_attrs.antialias) {
        bool isValidVendor = true;
        // Currently in Mac we only turn on antialias if vendor is NVIDIA.
        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        if (!std::strstr(vendor, "NVIDIA"))
            isValidVendor = false;
        if (!isValidVendor || !extensions->supports("GL_ANGLE_framebuffer_multisample"))
            m_attrs.antialias = false;
        else
            extensions->ensureEnabled("GL_ANGLE_framebuffer_multisample");
    }
}

void GraphicsContext3D::readRenderingResults(unsigned char *pixels, int pixelsSize)
{
    if (pixelsSize < m_currentWidth * m_currentHeight * 4)
        return;

    makeContextCurrent();

    bool mustRestoreFBO = false;
    if (m_attrs.antialias) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        mustRestoreFBO = true;
    } else {
        if (m_boundFBO != m_fbo) {
            mustRestoreFBO = true;
            glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        }
    }

    int packAlignment = 4;
    bool mustRestorePackAlignment = false;
    glGetIntegerv(GL_PACK_ALIGNMENT, &packAlignment);
    if (packAlignment > 4) {
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        mustRestorePackAlignment = true;
    }

    glReadPixels(0, 0, m_currentWidth, m_currentHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    if (mustRestorePackAlignment)
        glPixelStorei(GL_PACK_ALIGNMENT, packAlignment);

    if (mustRestoreFBO)
        glBindFramebuffer(GL_FRAMEBUFFER, m_boundFBO);
}

void GraphicsContext3D::paintRenderingResultsToCanvas(CanvasRenderingContext* context)
{
    WEBGL_DEBUG("");

    HTMLCanvasElement* canvas = context->canvas();
    ImageBuffer* imageBuffer = canvas->buffer();

    int rowBytes = m_currentWidth * 4;
    int totalBytes = rowBytes * m_currentHeight;

    OwnArrayPtr<unsigned char> pixels = adoptArrayPtr(new unsigned char[totalBytes]);
    if (!pixels)
        return;

    readRenderingResults(pixels.get(), totalBytes);

    if (!m_attrs.premultipliedAlpha) {
        for (int i = 0; i < totalBytes; i += 4) {
            // Premultiply alpha
            pixels[i + 0] = std::min(255, pixels[i + 0] * pixels[i + 3] / 255);
            pixels[i + 1] = std::min(255, pixels[i + 1] * pixels[i + 3] / 255);
            pixels[i + 2] = std::min(255, pixels[i + 2] * pixels[i + 3] / 255);
        }
    }

    // Convert to BGRA
    for (int i = 0; i < totalBytes; i += 4)
        std::swap(pixels[i], pixels[i + 2]);

    paintToCanvas(pixels.get(), m_currentWidth, m_currentHeight,
                  canvas->width(), canvas->height(), imageBuffer->context()->platformContext());
}

PassRefPtr<ImageData> GraphicsContext3D::paintRenderingResultsToImageData()
{
    WEBGL_DEBUG("");

    // Reading premultiplied alpha would involve unpremultiplying, which is
    // lossy
    if (m_attrs.premultipliedAlpha)
        return 0;

    RefPtr<ImageData> imageData = ImageData::create(IntSize(m_currentWidth, m_currentHeight));
    unsigned char* pixels = imageData->data()->data()->data();
    int totalBytes = 4 * m_currentWidth * m_currentHeight;

    readRenderingResults(pixels, totalBytes);

 #if !ENABLE(TVGG_BROWSER)
   // Convert to RGBA
    for (int i = 0; i < totalBytes; i += 4)
        std::swap(pixels[i], pixels[i + 2]);
#endif

    return imageData.release();
}

void GraphicsContext3D::reshape(int width, int height)
{
    if (!platformGraphicsContext3D())
        return;

    if (width == m_currentWidth && height == m_currentHeight)
        return;
    
    m_currentWidth = width;
    m_currentHeight = height;
    
    makeContextCurrent();

    validateAttributes();
    WEBGL_DEBUG("antialias: %d, stencil: %d, depth: %d, alpha: %d", m_attrs.antialias, m_attrs.stencil, m_attrs.depth, m_attrs.alpha);
   
    GLuint colorFormat, internalDepthStencilFormat = 0;
    if (m_attrs.alpha) {
        m_internalColorFormat = GL_RGBA;
        colorFormat = GL_RGBA;
    } else {
        m_internalColorFormat = GL_RGB;
        colorFormat = GL_RGB;
    }
    if (m_attrs.stencil || m_attrs.depth) {
        // We don't allow the logic where stencil is required and depth is not.
        // See GraphicsContext3D::validateAttributes.

        Extensions3D* extensions = getExtensions();
        // Use a 24 bit depth buffer where we know we have it
        if (extensions->supports("GL_EXT_packed_depth_stencil"))
            internalDepthStencilFormat = GL_STENCIL_INDEX8;
        else
            internalDepthStencilFormat = GL_DEPTH_COMPONENT16;
    }

    bool mustRestoreFBO = false;

    // resize regular FBO
    if (m_boundFBO != m_fbo) {
        mustRestoreFBO = true;
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }
    glBindTexture(GL_TEXTURE_2D, m_texture);
    BELog("Before glTexImage2D(%dx%d, %d) GLError = %d\n", width, height, colorFormat, glGetError());
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalColorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, 0);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
    glBindTexture(GL_TEXTURE_2D, m_compositorTexture);
    BELog("Before glTexImage2D(%dx%d, %d) GLError = %d\n", width, height, colorFormat, glGetError());
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalColorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, 0);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    if (m_attrs.stencil || m_attrs.depth) {
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
        BELog("Before glRenderbufferStorage(%dx%d, %d) GLError = %d\n", width, height, internalDepthStencilFormat, glGetError());
        glRenderbufferStorage(GL_RENDERBUFFER, internalDepthStencilFormat, width, height);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
        checkGLOOMandExit();
#endif
        if (m_attrs.stencil)
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
        if (m_attrs.depth)
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
#ifdef TVGG_PRINT_GLES_ERROR
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            BELog("FrameBuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            BELog("FrameBuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            BELog("FrameBuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            BELog("FrameBuffer incomplete: GL_FRAMEBUFFER_UNSUPPORTED\n");
            break;
        case GL_FRAMEBUFFER_COMPLETE:
            BELog("FrameBuffer Complete: GL_FRAMEBUFFER_COMPLETE\n");
            break;
        default:
            BELog("FrameBuffer incomplete: Unknown status\n");
            break;
    }
#else
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // FIXME: cleanup
        notImplemented();
    }
#endif
    // Initialize renderbuffers to 0.
    GLfloat clearColor[] = {0, 0, 0, 0}, clearDepth = 0;
    int clearStencil = 0;
    GLboolean colorMask[] = {GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE}, depthMask = GL_TRUE;
    GLuint stencilMask = 0xffffffff;
    GLboolean isScissorEnabled = GL_FALSE;
    GLboolean isDitherEnabled = GL_FALSE;
    GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    glClearColor(0, 0, 0, 0);
    glGetBooleanv(GL_COLOR_WRITEMASK, colorMask);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    if (m_attrs.depth) {
        glGetFloatv(GL_DEPTH_CLEAR_VALUE, &clearDepth);
        glClearDepthf(1);
        glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
        glDepthMask(GL_TRUE);
        clearMask |= GL_DEPTH_BUFFER_BIT;
    }
    if (m_attrs.stencil) {
        glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &clearStencil);
        glClearStencil(0);
        glGetIntegerv(GL_STENCIL_WRITEMASK, reinterpret_cast<int*>(&stencilMask));
        glStencilMaskSeparate(GL_FRONT, 0xffffffff);
        clearMask |= GL_STENCIL_BUFFER_BIT;
    }
    isScissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
    glDisable(GL_SCISSOR_TEST);
    isDitherEnabled = glIsEnabled(GL_DITHER);
    glDisable(GL_DITHER);

    glClear(clearMask);

    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
    if (m_attrs.depth) {
        glClearDepthf(clearDepth);
        glDepthMask(depthMask);
    }
    if (m_attrs.stencil) {
        glClearStencil(clearStencil);
        glStencilMaskSeparate(GL_FRONT, stencilMask);
    }
    if (isScissorEnabled)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
    if (isDitherEnabled)
        glEnable(GL_DITHER);
    else
        glDisable(GL_DITHER);

    if (mustRestoreFBO)
        glBindFramebuffer(GL_FRAMEBUFFER, m_boundFBO);

    glFlush();
}

IntSize GraphicsContext3D::getInternalFramebufferSize() const
{
    return IntSize(m_currentWidth, m_currentHeight);
}

void GraphicsContext3D::prepareTexture()
{
    WEBGL_DEBUG("");

    if (m_layerComposited)
        return;
    makeContextCurrent();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, m_compositorTexture);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, m_internalColorFormat, 0, 0, m_currentWidth, m_currentHeight, 0);
    glBindTexture(GL_TEXTURE_2D, m_boundTexture0);

    glActiveTexture(m_activeTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, m_boundFBO);
    glFinish();
    m_layerComposited = true;
}

void GraphicsContext3D::activeTexture(GC3Denum texture)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    m_activeTexture = texture;
    glActiveTexture(texture);
}

void GraphicsContext3D::attachShader(Platform3DObject program, Platform3DObject shader)
{
    ASSERT(program);
    ASSERT(shader);
	
    WEBGL_DEBUG("");
    makeContextCurrent();

    glAttachShader(program, shader);
}

void GraphicsContext3D::bindAttribLocation(Platform3DObject program, GC3Duint index, const String& name)
{
    ASSERT(program);
	
    WEBGL_DEBUG("");
    makeContextCurrent();

    glBindAttribLocation(program, index, name.utf8().data());
}

void GraphicsContext3D::bindBuffer(GC3Denum target, Platform3DObject buffer)
{
    WEBGL_DEBUG("buffer(%d)", buffer);
    makeContextCurrent();

    glBindBuffer(target, buffer);
}

void GraphicsContext3D::bindFramebuffer(GC3Denum target, Platform3DObject framebuffer)
{
    WEBGL_DEBUG("framebuffer(%d)", framebuffer);
    makeContextCurrent();

	/* @20110722-soon : temp */
	if(framebuffer == 0)
		return;
	
    glBindFramebuffer(target, framebuffer);
}

void GraphicsContext3D::bindRenderbuffer(GC3Denum target, Platform3DObject renderbuffer)
{
    WEBGL_DEBUG("renderbuffer(%d)", renderbuffer);
    makeContextCurrent();

    glBindRenderbuffer(target, renderbuffer);
}

void GraphicsContext3D::bindTexture(GC3Denum target, Platform3DObject texture)
{
    WEBGL_DEBUG("texture(%d)", texture);
    makeContextCurrent();

    if (m_activeTexture && target == GL_TEXTURE_2D)
        m_boundTexture0 = texture;
    glBindTexture(target, texture);
}

void GraphicsContext3D::blendColor(GC3Dclampf red, GC3Dclampf green, GC3Dclampf blue, GC3Dclampf alpha)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glBlendColor(red, green, blue, alpha);
}

void GraphicsContext3D::blendEquation(GC3Denum mode)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glBlendEquation(mode);
}

void GraphicsContext3D::blendEquationSeparate(GC3Denum modeRGB, GC3Denum modeAlpha)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glBlendEquationSeparate(modeRGB, modeAlpha);
}


void GraphicsContext3D::blendFunc(GC3Denum sfactor, GC3Denum dfactor)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glBlendFunc(sfactor, dfactor);
}       

void GraphicsContext3D::blendFuncSeparate(GC3Denum srcRGB, GC3Denum dstRGB, GC3Denum srcAlpha, GC3Denum dstAlpha)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void GraphicsContext3D::bufferData(GC3Denum target, GC3Dsizeiptr size, GC3Denum usage)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    if (!size)
        size = 1;

    BELog("Before glBufferData(size=%d) GLError=%d\n", size, glGetError());
	glBufferData(target, size, 0, usage);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
}

void GraphicsContext3D::bufferData(GC3Denum target, GC3Dsizeiptr size, const void* data, GC3Denum usage)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    if (!size)
        size = 1;

    BELog("Before glBufferData(size = %d, %d) GLError=%d\n", size, glGetError());
    glBufferData(target, size, data, usage);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
}

void GraphicsContext3D::bufferSubData(GC3Denum target, GC3Dintptr offset, GC3Dsizeiptr size, const void* data)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    //BELog("Before glBufferData(size = %d, %d) GLError = %d\n", size, glGetError());
    glBufferSubData(target, offset, size, data);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
}

GC3Denum GraphicsContext3D::checkFramebufferStatus(GC3Denum target)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    return glCheckFramebufferStatus(target);
}

void GraphicsContext3D::clearColor(GC3Dclampf r, GC3Dclampf g, GC3Dclampf b, GC3Dclampf a)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glClearColor(r, g, b, a);
}

void GraphicsContext3D::clear(GC3Dbitfield mask)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glClear(mask);
}

void GraphicsContext3D::clearDepth(GC3Dclampf depth)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glClearDepthf(depth);
}

void GraphicsContext3D::clearStencil(GC3Dint s)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glClearStencil(s);
}

void GraphicsContext3D::colorMask(GC3Dboolean red, GC3Dboolean green, GC3Dboolean blue, GC3Dboolean alpha)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glColorMask(red, green, blue, alpha);
}

void GraphicsContext3D::compileShader(Platform3DObject shader)
{
    WEBGL_DEBUG("shader(%d)", shader);
    makeContextCurrent();

    glCompileShader(shader);

#ifdef TVGG_PRINT_GLES_ERROR
    GLint compiled;
    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            BELog("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
    }
#endif
}

void GraphicsContext3D::copyTexImage2D(GC3Denum target, GC3Dint level, GC3Denum internalformat, GC3Dint x, GC3Dint y, GC3Dsizei width, GC3Dsizei height, GC3Dint border)
{
    WEBGL_DEBUG("");
    makeContextCurrent();
	
    glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void GraphicsContext3D::copyTexSubImage2D(GC3Denum target, GC3Dint level, GC3Dint xoffset, GC3Dint yoffset, GC3Dint x, GC3Dint y, GC3Dsizei width, GC3Dsizei height)
{
    WEBGL_DEBUG("");
    makeContextCurrent();
	
    glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void GraphicsContext3D::cullFace(GC3Denum mode)
{
    WEBGL_DEBUG("");
    makeContextCurrent();
	
    glCullFace(mode);
}

void GraphicsContext3D::depthFunc(GC3Denum func)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glDepthFunc(func);
}

void GraphicsContext3D::depthMask(GC3Dboolean flag)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glDepthMask(flag);
}

void GraphicsContext3D::depthRange(GC3Dclampf zNear, GC3Dclampf zFar)
{
    WEBGL_DEBUG("");
	makeContextCurrent();
	
    glDepthRangef(zNear, zFar);
}

void GraphicsContext3D::detachShader(Platform3DObject program, Platform3DObject shader)
{
    ASSERT(program);
    ASSERT(shader);
    WEBGL_DEBUG("");
	makeContextCurrent();
	
    glDetachShader(program, shader);
}

void GraphicsContext3D::disable(GC3Denum cap)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glDisable(cap);
}

void GraphicsContext3D::disableVertexAttribArray(GC3Duint index)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glDisableVertexAttribArray(index);
}

void GraphicsContext3D::drawArrays(GC3Denum mode, GC3Dint first, GC3Dsizei count)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glDrawArrays(mode, first, count);
}

void GraphicsContext3D::drawElements(GC3Denum mode, GC3Dsizei count, GC3Denum type, GC3Dintptr offset)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glDrawElements(mode, count, type, reinterpret_cast<GLvoid*>(static_cast<intptr_t>(offset)));
}

void GraphicsContext3D::enable(GC3Denum cap)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glEnable(cap);
}

void GraphicsContext3D::enableVertexAttribArray(GC3Duint index)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glEnableVertexAttribArray(index);
}

void GraphicsContext3D::finish()
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glFinish();
}

void GraphicsContext3D::flush()
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glFlush();
}

void GraphicsContext3D::framebufferRenderbuffer(GC3Denum target, GC3Denum attachment, GC3Denum renderbuffertarget, Platform3DObject buffer)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, buffer);
}

void GraphicsContext3D::framebufferTexture2D(GC3Denum target, GC3Denum attachment, GC3Denum textarget, Platform3DObject texture, GC3Dint level)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void GraphicsContext3D::frontFace(GC3Denum mode)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glFrontFace(mode);
}

void GraphicsContext3D::generateMipmap(GC3Denum target)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGenerateMipmap(target);
}

bool GraphicsContext3D::getActiveAttrib(Platform3DObject program, GC3Duint index, ActiveInfo& info)
{
    if (!program) {
        synthesizeGLError(INVALID_VALUE);
        return false;
    }
	
    WEBGL_DEBUG("");
    makeContextCurrent();

    int maxAttributeSize = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeSize);
	
    char name[maxAttributeSize]; // GL_ACTIVE_ATTRIBUTE_MAX_LENGTH includes null termination
    GLsizei nameLength = 0;
    int size = 0;
    GLenum type = 0;
	
    glGetActiveAttrib(program, index, maxAttributeSize, &nameLength, &size, &type, name);
    if (!nameLength)
        return false;
	
    info.name = String(name, nameLength);
    info.type = type;
    info.size = size;
    return true;
}
    
bool GraphicsContext3D::getActiveUniform(Platform3DObject program, GC3Duint index, ActiveInfo& info)
{
    if (!program) {
        synthesizeGLError(INVALID_VALUE);
        return false;
    }
	
    WEBGL_DEBUG("");
    makeContextCurrent();

    int maxUniformSize = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformSize);
	
    char name[maxUniformSize]; // GL_ACTIVE_UNIFORM_MAX_LENGTH includes null termination
    GLsizei nameLength = 0;
    int size = 0;
    GLenum type = 0;
	
    glGetActiveUniform(program, index, maxUniformSize, &nameLength, &size, &type, name);
    if (!nameLength)
        return false;
	
    info.name = String(name, nameLength);
    info.type = type;
    info.size = size;
    return true;
}

void GraphicsContext3D::getAttachedShaders(Platform3DObject program, GC3Dsizei maxCount, GC3Dsizei* count, Platform3DObject* shaders)
{
    if (!program) {
        synthesizeGLError(INVALID_VALUE);
        return;
    }
	
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetAttachedShaders(program, maxCount, count, shaders);
}

int GraphicsContext3D::getAttribLocation(Platform3DObject program, const String& name)
{
    if (!program)
        return -1;

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glGetAttribLocation(program, name.utf8().data());
}

GraphicsContext3D::Attributes GraphicsContext3D::getContextAttributes()
{
    return m_attrs;
}

GC3Denum GraphicsContext3D::getError()
{
    if (m_syntheticErrors.size() > 0) {
        ListHashSet<GC3Denum>::iterator iter = m_syntheticErrors.begin();
        GC3Denum err = *iter;
        m_syntheticErrors.remove(iter);
        return err;
    }

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glGetError();
}

String GraphicsContext3D::getString(GC3Denum name)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    return String((const char*) glGetString(name));
}

void GraphicsContext3D::hint(GC3Denum target, GC3Denum mode)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glHint(target, mode);
}

GC3Dboolean GraphicsContext3D::isBuffer(Platform3DObject buffer)
{
    if (!buffer)
        return GL_FALSE;

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glIsBuffer(buffer);
}

GC3Dboolean GraphicsContext3D::isEnabled(GC3Denum cap)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    return glIsEnabled(cap);
}

GC3Dboolean GraphicsContext3D::isFramebuffer(Platform3DObject framebuffer)
{
    if (!framebuffer)
        return GL_FALSE;

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glIsFramebuffer(framebuffer);
}

GC3Dboolean GraphicsContext3D::isProgram(Platform3DObject program)
{
    if (!program)
        return GL_FALSE;

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glIsProgram(program);
}

GC3Dboolean GraphicsContext3D::isRenderbuffer(Platform3DObject renderbuffer)
{
    if (!renderbuffer)
        return GL_FALSE;

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glIsRenderbuffer(renderbuffer);
}

GC3Dboolean GraphicsContext3D::isShader(Platform3DObject shader)
{
    if (!shader)
        return GL_FALSE;

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glIsShader(shader);
}

GC3Dboolean GraphicsContext3D::isTexture(Platform3DObject texture)
{
    if (!texture)
        return GL_FALSE;

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glIsTexture(texture);
}

void GraphicsContext3D::lineWidth(GC3Dfloat width)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glLineWidth(width);
}

void GraphicsContext3D::linkProgram(Platform3DObject program)
{
    ASSERT(program);
    WEBGL_DEBUG("");
    makeContextCurrent();

    glLinkProgram(program);

#ifdef TVGG_PRINT_GLES_ERROR
    GLint linked;
    // Check the compile status
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            BELog("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
    }
#endif
}

void GraphicsContext3D::pixelStorei(GC3Denum pname, GC3Dint param)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glPixelStorei(pname, param);
}

void GraphicsContext3D::polygonOffset(GC3Dfloat factor, GC3Dfloat units)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glPolygonOffset(factor, units);
}

void GraphicsContext3D::readPixels(GC3Dint x, GC3Dint y, GC3Dsizei width, GC3Dsizei height, GC3Denum format, GC3Denum type, void* data)
{
    // FIXME: remove the two glFlush calls when the driver bug is fixed, i.e.,
    // all previous rendering calls should be done before reading pixels.
    WEBGL_DEBUG("");
    makeContextCurrent();

    glFlush();
    glReadPixels(x, y, width, height, format, type, data);
}

void GraphicsContext3D::releaseShaderCompiler()
{
    // FIXME: This is not implemented on desktop OpenGL. We need to have ifdefs for the different GL variants
    WEBGL_DEBUG("not implemented");
    makeContextCurrent();

    //glReleaseShaderCompiler();
}

void GraphicsContext3D::renderbufferStorage(GC3Denum target, GC3Denum internalformat, GC3Dsizei width, GC3Dsizei height)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    switch (internalformat) {
    case DEPTH_STENCIL:
        internalformat = GL_STENCIL_INDEX8;
        break;
    case DEPTH_COMPONENT16:
        internalformat = GL_DEPTH_COMPONENT16;
        break;
    case RGBA4:
        internalformat = GL_RGBA4;
        break;
    case RGB5_A1:
        internalformat = GL_RGB5_A1;
        break;
    case RGB565:
        internalformat = GL_RGB565;
        break;
    }
    //BELog("Before glRenderbufferStorage(%dx%d, %d) GLError = %d\n", width, height, internalformat, glGetError());
    glRenderbufferStorage(target, internalformat, width, height);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
}

void GraphicsContext3D::sampleCoverage(GC3Dclampf value, GC3Dboolean invert)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glSampleCoverage(value, invert);
}

void GraphicsContext3D::scissor(GC3Dint x, GC3Dint y, GC3Dsizei width, GC3Dsizei height)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glScissor(x, y, width, height);
}

void GraphicsContext3D::shaderSource(Platform3DObject shader, const String& string)
{
    WEBGL_DEBUG("shader(%d)", shader);	
    makeContextCurrent();

    String replaceStr = string;
    int GLshaderType;
    
    glGetShaderiv(shader, SHADER_TYPE, &GLshaderType);
    if (GLshaderType == FRAGMENT_SHADER)
        replaceStr.replace("highp", "mediump");
    
    CString sourceCS = replaceStr.utf8();
    const char* str = sourceCS.data();
    int length = replaceStr.length();

#ifdef TVGG_PRINT_GLES_ERROR
    BELog("\n\n%s\n", str);
#endif
    glShaderSource(shader, 1, &str, &length);
}

void GraphicsContext3D::stencilFunc(GC3Denum func, GC3Dint ref, GC3Duint mask)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glStencilFunc(func, ref, mask);
}

void GraphicsContext3D::stencilFuncSeparate(GC3Denum face, GC3Denum func, GC3Dint ref, GC3Duint mask)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glStencilFuncSeparate(face, func, ref, mask);
}

void GraphicsContext3D::stencilMask(GC3Duint mask)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glStencilMask(mask);
}

void GraphicsContext3D::stencilMaskSeparate(GC3Denum face, GC3Duint mask)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glStencilMaskSeparate(face, mask);
}

void GraphicsContext3D::stencilOp(GC3Denum fail, GC3Denum zfail, GC3Denum zpass)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glStencilOp(fail, zfail, zpass);
}

void GraphicsContext3D::stencilOpSeparate(GC3Denum face, GC3Denum fail, GC3Denum zfail, GC3Denum zpass)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glStencilOpSeparate(face, fail, zfail, zpass);
}

void GraphicsContext3D::texParameterf(GC3Denum target, GC3Denum pname, GC3Dfloat value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glTexParameterf(target, pname, value);
}

void GraphicsContext3D::texParameteri(GC3Denum target, GC3Denum pname, GC3Dint value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glTexParameteri(target, pname, value);
}

void GraphicsContext3D::uniform1f(GC3Dint location, GC3Dfloat v0)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform1f(location, v0);
}

void GraphicsContext3D::uniform1fv(GC3Dint location, GC3Dfloat* array, GC3Dsizei size)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform1fv(location, size, array);
}

void GraphicsContext3D::uniform2f(GC3Dint location, GC3Dfloat v0, GC3Dfloat v1)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform2f(location, v0, v1);
}

void GraphicsContext3D::uniform2fv(GC3Dint location, GC3Dfloat* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 2
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform2fv(location, size, array);
}

void GraphicsContext3D::uniform3f(GC3Dint location, GC3Dfloat v0, GC3Dfloat v1, GC3Dfloat v2)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform3f(location, v0, v1, v2);
}

void GraphicsContext3D::uniform3fv(GC3Dint location, GC3Dfloat* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 3
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform3fv(location, size, array);
}

void GraphicsContext3D::uniform4f(GC3Dint location, GC3Dfloat v0, GC3Dfloat v1, GC3Dfloat v2, GC3Dfloat v3)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform4f(location, v0, v1, v2, v3);
}

void GraphicsContext3D::uniform4fv(GC3Dint location, GC3Dfloat* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 4
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform4fv(location, size, array);
}

void GraphicsContext3D::uniform1i(GC3Dint location, GC3Dint v0)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform1i(location, v0);
}

void GraphicsContext3D::uniform1iv(GC3Dint location, GC3Dint* array, GC3Dsizei size)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform1iv(location, size, array);
}

void GraphicsContext3D::uniform2i(GC3Dint location, GC3Dint v0, GC3Dint v1)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform2i(location, v0, v1);
}

void GraphicsContext3D::uniform2iv(GC3Dint location, GC3Dint* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 2
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform2iv(location, size, array);
}

void GraphicsContext3D::uniform3i(GC3Dint location, GC3Dint v0, GC3Dint v1, GC3Dint v2)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform3i(location, v0, v1, v2);
}

void GraphicsContext3D::uniform3iv(GC3Dint location, GC3Dint* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 3
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform3iv(location, size, array);
}

void GraphicsContext3D::uniform4i(GC3Dint location, GC3Dint v0, GC3Dint v1, GC3Dint v2, GC3Dint v3)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform4i(location, v0, v1, v2, v3);
}

void GraphicsContext3D::uniform4iv(GC3Dint location, GC3Dint* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 4
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniform4iv(location, size, array);
}

void GraphicsContext3D::uniformMatrix2fv(GC3Dint location, GC3Dboolean transpose, GC3Dfloat* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 4
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniformMatrix2fv(location, size, transpose, array);
}

void GraphicsContext3D::uniformMatrix3fv(GC3Dint location, GC3Dboolean transpose, GC3Dfloat* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 9
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniformMatrix3fv(location, size, transpose, array);
}

void GraphicsContext3D::uniformMatrix4fv(GC3Dint location, GC3Dboolean transpose, GC3Dfloat* array, GC3Dsizei size)
{
    // FIXME: length needs to be a multiple of 16
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUniformMatrix4fv(location, size, transpose, array);
}

void GraphicsContext3D::useProgram(Platform3DObject program)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glUseProgram(program);
}

void GraphicsContext3D::validateProgram(Platform3DObject program)
{
    ASSERT(program);

    WEBGL_DEBUG("");
    makeContextCurrent();

    glValidateProgram(program);

#ifdef TVGG_PRINT_GLES_ERROR
    GLint validated;
    // Check the compile status
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validated);
    if(!validated)
    {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            BELog("Error validating program:\n%s\n", infoLog);
            free(infoLog);
        }
    }
#endif
}

void GraphicsContext3D::vertexAttrib1f(GC3Duint index, GC3Dfloat v0)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib1f(index, v0);
}

void GraphicsContext3D::vertexAttrib1fv(GC3Duint index, GC3Dfloat* array)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib1fv(index, array);
}

void GraphicsContext3D::vertexAttrib2f(GC3Duint index, GC3Dfloat v0, GC3Dfloat v1)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib2f(index, v0, v1);
}

void GraphicsContext3D::vertexAttrib2fv(GC3Duint index, GC3Dfloat* array)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib2fv(index, array);
}

void GraphicsContext3D::vertexAttrib3f(GC3Duint index, GC3Dfloat v0, GC3Dfloat v1, GC3Dfloat v2)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib3f(index, v0, v1, v2);
}

void GraphicsContext3D::vertexAttrib3fv(GC3Duint index, GC3Dfloat* array)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib3fv(index, array);
}

void GraphicsContext3D::vertexAttrib4f(GC3Duint index, GC3Dfloat v0, GC3Dfloat v1, GC3Dfloat v2, GC3Dfloat v3)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib4f(index, v0, v1, v2, v3);
}

void GraphicsContext3D::vertexAttrib4fv(GC3Duint index, GC3Dfloat* array)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttrib4fv(index, array);
}

void GraphicsContext3D::vertexAttribPointer(GC3Duint index, GC3Dint size, GC3Denum type, GC3Dboolean normalized, GC3Dsizei stride, GC3Dintptr offset)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<GLvoid*>(static_cast<intptr_t>(offset)));
}

void GraphicsContext3D::viewport(GC3Dint x, GC3Dint y, GC3Dsizei width, GC3Dsizei height)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glViewport(x, y, width, height);
}

void GraphicsContext3D::getBooleanv(GC3Denum pname, GC3Dboolean* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetBooleanv(pname, value);
}

void GraphicsContext3D::getBufferParameteriv(GC3Denum target, GC3Denum pname, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetBufferParameteriv(target, pname, value);
}

void GraphicsContext3D::getFloatv(GC3Denum pname, GC3Dfloat* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetFloatv(pname, value);
}

void GraphicsContext3D::getFramebufferAttachmentParameteriv(GC3Denum target, GC3Denum attachment, GC3Denum pname, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    if (attachment == DEPTH_STENCIL_ATTACHMENT)
        attachment = DEPTH_ATTACHMENT; // Or STENCIL_ATTACHMENT, either works.
    glGetFramebufferAttachmentParameteriv(target, attachment, pname, value);
}

void GraphicsContext3D::getIntegerv(GC3Denum pname, GC3Dint* value)
{
    // Need to emulate MAX_FRAGMENT/VERTEX_UNIFORM_VECTORS and MAX_VARYING_VECTORS
    // because desktop GL's corresponding queries return the number of components
    // whereas GLES2 return the number of vectors (each vector has 4 components).
    // Therefore, the value returned by desktop GL needs to be divided by 4.
    
    WEBGL_DEBUG("");
    makeContextCurrent();

    switch (pname) {
    case MAX_FRAGMENT_UNIFORM_VECTORS:
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, value);
        break;
    case MAX_VERTEX_UNIFORM_VECTORS:
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, value);
        break;
    case MAX_VARYING_VECTORS:
        glGetIntegerv(GL_MAX_VARYING_VECTORS, value);
        break;
    default:
        glGetIntegerv(pname, value);
    }
}

void GraphicsContext3D::getProgramiv(Platform3DObject program, GC3Denum pname, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetProgramiv(program, pname, value);
}

String GraphicsContext3D::getProgramInfoLog(Platform3DObject program)
{
    ASSERT(program);

    WEBGL_DEBUG("");
    makeContextCurrent();

    int logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength <= 1)
        return String();

    OwnArrayPtr<char> log = adoptArrayPtr(new char[logLength]);
    if (!log)
        return String();

    int returnedLogLength = 0;
    glGetProgramInfoLog(program, logLength, &returnedLogLength, log.get());
    ASSERT(logLength == returnedLogLength + 1);

    String res = String::fromUTF8(log.get(), returnedLogLength);
    return res;
}

void GraphicsContext3D::getRenderbufferParameteriv(GC3Denum target, GC3Denum pname, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetRenderbufferParameteriv(target, pname, value);
}

void GraphicsContext3D::getShaderiv(Platform3DObject shader, GC3Denum pname, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();
	
    glGetShaderiv(shader, pname, value);
}

String GraphicsContext3D::getShaderInfoLog(Platform3DObject shader)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    int logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength <= 1)
        return String();

    OwnArrayPtr<char> log = adoptArrayPtr(new char[logLength]);
    if (!log)
        return String();

    int returnedLogLength = 0;
    glGetShaderInfoLog(shader, logLength, &returnedLogLength, log.get());
    ASSERT(logLength == returnedLogLength + 1);

    String res = String::fromUTF8(log.get(), returnedLogLength);
    return res;
}

String GraphicsContext3D::getShaderSource(Platform3DObject shader)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    int logLength = 0;
    glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &logLength);
    if (logLength <= 1)
        return String();

    OwnArrayPtr<char> log = adoptArrayPtr(new char[logLength]);
    if (!log)
        return String();

    int returnedLogLength = 0;
    glGetShaderSource(shader, logLength, &returnedLogLength, log.get());
    ASSERT(logLength == returnedLogLength + 1);

    String res = String::fromUTF8(log.get(), returnedLogLength);
    return res;
}


void GraphicsContext3D::getTexParameterfv(GC3Denum target, GC3Denum pname, GC3Dfloat* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetTexParameterfv(target, pname, value);
}

void GraphicsContext3D::getTexParameteriv(GC3Denum target, GC3Denum pname, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetTexParameteriv(target, pname, value);
}

void GraphicsContext3D::getUniformfv(Platform3DObject program, GC3Dint location, GC3Dfloat* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetUniformfv(program, location, value);
}

void GraphicsContext3D::getUniformiv(Platform3DObject program, GC3Dint location, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetUniformiv(program, location, value);
}

GC3Dint GraphicsContext3D::getUniformLocation(Platform3DObject program, const String& name)
{
    ASSERT(program);

    WEBGL_DEBUG("");
    makeContextCurrent();

    return glGetUniformLocation(program, name.utf8().data());
}

void GraphicsContext3D::getVertexAttribfv(GC3Duint index, GC3Denum pname, GC3Dfloat* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetVertexAttribfv(index, pname, value);
}

void GraphicsContext3D::getVertexAttribiv(GC3Duint index, GC3Denum pname, GC3Dint* value)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    glGetVertexAttribiv(index, pname, value);
}

GC3Dsizeiptr GraphicsContext3D::getVertexAttribOffset(GC3Duint index, GC3Denum pname)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    GLvoid* pointer = 0;
    glGetVertexAttribPointerv(index, pname, &pointer);
    return static_cast<GC3Dsizeiptr>(reinterpret_cast<intptr_t>(pointer));
}

bool GraphicsContext3D::texImage2D(GC3Denum target, GC3Dint level, GC3Denum internalformat, GC3Dsizei width, GC3Dsizei height, GC3Dint border, GC3Denum format, GC3Denum type, const void* pixels)
{
    if (width && height && !pixels) {
        synthesizeGLError(INVALID_VALUE);
        return false;
    }
	
    WEBGL_DEBUG("");
    makeContextCurrent();

    GC3Denum openGLInternalFormat = internalformat;
#if !ENABLE(TVGG_BROWSER)
    if (type == GL_FLOAT) {
        if (format == GL_RGBA)
            openGLInternalFormat = GL_RGBA32F_ARB;
        else if (format == GL_RGB)
            openGLInternalFormat = GL_RGB32F_ARB;
    }
#endif
    //BELog("Before glTexImage2D(%dx%d, %d) GLError = %d\n", width, height, internalformat, glGetError());
    glTexImage2D(target, level, openGLInternalFormat, width, height, border, format, type, pixels);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
    return true;
}

void GraphicsContext3D::texSubImage2D(GC3Denum target, GC3Dint level, GC3Dint xoff, GC3Dint yoff, GC3Dsizei width, GC3Dsizei height, GC3Denum format, GC3Denum type, const void* pixels)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    // FIXME: we will need to deal with PixelStore params when dealing with image buffers that differ from the subimage size
    //BELog("Before glTexSubImage2D(%dx%d, %d) GLError = %d\n", width, height, format, glGetError());
    glTexSubImage2D(target, level, xoff, yoff, width, height, format, type, pixels);
#if ENABLE(TVGG_EXIT_ON_GPU_OOM)
    checkGLOOMandExit();
#endif
}

Platform3DObject GraphicsContext3D::createBuffer()
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
	
    WEBGL_DEBUG("buffer(%d)", buffer);
    return buffer;
}

Platform3DObject GraphicsContext3D::createFramebuffer()
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    GLuint framebuffer = 0;
    glGenFramebuffers(1, &framebuffer);

    WEBGL_DEBUG("framebuffer(%d)", framebuffer);
	return framebuffer;
}

Platform3DObject GraphicsContext3D::createProgram()
{
    makeContextCurrent();

    GLuint program = 0;
    program = glCreateProgram();
	
    WEBGL_DEBUG("program(%d)", program);
	return program;
}

Platform3DObject GraphicsContext3D::createRenderbuffer()
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    GLuint renderbuffer = 0;
    glGenRenderbuffers(1, &renderbuffer);

    WEBGL_DEBUG("renderbuffer(%d)", renderbuffer);
	return renderbuffer;
}

Platform3DObject GraphicsContext3D::createShader(GC3Denum type)
{
    WEBGL_DEBUG("");
    makeContextCurrent();

    GLuint shader = 0;
    shader = glCreateShader((type == FRAGMENT_SHADER) ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER);
	
    WEBGL_DEBUG("shader(%d)", shader);
	return shader;
}

Platform3DObject GraphicsContext3D::createTexture()
{
    makeContextCurrent();

    GLuint texture = 0;
    glGenTextures(1, &texture);

    WEBGL_DEBUG("texture(%d)", texture);
    return texture;
}

void GraphicsContext3D::deleteBuffer(Platform3DObject buffer)
{
    WEBGL_DEBUG("buffer(%d)", buffer);
    makeContextCurrent();

    glDeleteBuffers(1, &buffer);
}

void GraphicsContext3D::deleteFramebuffer(Platform3DObject framebuffer)
{
    WEBGL_DEBUG("framebuffer(%d)", framebuffer);
    makeContextCurrent();

    glDeleteFramebuffers(1, &framebuffer);
}

void GraphicsContext3D::deleteProgram(Platform3DObject program)
{
    WEBGL_DEBUG("program(%d)", program);	
    makeContextCurrent();

    glDeleteProgram(program);
}

void GraphicsContext3D::deleteRenderbuffer(Platform3DObject renderbuffer)
{
    WEBGL_DEBUG("renderbuffer(%d)", renderbuffer);
    makeContextCurrent();

    glDeleteRenderbuffers(1, &renderbuffer);
}

void GraphicsContext3D::deleteShader(Platform3DObject shader)
{
    WEBGL_DEBUG("shader(%d)", shader);
    makeContextCurrent();

    glDeleteShader(shader);
}

void GraphicsContext3D::deleteTexture(Platform3DObject texture)
{
    WEBGL_DEBUG("texture(%d)", texture);
    makeContextCurrent();

    glDeleteTextures(1, &texture);
}

void GraphicsContext3D::synthesizeGLError(GC3Denum error)
{
    m_syntheticErrors.add(error);
}

void GraphicsContext3D::markContextChanged()
{
    m_layerComposited = false;
}

void GraphicsContext3D::markLayerComposited()
{
    m_layerComposited = true;
}

bool GraphicsContext3D::layerComposited() const
{
    return m_layerComposited;
}

Extensions3D* GraphicsContext3D::getExtensions()
{
    if (!m_extensions)
        m_extensions = adoptPtr(new Extensions3DOpenGL(this));
    return m_extensions.get();
}

}

#endif // ENABLE(WEBGL)
