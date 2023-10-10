#pragma once

namespace Referencer {
	class Shader
	{
	private:
		uint32_t m_programID = -1;
	public:
		Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		~Shader();

		void bind();
		void unBind();
		


	};
}