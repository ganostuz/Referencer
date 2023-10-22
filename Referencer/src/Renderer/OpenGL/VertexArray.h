#pragma once
namespace Referencer {
    class VertexArray
    {
    private:
        GLuint m_bufferID;

    public:
        VertexArray();
        ~VertexArray();

        void addBuffer(const VertexBuffer& vb);
        void bind() const;
        void unBind() const;
    };

}
