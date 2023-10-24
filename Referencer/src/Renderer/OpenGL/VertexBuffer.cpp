#include "rfpch.h"
#include "VertexBuffer.h"
#include "glad\glad.h"

namespace Referencer {
    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
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

    void VertexBuffer::addFloat(unsigned int count)
    {
        push(GL_FLOAT, count, GL_FALSE);
    }

    void VertexBuffer::addUnsignedInt(unsigned int count)
    {
        push(GL_UNSIGNED_INT, count, GL_FALSE);
    }

    void VertexBuffer::addUnsignedByte(unsigned int count)
    {
        push(GL_UNSIGNED_BYTE, count, GL_TRUE);
    }

    //private funcs

    void VertexBuffer::push(unsigned int type, unsigned int count, unsigned int normalized)
    {
        struct VertexBufferElement vbe = { type, count, normalized };
        m_elements.push_back(vbe);
        m_stride += count * VertexBufferElement::getSizeOfType(type);
    };
}
