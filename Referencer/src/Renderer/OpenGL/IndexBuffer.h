#pragma once
namespace Referencer {
    class IndexBuffer
    {
    private:
        GLuint m_bufferID;
        GLuint m_count;

    public:
        IndexBuffer(const GLuint* indices, GLuint count);
        ~IndexBuffer();

        void bind() const;
        void unBind() const;

        inline unsigned int GetCount() const { return m_Count; }
    };
}