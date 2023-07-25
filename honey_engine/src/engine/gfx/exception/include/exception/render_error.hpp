#pragma once

namespace gfx
{
class render_error : public std::exception 
{
public:
    render_error(const char * msg) : message(msg) 
    {
    }
    render_error() : message(nullptr) 
    {
    }
    const char * what () 
    {
        return message;
    }

 private:
    const char * message;
};
} // namespace gfx