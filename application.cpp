#include "application.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <future>

#include "debug.h"


Application::Application() {
	initApplication();
}

Application::~Application() {
	glDeleteVertexArrays(1, &VertexArrayObjects);
	glDeleteBuffers(1, &VertexBufferObjects);
	glDeleteBuffers(1, &ElementBufferObjects);

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
		glBindVertexArray(VertexArrayObjects);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

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

void Application::movmentCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	auto application = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

	float zoom = application->zoomAmount;
	switch (key)
	{
	case GLFW_KEY_RIGHT:
		application->cameraPosX += (0.01 * zoom);
		application->moveCamera(application->cameraPosX, application->cameraPosY);
		break;
	case GLFW_KEY_LEFT:
		application->cameraPosX -= (0.01 * zoom);
		application->moveCamera(application->cameraPosX, application->cameraPosY);
		break;
	case GLFW_KEY_DOWN:
		application->cameraPosY -= (0.01 * zoom);
		application->moveCamera(application->cameraPosX, application->cameraPosY);
		break;
	case GLFW_KEY_UP:
		application->cameraPosY += (0.01 * zoom);
		application->moveCamera(application->cameraPosX, application->cameraPosY);
		break;
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
	glfwSetKeyCallback(window, movmentCallback);

	glGenVertexArrays(1, &VertexArrayObjects);
	glGenBuffers(1, &VertexBufferObjects);
	glGenBuffers(1, &ElementBufferObjects);

	glBindVertexArray(VertexArrayObjects);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObjects);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObjects);
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
		zoomAmount += 0.02;
		shader.setUniformShaderFloat("zoom", zoomAmount);
	}
	else {
		zoomAmount -= 0.02;
		shader.setUniformShaderFloat("zoom", zoomAmount);
	}

}

void Application::moveCamera(float xoffset, float yoffset) {

	shader.setUniformShaderFloat("xoffset", xoffset);
	shader.setUniformShaderFloat("yoffset", yoffset);
}
