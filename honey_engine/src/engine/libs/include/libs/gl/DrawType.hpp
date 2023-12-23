#pragma once

#include <libs/glad/glad.h> // note: it must be higher than other gl libraries

namespace he
{
namespace libs
{
namespace gl
{
enum DrawType : GLenum
{
    Dynamic = GL_DYNAMIC_DRAW,        //!< The data store contents will be modified repeatedly and used many times.
    Static = GL_STATIC_DRAW,          //!< The data store contents will be modified once and used many times.
    Stream = GL_STREAM_DRAW,          //!< The data store contents will be modified once and used at most a few times.
};
} // namespace gl
} // namespace libs
} // namespace he