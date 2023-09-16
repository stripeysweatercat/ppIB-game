#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
public:
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    unsigned int ID;

    Shader(const char* filepath, const char* name);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetBool(const std::string& name, bool value);
    void UseShader();
    void DeleteShader();
};