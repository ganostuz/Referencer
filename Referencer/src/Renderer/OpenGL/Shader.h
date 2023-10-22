#pragma once

namespace Referencer {

	class Shader
	{
	private:
		GLuint m_programID = 0;
		std::string m_vetexFilePath, m_fragmentFilePath;
		std::unordered_map<std::string, int> m_uniformLocationCache;

        int getUniformLocation(const std::string& name) const;
	public:
        Shader(const std::string& vetexFilePath, const std::string& fragmentFilePath);
        ~Shader();

        void bind() const;
        void unBind() const;

        // uniforms, add glm
        void setUniform1f(const std::string& name, float value);
        void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
        //void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
	};
}