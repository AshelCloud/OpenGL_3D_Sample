#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const GLchar* vsPath, const GLchar* fsPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vsFile;
	std::ifstream fsFile;

	vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//vertex부터 읽자
		vsFile.open(vsPath);
		
		std::stringstream vsStream;
		vsStream << vsFile.rdbuf();

		vertexCode = vsStream.str();

		vsFile.close();

		//fragment
		fsFile.open(fsPath);
		
		std::stringstream fsStream;
		fsStream << fsFile.rdbuf();

		fragmentCode = fsStream.str();

		fsFile.close();

		//이제 Shader 컴파일
		int result;
		char infoLog[512];

		const char* vSource = vertexCode.c_str();
		unsigned int v;

		v = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(v, 1, &vSource, nullptr);
		glCompileShader(v);

		glGetShaderiv(v, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(v, 512, nullptr, infoLog);
			std::cout << infoLog << std::endl;
		}

		const char* fSource = fragmentCode.c_str();
		unsigned int f;

		f = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(f, 1, &fSource, nullptr);
		glCompileShader(f);

		glGetShaderiv(f, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(f, 512, nullptr, infoLog);
			std::cout << infoLog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, v);
		glAttachShader(ID, f);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(f, 512, nullptr, infoLog);
			std::cout << infoLog << std::endl;
		}

		glDeleteShader(v);
		glDeleteShader(f);
	}
	catch (std::ifstream::failure e)
	{
		std::cout << e.what() << std::endl;
	}
}

Shader::~Shader()
{

}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
{
	unsigned int loc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::Release()
{
	
}

