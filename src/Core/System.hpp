#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Camera.hpp"

class System
{
public:
	System();
	~System();

public:
	bool Initialize();
	void Update();
	void Render();
	void Release();

	int ShouldCloseWindow();
	void ProcessInput();

private:
	bool SystemInitialize();

	//흠 일단 이렇게하자
	static void MouseCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			mouseX = xPos;
			mouseY = yPos;

			firstMouse = false;
		}

		double xOffset, yOffset;

		xOffset = xPos - mouseX;
		yOffset = mouseY - yPos;

		mouseX = xPos;
		mouseY = yPos;

		float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.f)
		{
			pitch = 89.f;
		}
		if (pitch < -89.f)
		{
			pitch = -89.f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		camera->SetFrontVector(front);
	}

	static void ScrollCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (fov >= 1.f && fov <= 45.f)
		{
			fov -= yPos;
		}
		if (fov <= 1.f)
		{
			fov = 1.f;
		}
		if (fov >= 45.f)
		{
			fov = 45.f;
		}
	}

	//클래스 만들기전 임시
	unsigned int VAO, VBO, EBO;
	unsigned int texture_1, texture_2;

private:
	GLFWwindow* windowContext;
	Shader* shader;
	static Camera* camera;

	glm::vec3 cubePositions[10];

	static float yaw, pitch;
	static double mouseX, mouseY;
	static bool firstMouse;
	static float fov;
};