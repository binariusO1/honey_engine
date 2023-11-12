#include "gfx/text/FontHandles.hpp"

namespace he::gfx::text
{
////////////////////////////////////////////////////////////
bool FontHandles::initFreeType()
{
    FT_Library ftLibrary;
    auto errorCode = m_freeType2Wrapper.initFreeType(&ftLibrary);
    m_freeType2Wrapper.setErrorCode(errorCode);

    if (errorCode == 0)
    {
        library.reset(ftLibrary);
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
bool FontHandles::loadFontFace(const std::string& path)
{
    // Note: Load the new font face from the specified file
    FT_Face ftFace;
    auto errorCode = m_freeType2Wrapper.newFace(library.get(), path.c_str(), 0, &ftFace);
    m_freeType2Wrapper.setErrorCode(errorCode);

    if (errorCode == 0)
    {
        face.reset(ftFace);
        return true;    
    }

    return false;
}


////////////////////////////////////////////////////////////
bool FontHandles::loadStoker()
{
    // Note: Load the stroker that will be used to outline the font
    FT_Stroker ftStroker;
    auto errorCode = m_freeType2Wrapper.strokerNew(library.get(), &ftStroker);
    m_freeType2Wrapper.setErrorCode(errorCode);

    if (errorCode == 0)
    {
        stroker.reset(ftStroker);
        return true;    
    }

    return false;
}


////////////////////////////////////////////////////////////
bool FontHandles::setUnicodeChar()
{
    // Note: Select the unicode character map
    auto errorCode = m_freeType2Wrapper.selectCharmap(face.get(), FT_ENCODING_UNICODE);
    m_freeType2Wrapper.setErrorCode(errorCode);

    if (errorCode == 0)
    {
        return true;
    }

    return false;
}
} // namespace he::gfx::text