#include "shader.h"

Shader::Shader(const char* filepath, const char* name)
{
    std::string vertexCode, fragmentCode;
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    unsigned int type2;

    try {
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                    type2 = GL_VERTEX_SHADER;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                    type2 = GL_FRAGMENT_SHADER;
                }
            }
            else
            {
                ss[(int)type] << line << "\n";
            }
        }
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR: Shader" << name << "Unsuccessfully Parsed" << std::endl;
    }

    vertexCode = ss[0].str();
    fragmentCode = ss[1].str();

    const char* vertexCodeCompile = vertexCode.c_str();
    const char* fragmentCodeCompile = fragmentCode.c_str();

    ID = glCreateProgram();
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCodeCompile, nullptr);
    glCompileShader(vertex);

    int result = 10;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(vertex, length, &length, message);
        std::cout << "Failed to compile " << name << " vertex shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(vertex);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCodeCompile, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(fragment, length, &length, message);
        std::cout << "Failed to compile " << name << " fragment shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(fragment);
    }

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glValidateProgram(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::SetInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), (float)value);
}

void Shader::SetBool(const std::string & name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (bool)value);
}

void Shader::UseShader() 
{
    glUseProgram(ID);
}

void Shader::DeleteShader()
{
    glDeleteShader(ID);
}