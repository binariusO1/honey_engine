#pragma once

#include <libs/glad/glad.h> // note: it must be higher than other gl libraries

namespace he
{
namespace libs
{
namespace gl
{
enum ConnectionType : GLenum
{
    Points = GL_POINTS,                 //!< List of individual points
    Lines = GL_LINES,                   //!< List of individual lines
    LineStrip = GL_LINE_STRIP,          //!< List of connected lines, a point uses the previous point to form a line
    Triangles = GL_TRIANGLES,           //!< List of individual triangles
    TriangleStrip = GL_TRIANGLE_STRIP,  //!< List of connected triangles, a point uses the two previous points to form a triangle
    TriangleFan = GL_TRIANGLE_FAN       //!< List of connected triangles, a point uses the common center and the previous point to form a triangle
};
} // namespace gl
} // namespace libs
} // namespace he