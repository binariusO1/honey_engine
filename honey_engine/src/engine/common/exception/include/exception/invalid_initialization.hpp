#pragma once

#include <exception>

namespace he
{
namespace common
{
class invalid_initialization : public std::exception 
{
public:
    invalid_initialization(const char * msg) : message(msg) 
    {
    }
    invalid_initialization() : message(nullptr) 
    {
    }
    const char * what () 
    {
        return message;
    }

 private:
    const char * message;
};
} // namespace common
} // namespace he