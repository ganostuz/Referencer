#pragma once
#include "glad\glad.h"
namespace Referencer {

    struct VertexBufferElement
    {
        unsigned int type;
        unsigned int count;
        unsigned int normalized;

        static unsigned int getSizeOfType(unsigned int type)
        {
            switch (type)
            {
                case GL_FLOAT: return sizeof(GLfloat);
                case GL_UNSIGNED_INT: return sizeof(GLuint);
                case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
            }
            return -1;
        }
    };

    class VertexBuffer
    {
    private:
        unsigned int m_bufferID;
        unsigned int m_stride;
        std::vector<VertexBufferElement> m_elements;

        void push(unsigned int type, unsigned int count, unsigned int normalized);
    
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();
    
        void bind() const;
        void unBind() const;

        void addFloat(unsigned int count);
        void addUnsignedInt(unsigned int count);
        void addUnsignedByte(unsigned int count);

        inline const std::vector<VertexBufferElement> getElements() const { return m_elements; };
        inline unsigned int getStride() const { return m_stride; };
    };
}
