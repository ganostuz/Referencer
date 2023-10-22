#include "rfpch.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glad\glad.h"

namespace Referencer {

    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_bufferID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_bufferID);
    }

    void VertexArray::addBuffer(const VertexBuffer& vb)
    {
        bind();
        vb.bind();
        const std::vector<VertexBufferElement> elements = vb.getElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++)
        {
            const VertexBufferElement element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), INT2VOIDP(offset));
            offset += element.count * VertexBufferElement::getSizeOfType(element.type);
        }
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_bufferID);
    }

    void VertexArray::unBind() const
    {
        GLCall(glBindVertexArray(0));
    };

}
