#pragma once

#include "gl_wrapper/Source.hpp"

namespace gl_wrapper
{
enum DrawType : GLenum
{
    Dynamic = GL_DYNAMIC_DRAW,        //!< The data store contents will be modified repeatedly and used many times.
    Static = GL_STATIC_DRAW,          //!< The data store contents will be modified once and used many times.
    Stream = GL_STREAM_DRAW,          //!< The data store contents will be modified once and used at most a few times.
};
} // namespace gl_wrapper