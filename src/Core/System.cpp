#include "System.hpp"
#include <iostream>
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

System::System() :
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
		// positions            // texture coords
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindTexture(GL_TEXTURE_2D, texture_1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("../resources/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture_2);
	glBindTexture(GL_TEXTURE_2D, texture_2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("../resources/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader->Use();
	shader->SetInt("texture_1", 0);
	shader->SetInt("texture_2", 1);


	return true;
}

void System::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_2);

	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
	float scaleAmount = sin(glfwGetTime());
	transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));

	shader->Use();
	shader->SetMat4("transform", transform);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
