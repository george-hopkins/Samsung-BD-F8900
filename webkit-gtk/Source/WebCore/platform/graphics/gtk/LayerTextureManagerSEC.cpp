/*
    Copyright (C) 2009-2010 Samsung Electronics

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#if 0
#include "LayerTextureManagerSEC.h"

#if USE(ACCELERATED_COMPOSITING_WITH_GLES)

#include "GraphicsContext3D.h"

#include <stdio.h>

namespace WebCore {

PassOwnPtr<LayerTextureManager> LayerTextureManagerSEC::create(GraphicsContext3D* context)
{
    return adoptPtr(new LayerTextureManagerSEC(context));
}

LayerTextureManagerSEC::LayerTextureManagerSEC(GraphicsContext3D* context)
	: LayerTextureManager(context)
	, m_extensions(NULL)
{
	m_bInitialized = initialize();
}

LayerTextureManagerSEC::~LayerTextureManagerSEC()
{
}

bool LayerTextureManagerSEC::initialize()
{
	if (!LayerTextureManager::initialize())
		return false;

	m_extensions = static_cast<Extensions3DGtk*>(m_context->getExtensions());
	if (!m_extensions)
		return false;

	if (!m_extensions->supports("EGL_KHR_image") ||
		!m_extensions->supports("EGL_SEC_image_map") ||
		!m_extensions->supports("GL_OES_EGL_image")) {
		return false;
	}
	return true;
}

void LayerTextureManagerSEC::release()
{

}

unsigned LayerTextureManagerSEC::createTexture()
{
    unsigned textureId = m_context->createTexture();

    m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, textureId);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MIN_FILTER, GraphicsContext3D::LINEAR);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MAG_FILTER, GraphicsContext3D::LINEAR);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_S, GraphicsContext3D::CLAMP_TO_EDGE);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_T, GraphicsContext3D::CLAMP_TO_EDGE);

    m_mappedEGLImages.add(textureId, 0);
	return textureId;
}

void LayerTextureManagerSEC::deleteTexture(unsigned id)
{
	mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
	if (it == m_mappedEGLImages.end()) {
		printf("[%p] LayerTextureManagerSEC::%s() - cannot find %d texture ID\n", this, __func__, (int)id);
		return;
	}

	if (it->second)
		m_extensions->destroyEGLImage(it->second);

	m_context->deleteTexture(id);

	m_mappedEGLImages.remove(it);
}

void LayerTextureManagerSEC::updateTexture(unsigned id, const IntSize& textureSize, void* pixels)
{
	mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
	if (it == m_mappedEGLImages.end()) {
		printf("[%p] LayerTextureManagerSEC::%s() - cannot find %d texture ID\n", this, __func__, (int)id);
		return;
	}

	EGLImageKHR eglImage = it->second;
	if (!eglImage) {
		eglImage = m_extensions->createEGLImage(textureSize.width(), textureSize.height());
		ASSERT(eglImage);
		it->second = eglImage;
	}

	int eglImageWidth = m_extensions->getEGLImageAttrib(eglImage, EGL_MAP_GL_TEXTURE_WIDTH_SEC);
	int eglImageHeight = m_extensions->getEGLImageAttrib(eglImage, EGL_MAP_GL_TEXTURE_HEIGHT_SEC);

	void* ptr = m_extensions->mapEGLImage(eglImage);
	if (!ptr) {
		printf("[%p] LayerTextureManagerSEC::%s() - mapEGLImage failed\n", this, __func__);
		return;
	}

	updateEGLImage(ptr, pixels, 0, 0, textureSize.width(), textureSize.height(), eglImageWidth, eglImageHeight);

	if (!m_extensions->unmapEGLImage(eglImage)) {
		printf("[%p] LayerTextureManagerSEC::%s() - unmapEGLImage failed\n", this, __func__);
		return;
	}
}

void LayerTextureManagerSEC::updateTexture(unsigned id, const IntRect& updateRect, void* pixels)
{
	mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
	if (it == m_mappedEGLImages.end()) {
		printf("[%p] LayerTextureManagerSEC::%s() - cannot find %d texture ID\n", this, __func__, (int)id);
		return;
	}

	EGLImageKHR eglImage = it->second;
	ASSERT(eglImage);

	int eglImageWidth = m_extensions->getEGLImageAttrib(eglImage, EGL_MAP_GL_TEXTURE_WIDTH_SEC);
	int eglImageHeight = m_extensions->getEGLImageAttrib(eglImage, EGL_MAP_GL_TEXTURE_HEIGHT_SEC);

	ASSERT(eglImageWidth >= updateRect.width());
	ASSERT(eglImageHeight >= updateRect.height());

	void* ptr = m_extensions->mapEGLImage(eglImage);
	if (!ptr) {
		printf("[%p] LayerTextureManagerSEC::%s() - mapEGLImage failed\n", this, __func__);
		return;
	}

	updateEGLImage(ptr, pixels, updateRect.x(), updateRect.y(), updateRect.width(), updateRect.height(), eglImageWidth, eglImageHeight);

	if (!m_extensions->unmapEGLImage(eglImage)) {
		printf("[%p] LayerTextureManagerSEC::%s() - unmapEGLImage failed\n", this, __func__);
		return;
	}
}

void LayerTextureManagerSEC::useTexture(unsigned id)
{
	mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
	if (it == m_mappedEGLImages.end()) {
		printf("[%p] LayerTextureManagerSEC::%s() - cannot find %d texture ID\n", this, __func__, (int)id);
		return;
	}
	if (!it->second)
		return;

	m_context->activeTexture(GraphicsContext3D::TEXTURE0);
	m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, id);
	m_extensions->EGLImageTargetTexture2D(GraphicsContext3D::TEXTURE_2D, (GLeglImageOES)it->second);
}

void LayerTextureManagerSEC::updateEGLImage(void* dst, const void* src, int offsetX, int offsetY, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
	int dstStride = dstWidth * 4;
	int srcStride = srcWidth * 4;

	unsigned char* ptr = (unsigned char*)dst + (offsetY * dstStride) + (offsetX * 4);

	for (int i = 0; i < srcHeight; ++i) {
		memcpy(ptr + (i * dstStride), (unsigned char*)src + (i * srcStride), srcStride);
	}
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
#endif
