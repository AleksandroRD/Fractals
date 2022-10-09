#pragma once
#define GLEW_STATIC

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
public:
    unsigned int program_ID;

    ~Shader();

    void useShader();

    void linkShader(const char* vertex_shader_path, const char* fragment_shader_path);

    void setUniformShaderFloat(const std::string& name, float value) const;

private:
    std::string readFile(const char* file_path);
    void createShader(unsigned int program, const char* shader_path, GLenum shader_type);
};