#include "rfpch.h"
#include "Shader.h"
#include "glad\glad.h"

#define RF_ASSERT(cond, ...) { if(!(cond)) { std::cout<<"Assertion Failed: "<< __VA_ARGS__ << std::endl; __debugbreak(); } }
namespace Referencer {

	Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		
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

		glDetachShader(m_programID, vertexShader);
		glDetachShader(m_programID, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_programID);
	}
	void Shader::bind()
	{
		glUseProgram(m_programID);
	}
	void Shader::unBind()
	{
		glUseProgram(0);
	}
}

