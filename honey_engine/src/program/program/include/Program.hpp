#pragma once

namespace he
{
class Program
{
public:
    Program();
    Program(const Program&) = delete;
    Program(Program&&) = delete;
    ~Program() = default;

public:
    int run(char* argv[]);
    int runTest(const char* testName);
    
private:
    int startProgram();

private:
    static bool isExists;
};
} // namespace he