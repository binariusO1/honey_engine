#pragma once

#include <libs/glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace he
{
namespace libs
{
namespace gl
{
class Shader
{
public:
    Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath);

    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

    int getId() const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);

private:
    unsigned int m_ID;
    const char* m_vShaderCode;
    const char* m_fShaderCode;
    unsigned int m_vertexShader;
    unsigned int m_fragmentShader;
};
} // namespace render
} // namespace gfx
} // namespace he