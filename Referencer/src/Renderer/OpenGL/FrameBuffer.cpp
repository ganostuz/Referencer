#include "rfpch.h"
#include "FrameBuffer.h"
#include "glad\glad.h"

namespace Referencer {
	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
		:m_bufferID(0), m_renderBuffer_ID(0), m_texture_ID(0)
	{
		glGenFramebuffers(1, &m_bufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);

		glGenTextures(1, &m_texture_ID);
		glBindTexture(GL_TEXTURE_2D, m_texture_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_ID, 0);

		glGenRenderbuffers(1, &m_renderBuffer_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer_ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer_ID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteBuffers(1, &m_bufferID); // not sure if i need to delete other buffers
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
	}

	void FrameBuffer::resize(unsigned int width, unsigned int height)
	{
		glBindTexture(GL_TEXTURE_2D, m_texture_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_ID, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer_ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer_ID);
	}

	void FrameBuffer::unBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}