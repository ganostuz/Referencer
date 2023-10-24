#pragma once
namespace Referencer {

	class FrameBuffer
	{
	private:
		unsigned int m_bufferID;

		unsigned int m_renderBuffer_ID, m_texture_ID;
	public:
		FrameBuffer(unsigned int width, unsigned int height);
		~FrameBuffer();

		void bind();
		void resize(unsigned int width, unsigned int height);
		void unBind();
	
	};
}
/*void Renderer::Clear() const
{
        GLCall( glClear( GL_COLOR_BUFFER_BIT ) );
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
        shader.Bind();
        va.Bind();
        ib.Bind();
        GLCall( glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr) );
}
*/