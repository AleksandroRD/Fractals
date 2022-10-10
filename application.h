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

    unsigned int vertexArrayObjects, vertexBufferObjects, elementBufferObjects;

    GLFWwindow* window;

    Shader shader;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    static void zoomCallback(GLFWwindow* window, double xoffset, double yoffset);

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    void moveCamera();

    void initApplication();

    void zoom(float zoomAmount);

    float scale = 1.0f;

    float xoffset, yoffset;

    double mousePosX = 0, mousePosY = 0;

    bool isMoved = false;
};
