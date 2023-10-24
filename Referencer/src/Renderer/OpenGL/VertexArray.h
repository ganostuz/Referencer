#pragma once
#include "VertexBuffer.h"
namespace Referencer {
    class VertexArray
    {
    private:
        unsigned int m_bufferID;

    public:
        VertexArray();
        ~VertexArray();

        void addBuffer(const VertexBuffer& vb);
        void bind() const;
        void unBind() const;
    };

}
