#pragma once

#include "GLFW/glfw3.h"
#include "Singleton.h"

class Time : public Singleton<Time>
{
public:
	Time():
		deltaTime(0.f), lastTime(0.f)
	{
	}
	~Time() {}
public:
	float GetDeltaTime()
	{
		return deltaTime;
	}

public:
	void Update()
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

private:
	float deltaTime;
	float lastTime;
};