#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"


class System
{
public:
	System();
	~System();

public:
	bool Initialize();
	void Render();
	void Release();

	int ShouldCloseWindow();
	void ProcessInput();

private:
	bool SystemInitialize();

	//클래스 만들기전 임시
	unsigned int VAO, VBO;

private:
	GLFWwindow* windowContext;
	Shader* shader;
};