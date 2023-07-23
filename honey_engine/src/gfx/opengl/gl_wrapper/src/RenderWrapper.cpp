#include "gl_wrapper/render/RenderWrapper.hpp"
#include "logger/Logger.hpp"
#include <cmath>
namespace
{
float uint8toFloat(const std::uint8_t value)
{
    return static_cast<float>(value)/255.0f;
}
} // namespace

namespace gl_wrapper
{


////////////////////////////////////////////////////////////
RenderWrapper::RenderWrapper()
{
    init();
}


////////////////////////////////////////////////////////////
RenderWrapper::~RenderWrapper()
{
    LOG_DEBUG << "RenderWrapper::~RenderWrapper"; 
}

void RenderWrapper::draw(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount)
{
    // TODO - draw teture, so rectangle here
    // drawPrimitives(type, firstVertex, vertexCount);
}


////////////////////////////////////////////////////////////
void RenderWrapper::draw2dTexture(std::size_t firstVertex, std::size_t vertexCount, const gfx::Vertex2d* vertex, const int textureId, const gfx::render::RenderSettings& renderSettings)
try
{
    static int x = 0;
    if (x == 0)
    {
        LOG_DEBUG << "[LOG] RenderWrapper::draw2d() : " 
        << "firstVertex: " << firstVertex 
        << ", vertexCount: " << vertexCount
        << ", vertex: " << &vertex
        << ", textureId: " << textureId
        << ", shader id: " << renderSettings.shaderProgram.getId();
        x++;
    }

    setupDraw2d(vertex, vertexCount, renderSettings.drawType);

    glBindTexture(GL_TEXTURE_2D, textureId);
    int programShaderId = renderSettings.shaderProgram.getId();

    glUseProgram(programShaderId);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(VAO);
    glDrawArrays(renderSettings.prymitiveType, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount));

    if (renderSettings.polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    clear(programShaderId);
}
catch(std::exception& e)
{
    LOG_ERROR << "Exception! Unable to draw.";
}



////////////////////////////////////////////////////////////
void RenderWrapper::draw2d(std::size_t firstVertex, std::size_t vertexCount, const gfx::Vertex2d* vertex, const gfx::render::RenderSettings& renderSettings)
try
{
    if (false)
    {
        LOG_DEBUG << "[LOG] RenderWrapper::draw2d() : " 
        << "firstVertex: " << firstVertex 
        << ", vertexCount: " << vertexCount
        << ", vertex: " << &vertex
        << ", shader id: " << renderSettings.shaderProgram.getId();
    }

    setupDraw2d(vertex, vertexCount, renderSettings.drawType);

    int programShaderId = renderSettings.shaderProgram.getId();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(programShaderId);
    glBindVertexArray(VAO);
    glDrawArrays(renderSettings.prymitiveType, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount));
    
    if (renderSettings.polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnd();
    clear(programShaderId);
}
catch(std::exception& e)
{
    LOG_ERROR << "Exception! Unable to draw.";
}


////////////////////////////////////////////////////////////
void RenderWrapper::draw2dPrimitives(std::size_t firstVertex, std::size_t vertexCount, const gfx::Vertex2dPrimitive* vertex, const gfx::Color color, const gfx::render::RenderSettings& renderSettings)
try
{
    setupDraw2dPrimitives(vertex, vertexCount, renderSettings.drawType);

    int programShaderId = renderSettings.shaderProgram.getId();
    // LOG_DEBUG << "draw2dPrimitives(), shader id: " << programShaderId;
    glUseProgram(programShaderId);

    int vertexColorLocation = glGetUniformLocation(programShaderId, "PrimitivesColor");

    if (color.a < 255)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glUniform4f(vertexColorLocation, uint8toFloat(color.r), uint8toFloat(color.g), uint8toFloat(color.b), uint8toFloat(color.a));
    glBindVertexArray(VAO);
    glDrawArrays(renderSettings.prymitiveType, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount));

    if (renderSettings.polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnd();
    clear(programShaderId);
}
catch(std::exception& e)
{
    LOG_ERROR << "Exception! Unable to draw.";
}

////////////////////////////////////////////////////////////
void RenderWrapper::flashColor()
{
    // TODO
    /*
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // remember -> if see tutorial if no drawing
    glUniform4f(vertexColorLocation, 0.0f, 0.5f, 0.0f, 0.3f);
    */
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderWrapper::terminate()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


// PRIVATE


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderWrapper::init()
{
    //
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderWrapper::clear(const int& programShaderId)
{
    // TODO: VERY IMPORTANT - handle it by another wrapper, which tells us we need to construct -> descruct
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteProgram(programShaderId);
    //LOG_DEBUG << "after clear()";
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderWrapper::setupDraw2dPrimitives(const gfx::Vertex2dPrimitive* vertex, std::size_t vertexCount, const DrawType drawType)
{
    // TODO reset GL State

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, (vertexCount) * sizeof(gfx::Vertex2dPrimitive), &vertex[0], drawType);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex2dPrimitive), (void*)0);
    glEnableVertexAttribArray(0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderWrapper::setupDraw2d(const gfx::Vertex2d* vertex, std::size_t vertexCount, const DrawType drawType)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, (vertexCount) * sizeof(gfx::Vertex2d), &vertex[0], drawType);

    // vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex2d), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(gfx::Vertex2d), (void*)(2* sizeof(float))); //from (0)
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex2d), (void*)( (2* sizeof(float) ) + (4*sizeof(unsigned char) )));        // 12 = 2*4(float) from (0) + 4*1(char) from (1)
    glEnableVertexAttribArray(2);
}


////////////////////////////////////////////////////////////
void RenderWrapper::setupDraw(const gfx::Vertex2dPrimitive* vertex, std::size_t vertexCount, const DrawType drawType)
{
    // TODO reset GL State

    //createShaderProgram();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    // glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, (vertexCount) * sizeof(gfx::Vertex2dPrimitive), &vertex[0], drawType); // TODO _DRAW as argument

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




    ///////////////////////////  // VERTICIES
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Vertex2dPrimitive), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(gfx::Vertex2dPrimitive), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // glEnableClientState(GL_VERTEX_ARRAY);
    // glVertexPointer(3, GL_FLOAT, sizeof(geometry::Point3Df), &vertex[0]);
}
} // namespace gl_wrapper