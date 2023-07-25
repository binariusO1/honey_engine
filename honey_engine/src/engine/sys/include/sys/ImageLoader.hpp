#pragma once

#include <filesystem>
#include <vector>

namespace he
{
namespace sys
{
class ImageLoader
{
public:
    ImageLoader(const ImageLoader&) = delete;
    ImageLoader(const ImageLoader&&) = delete;
    static ImageLoader& getInstance();
    ~ImageLoader() = default;

public:
    bool loadImageFromFile(const std::filesystem::path& filename, std::vector<std::uint8_t>& pixels, int& width, int& height);

private:
    ImageLoader() = default;

private:
    const std::string getAbsolutePath(const std::string&);
    unsigned char* loadImage(const std::string& absolutePath, int& width, int& height, int& channels);
    void printError(const std::filesystem::path& path);
    void freeMemory(unsigned char* data);
};

} // namespace he
} // namespace sys
