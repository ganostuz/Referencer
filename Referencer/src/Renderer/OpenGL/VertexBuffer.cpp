#include "rfpch.h"
#include "VertexBuffer.h"
#include "glad\glad.h"

namespace Referencer {
    VertexBuffer::VertexBuffer(const void* data, GLuint size)
        : m_stride(0)
    {
        glGenBuffers(1, &m_bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_bufferID);
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    }

    void VertexBuffer::unBind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::addFloat(GLuint count)
    {
        push(GL_FLOAT, count, GL_FALSE);
    }

    void VertexBuffer::addUnsignedInt(GLuint count)
    {
        push(GL_UNSIGNED_INT, count, GL_FALSE);
    }

    void VertexBuffer::addUnsignedByte(GLuint count)
    {
        push(GL_UNSIGNED_BYTE, count, GL_TRUE);
    }

    inline const std::vector<VertexBufferElement> VertexBuffer::getElements() const
    {
        return m_elements;
    }

    inline GLuint VertexBuffer::getStride() const
    {
        return m_stride;
    }

    //private funcs

    void VertexBuffer::push(GLuint type, GLuint count, GLuint normalized)
    {
        struct VertexBufferElement vbe = { type, count, normalized };
        m_elements.push_back(vbe);
        m_stride += count * VertexBufferElement::getSizeOfType(type);
    };
}
