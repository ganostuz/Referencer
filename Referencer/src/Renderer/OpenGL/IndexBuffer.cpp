#include "rfpch.h"
#include "IndexBuffer.h"
#include "glad\glad.h"

namespace Referencer {
    IndexBuffer::IndexBuffer(const GLuint* indices, GLuint count)
        : m_count(count)
    {
        glGenBuffers(1, &m_bufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::unBind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}