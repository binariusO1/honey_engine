#include "sys/ImageLoader.hpp"

#include "libs/stb/stb_image.h"
#include "sys/honey_engine.h"
#include "logger/Logger.hpp"

namespace
{
inline std::string formatDebugPathInfo(const std::filesystem::path& path)
{
    std::ostringstream stream;
    stream << "    Provided path: " << path << '\n';
    stream << "    Absolute path: " << std::filesystem::absolute(path);
    return stream.str();
}
} // namespace

namespace he
{
namespace sys
{
////////////////////////////////////////////////////////////
ImageLoader& ImageLoader::getInstance()
{
    static ImageLoader instance;

    return instance;
}


////////////////////////////////////////////////////////////
bool ImageLoader::loadImageFromFile(const std::filesystem::path& filename, std::vector<std::uint8_t>& pixels, int& width, int& height)
{
    pixels.clear();
    int channels = 0;
    const std::string absolutePath{getAbsolutePath(filename.string().c_str())};

    unsigned char* data = loadImage(absolutePath, width, height, channels);
    if (not data)
    {
        printError(absolutePath);
        return false;
    }
    if (width > 0 and height > 0)
    {
        pixels.resize(static_cast<std::size_t>(width * height * 4));
        memcpy(pixels.data(), data, pixels.size());
    }
    else
    {
        LOG_ERROR << "Cannot load image becase size is wrong: [" << width << "x" << height << "]";
        return false;
    }
    LOG_DEBUG << "Load image: " << "[" << width << "x" << height << "]" << " from: " << absolutePath;
    freeMemory(data);
    return true;
}


////////////////////////////////////////////////////////////
const std::string ImageLoader::getAbsolutePath(const std::string& path)
{
   std::string prefix{TODO__DATA_PATH_PREFIX};
   return prefix + path;
}


////////////////////////////////////////////////////////////
unsigned char* ImageLoader::loadImage(const std::string& absolutePath, int& width, int& height, int& channels)
{
    stbi_set_flip_vertically_on_load(true);
    // note: 4 = STBI_rgb_alpha
    return stbi_load(absolutePath.c_str(), &width, &height, &channels, 4);
}


////////////////////////////////////////////////////////////
void ImageLoader::printError(const std::filesystem::path& path)
{
    if (not std::filesystem::exists(path))
    {
        LOG_ERROR << "Failed to load image from: " << formatDebugPathInfo(path) << ", Reason: " << "Wrong path or file does't exist!";
    }
    else
    {
        LOG_ERROR << "Failed to load image from: " << formatDebugPathInfo(path) << ", Reason: " << stbi_failure_reason();
    }
}


////////////////////////////////////////////////////////////
void ImageLoader::freeMemory(unsigned char* data)
{
    stbi_image_free(data);
    LOG_DEBUG << "Thank you for release memory [stbi--]";
}
} // namespace he
} // namespace sys