#pragma once
namespace Referencer {
    class IndexBuffer
    {
    private:
        unsigned int m_bufferID;
        unsigned int m_count;

    public:
        IndexBuffer(const unsigned int* indices, unsigned int count);
        ~IndexBuffer();

        void bind() const;
        void unBind() const;

        inline unsigned int GetCount() const { return m_count; }
    };
}