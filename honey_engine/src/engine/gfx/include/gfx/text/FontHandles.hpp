#pragma once

#include <memory>
#include "gfx/text/IFreeType2Wrapper.hpp"

namespace he
{
namespace gfx
{
namespace text
{
class Font;

class FontHandles
{
public:
    FontHandles(he::gfx::text::IFreeType2Wrapper& wrapper) : m_freeType2Wrapper{wrapper}
    {
    }

private:
    struct Deleter
    {
        void operator()(FT_Library theLibrary)
        {
            FT_Done_FreeType(theLibrary);
        }
        void operator()(FT_Face theFace)
        {
            FT_Done_Face(theFace);
        }
        void operator()(FT_Stroker theStroker)
        {
            FT_Stroker_Done(theStroker);
        }
    };

public:
    bool initFreeType();
    bool loadFontFace(const std::string& path);
    bool loadStoker();
    bool setUnicodeChar();

private:
    friend class he::gfx::text::Font;
    std::unique_ptr<std::remove_pointer_t<FT_Library>, Deleter> library{nullptr};   //< Pointer to the internal library interface
    std::unique_ptr<FT_StreamRec>                               streamRec{nullptr}; //< Pointer to the stream rec instance
    std::unique_ptr<std::remove_pointer_t<FT_Face>, Deleter>    face{nullptr};      //< Pointer to the internal font face
    std::unique_ptr<std::remove_pointer_t<FT_Stroker>, Deleter> stroker{nullptr};   //< Pointer to the stroker
    he::gfx::text::IFreeType2Wrapper& m_freeType2Wrapper;
};
} // namespace text
} // namespace gfx
} // namespace he