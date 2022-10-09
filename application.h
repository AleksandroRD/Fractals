#pragma once
#include "shader.h"

#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>

class Application {

public:

    Application();
    ~Application();

    int height = 1080;
    int width = 1080;

    void run();

private:

    unsigned int VertexArrayObjects, VertexBufferObjects, ElementBufferObjects;

    GLFWwindow* window;

    Shader shader;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    static void zoomCallback(GLFWwindow* window, double xoffset, double yoffset);

    static void movmentCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void moveCamera(float xoffset, float yoffset);

    void initApplication();

    void zoom(float zoomAmount);

    float zoomAmount = 1.0f;

    float cameraPosX = 0;
    float cameraPosY = 0;

    bool isMoving = false;
};
