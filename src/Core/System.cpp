#include "System.hpp"
#include <iostream>
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Math.h"
#include "Mouse.h"

Camera* System::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// world space positions of our cubes
	cubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	cubePositions[1] = glm::vec3(2.0f, 5.0f, -15.0f);
	cubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
	cubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
	cubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
	cubePositions[5] = glm::vec3(-1.7f, 3.0f, -7.5f);
	cubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
	cubePositions[7] = glm::vec3(1.5f, 2.0f, -2.5f);
	cubePositions[8] = glm::vec3(1.5f, 0.2f, -1.5f);
	cubePositions[9] = glm::vec3(-1.3f, 1.0f, -1.5f);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

	glEnable(GL_DEPTH_TEST);

	return true;
}

void System::Update()
{
	Time::GetInstance().Update();

	glm::mat4 projection = glm::perspective(glm::radians(fov), 800.f / 600.f, 0.1f, 100.0f);
	shader->SetMat4("projection", projection);
}

void System::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_2);

	shader->Use();

	glBindVertexArray(VAO);

	glm::mat4 view = glm::lookAt(camera->GetPosition(), camera->GetPosition() + camera->GetFrontVector(), camera->GetUpVector());
	shader->SetMat4("view", view);

	// render boxes
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
			angle = glfwGetTime() * 25.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader->SetMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

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

	float cameraSpeed = 2.5f * Time::GetInstance().GetDeltaTime();

	if (glfwGetKey(windowContext, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->Translate(cameraSpeed * camera->GetFrontVector());
	}
	if (glfwGetKey(windowContext, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->Translate(-(cameraSpeed * camera->GetFrontVector()));
	}
	if (glfwGetKey(windowContext, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->Translate(-(glm::normalize(glm::cross(camera->GetFrontVector(), camera->GetUpVector())) * cameraSpeed));
	}
	if (glfwGetKey(windowContext, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->Translate(glm::normalize(glm::cross(camera->GetFrontVector(), camera->GetUpVector())) * cameraSpeed);
	}
}

double System::mouseX = 400;
double System::mouseY = 300;
float System::yaw = -90.f;
float System::pitch = 0.f;
bool System::firstMouse = true;
float System::fov = 45.f;

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

	glfwSetInputMode(windowContext, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//static 함수로 할필요가 있을까
	//뭐 일단 처리하는건 없으니 람다로하고 나중에 처리하자
	glfwSetFramebufferSizeCallback(windowContext, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});

	//이건 람다가 안되네
	glfwSetCursorPosCallback(windowContext, MouseCallback);
	glfwSetScrollCallback(windowContext, ScrollCallback);

	return true;
}