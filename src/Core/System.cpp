#include "System.h"
#include <iostream>

System::System():
	windowContext(nullptr), shader(nullptr), VAO(0), VBO(0)
{

}

System::~System()
{

}

bool System::Initialize()
{
	bool result = SystemInitialize();
	if (false == result)
	{
		return false;
	}

	std::string fs = "F:/ACEngine/src/Shader/";
	shader = new Shader((fs + "vshader.vs").c_str(), (fs + "fshader.fs").c_str());

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return true;
}

void System::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->Use();
	shader->SetVec4("color", 0.f, sin(glfwGetTime()) / 2.f + 0.5f, 0.f, 1.f);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(windowContext);
	glfwPollEvents();
}

void System::Release()
{
	if (shader)
	{
		shader->Release();
		delete shader;
		shader = nullptr;
	}

	glfwDestroyWindow(windowContext);
	glfwTerminate();
}

int System::ShouldCloseWindow()
{
	return glfwWindowShouldClose(windowContext);
}

void System::ProcessInput()
{
	if (glfwGetKey(windowContext, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(windowContext, true);
	}
	
	if (glfwGetKey(windowContext, GLFW_KEY_1) == GLFW_PRESS)
	{
		//wire로 보자
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(windowContext, GLFW_KEY_1) == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

bool System::SystemInitialize()
{
	int Result = glfwInit();
	if (GLFW_FALSE == Result)
	{
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	windowContext = glfwCreateWindow(800, 600, "ACEngine", nullptr, nullptr);
	if (nullptr == windowContext)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(windowContext);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glViewport(0, 0, 800, 600);

	//static 함수로 할필요가 있을까
	//뭐 일단 처리하는건 없으니 람다로하고 나중에 처리하자
	glfwSetFramebufferSizeCallback(windowContext, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});

	return true;
}
