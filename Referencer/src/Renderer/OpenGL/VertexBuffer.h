#pragma once
namespace Referencer {

    struct VertexBufferElement
    {
        GLuint type;
        GLuint count;
        GLuint normalized;

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
        GLuint m_bufferID;
        GLuint m_stride;
        std::vector<VertexBufferElement> m_elements;

        void push(GLuint type, GLuint count, GLuint normalized);
    
    public:
        VertexBuffer(const void* data, GLuint size);
        ~VertexBuffer();
    
        void bind() const;
        void unBind() const;

        void addFloat(GLuint count);
        void addUnsignedInt(GLuint count);
        void addUnsignedByte(GLuint count);

        inline const std::vector<VertexBufferElement> getElements() const;
        inline GLuint getStride() const;
    };
}
