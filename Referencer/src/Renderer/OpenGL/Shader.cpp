#include "rfpch.h"
#include <fstream>
#include "Shader.h"
#include "glad\glad.h"

//#define ASSERT(cond, ...) { if(!(cond)) { std::cout<<"Assertion Failed: "<< __VA_ARGS__ << std::endl; __debugbreak(); } }
// 
namespace Referencer {

	Shader::Shader(const std::string& vetexFilePath, const std::string& fragmentFilePath)
	{
		
		std::ifstream vstream(vetexFilePath);
		const std::string vertexShaderSource((std::istreambuf_iterator<char>(vstream)), std::istreambuf_iterator<char>());
		
		std::ifstream fstream(fragmentFilePath);
		const std::string fragmentShaderSource((std::istreambuf_iterator<char>(fstream)), std::istreambuf_iterator<char>());
		
		const GLchar* source = vertexShaderSource.c_str();
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &source, NULL);
		glCompileShader(vertexShader);

		// errors
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
			return;
		}

		// Create and compile the fragment shader
		source = fragmentShaderSource.c_str();
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &source, NULL);
		glCompileShader(fragmentShader);

		// errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
			return;
		}

		// Create a shader program and link the shaders
		m_programID = glCreateProgram();

		glAttachShader(m_programID, vertexShader);
		glAttachShader(m_programID, fragmentShader);
		glLinkProgram(m_programID);

		// errors
		
		glGetShaderiv(m_programID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
			return;
		}

		glValidateProgram(m_programID); // idk if it needs to be used but documentation...

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
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
	/*
	void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}
	*/

}

