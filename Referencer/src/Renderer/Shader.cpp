#include "rfpch.h"
#include <fstream>
#include "Shader.h"
#include <glm/common.hpp>

//#define ASSERT(cond, ...) { if(!(cond)) { std::cout<<"Assertion Failed: "<< __VA_ARGS__ << std::endl; __debugbreak(); } }
// 
namespace Referencer {
	Shader::Shader()
	{

	}
	Shader::Shader(const std::string& vertex, const std::string& fragment, bool isPath)
	{
		setShaders(vertex, fragment, isPath);
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_programID);
	}

	int Shader::getUniformLocation(const std::string& name)
	{
		if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
			return m_uniformLocationCache[name];

		int location = glGetUniformLocation(m_programID, name.c_str());

		if (location == -1)
		{
			std::cerr << "No active uniform variable with name " << name << std::endl;
			return location;
		}

		m_uniformLocationCache[name] = location;
		return location;
	}

	void Shader::bind() const
	{
		glUseProgram(m_programID);
	}
	void Shader::unBind() const
	{
		glUseProgram(0);
	}

	void Shader::setUniform1f(const std::string& name, float value)
	{
		glUniform1f(getUniformLocation(name), value);
	}
	void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
	{
		glUniform4f(getUniformLocation(name), f0, f1, f2, f3);
	}
	void Shader::setUniform3f(const std::string& name, float f0, float f1, float f2)
	{
		glUniform3f(getUniformLocation(name), f0, f1, f2);
	}
	void Shader::setMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::setShaders(const std::string& vertex, const std::string& fragment, bool isPath)
	{
		
		if (isPath)
		{
			if (!vertex.empty())
			{
				std::ifstream vstream(vertex);
				m_vertex = std::string((std::istreambuf_iterator<char>(vstream)), std::istreambuf_iterator<char>());
			}
			
			if (!fragment.empty())
			{
				std::ifstream fstream(fragment);
				m_fragment = std::string((std::istreambuf_iterator<char>(fstream)), std::istreambuf_iterator<char>());
			}
			
		}
		else {
			if (!vertex.empty())
				m_vertex = vertex;
			if (!fragment.empty())
				m_fragment = fragment;
		}


		const GLchar* source = m_vertex.c_str();
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, &source, NULL);
		glCompileShader(vertexShaderID);

		// errors
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
			std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
			return;
		}

		// Create and compile the fragment shader
		source = m_fragment.c_str();
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, &source, NULL);
		glCompileShader(fragmentShaderID);

		// errors
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
			std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
			return;
		}

		// Create and delete a shader program and link the shaders
		glDeleteProgram(m_programID);
		m_programID = glCreateProgram();

		glAttachShader(m_programID, vertexShaderID);
		glAttachShader(m_programID, fragmentShaderID);
		glLinkProgram(m_programID);

		// errors

		glGetShaderiv(m_programID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
			std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
			return;
		}

		glValidateProgram(m_programID); // idk if it needs to be used but documentation...

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

}