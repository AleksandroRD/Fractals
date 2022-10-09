#include "shader.h"

void Shader::linkShader(const char* vertex_shader_path, const char* fragment_shader_path)
{
    program_ID = glCreateProgram();

    createShader(program_ID, vertex_shader_path, GL_VERTEX_SHADER);
    createShader(program_ID, fragment_shader_path, GL_FRAGMENT_SHADER);

    glLinkProgram(program_ID);

    int success;
    char error_message[512];
    glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program_ID, 512, nullptr, error_message);
        std::cout << "Error linking shader program: " << error_message << "\n";
    }
}

Shader::~Shader()
{
    if (program_ID != 0)
    {
        glDeleteProgram(program_ID);
        program_ID = 0;
    }
}

void Shader::useShader()
{
    glUseProgram(program_ID);
}


std::string Shader::readFile(const char* file_path)
{
    std::string code;
    std::ifstream shader_file(file_path, std::ios::in);

    if (!shader_file)
    {
        std::cout << "Failed to open shader file: " << file_path << "\n";
        return "";
    }

    std::string line = "";
    while (std::getline(shader_file, line))
    {
        code.append(line + '\n');
    }

    shader_file.close();
    return code;
}

void Shader::createShader(unsigned int program, const char* shader_path, GLenum shader_type)
{
    std::string shader_string = readFile(shader_path);

    const GLchar* code[1]{};
    code[0] = shader_string.c_str();

    GLint code_length[1]{};
    code_length[0] = strlen(shader_string.c_str());

    unsigned int shader;
    int success;

    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, code, code_length);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char error_message[512];
        glGetShaderInfoLog(shader, 512, nullptr, error_message);
        std::cout << "Error compiling shader: " << error_message << "\n";
        std::cout << "Shader location: " << shader_path << "\n";
    }

    glAttachShader(program, shader);
}

void Shader::setUniformShaderFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(program_ID, name.c_str()), value);
}
