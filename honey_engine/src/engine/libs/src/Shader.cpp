#include "libs/gl/Shader.hpp"
#include "logger/Logger.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
//
/////////////////////////////////////////////////////////////////////////////////////////////

namespace he
{
namespace libs
{
namespace gl
{

////////////////////////////////////////////////////////////
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    m_vShaderCode = vertexCode.c_str();
    m_fShaderCode = fragmentCode.c_str();
    // 2. compile shaders

    // vertex shader
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader, 1, &m_vShaderCode, NULL);
    glCompileShader(m_vertexShader);
    checkCompileErrors(m_vertexShader, "VERTEX");
    // fragment Shader
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader, 1, &m_fShaderCode, NULL);
    glCompileShader(m_fragmentShader);
    checkCompileErrors(m_fragmentShader, "FRAGMENT");
    // shader Program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, m_vertexShader);
    glAttachShader(m_ID, m_fragmentShader);
    glLinkProgram(m_ID);
    checkCompileErrors(m_ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    LOG_INFO << "Create shader program id: " << m_ID;
}


////////////////////////////////////////////////////////////
void Shader::use() 
{ 
    //LOG_DEBUG << "m_ID: " << m_ID << ", m_vertexShader: " << m_vertexShader << ", m_fragmentShader: " << m_fragmentShader;
    glAttachShader(m_ID, m_vertexShader);
    glAttachShader(m_ID, m_fragmentShader);
    glLinkProgram(m_ID);

    // glDeleteShader(m_vertexShader);
    // glDeleteShader(m_fragmentShader);
}


////////////////////////////////////////////////////////////
void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value); 
}


////////////////////////////////////////////////////////////
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value); 
}


////////////////////////////////////////////////////////////
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value); 
}


////////////////////////////////////////////////////////////
int Shader::getId() const
{
    return m_ID;
}

 // private:


////////////////////////////////////////////////////////////
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

} // namespace render
} // namespace gfx
} // namespace he