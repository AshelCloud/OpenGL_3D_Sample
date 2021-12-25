#pragma once

#include <glad/glad.h>
#include <string>

class Shader
{
public:
	Shader(const GLchar* vsPath, const GLchar* fsPath);
	virtual ~Shader();

public:
	void Use();
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w) const;

	void Release();

private:
	unsigned int ID;
};