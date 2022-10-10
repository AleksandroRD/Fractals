#include "application.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <future>
#include <math.h>

#include "debug.h"


Application::Application() {
	initApplication();
}

Application::~Application() {
	glDeleteVertexArrays(1, &vertexArrayObjects);
	glDeleteBuffers(1, &vertexBufferObjects);
	glDeleteBuffers(1, &elementBufferObjects);

	glfwTerminate();
}

float vertices[] =
{
	//    x      y      z   
		-1.0f, -1.0f, -0.0f,
		 1.0f,  1.0f, -0.0f,
		-1.0f,  1.0f, -0.0f,
		 1.0f, -1.0f, -0.0f
};

unsigned int indices[] =
{
	//  2---,1
	//  | .' |
	//  0'---3
		0, 1, 2,
		0, 3, 1
};

void Application::run() {
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.useShader();
		glBindVertexArray(vertexArrayObjects);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		if (isMoved) {
			moveCamera();
		}
		glfwGetCursorPos(window, &mousePosX, &mousePosY);
	}
}

void Application::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::zoomCallback(GLFWwindow* window, double xoffset, double yoffset) {

	auto appliction = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

	appliction->zoom(yoffset);
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	auto application = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		switch (action)
		{
		case GLFW_PRESS:
			application->isMoved = true;
			break;
		case GLFW_RELEASE:
			application->isMoved = false;
			//glfwGetCursorPos(window, &application->mousePosX, &application->mousePosY);
			break;
		}
	}
}

void Application::initApplication() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Fractals", NULL, NULL);

	glfwSetWindowUserPointer(window, this);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (glewInit())
	{
		std::cout << "Failed initializing GLEW\n";
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetScrollCallback(window, zoomCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glGenVertexArrays(1, &vertexArrayObjects);
	glGenBuffers(1, &vertexBufferObjects);
	glGenBuffers(1, &elementBufferObjects);

	glBindVertexArray(vertexArrayObjects);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjects);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	shader.linkShader("shaders/mandelbrotSet.vert", "shaders/mandelbrotSet.frag");
}

void Application::zoom(float direction) {
	if (direction < 0) {
		scale += 0.015;
		shader.setUniformShaderFloat("zoom", scale);
	}
	else {
		scale -= 0.015;
		shader.setUniformShaderFloat("zoom", scale);
	}
}

void Application::moveCamera() {
	double currentPosX , currentPosY;
	float differenceX, differenceY;
	glfwGetCursorPos(window, &currentPosX, &currentPosY);
	
	differenceX = currentPosX - mousePosX;
	differenceY = currentPosY - mousePosY;

	xoffset -= differenceX / 1.0f / width;
	yoffset += differenceY / 1.0f / height;
	shader.setUniformShaderFloat("xoffset", xoffset);
	shader.setUniformShaderFloat("yoffset", yoffset);
}
