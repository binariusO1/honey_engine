#pragma once

#include <gmock/gmock-function-mocker.h>
#include "gfx/text/IFreeType2Wrapper.hpp"

namespace he
{
namespace gfx
{
namespace text
{
class FreeType2Mock : public IFreeType2Wrapper
{ 
public:
    FreeType2Mock() = default;
    ~FreeType2Mock() override = default;

    MOCK_METHOD(void, doneGlyph, (FT_Glyph), (override));
    MOCK_METHOD(void, strokerSet, (FT_Stroker, FT_Fixed, FT_Stroker_LineCap, FT_Stroker_LineJoin, FT_Fixed), (override));
    MOCK_METHOD(int, setPixelSizes, (FT_Face, FT_UInt, FT_UInt), (override));
    MOCK_METHOD(bool, isScalable, (FT_Face), (override));
    MOCK_METHOD(bool, hasKerning, (FT_Face), (override));
    MOCK_METHOD(const unsigned int, getCharIndex, (FT_Face, FT_ULong), (override));
    MOCK_METHOD(const signed long, multiplierFix, (FT_Long, FT_Long), (override));
    MOCK_METHOD(const int, loadChar, (FT_Face, FT_ULong, FT_Int32), (override));
    MOCK_METHOD(const int, getGlyph, (FT_GlyphSlot, FT_Glyph*), (override));
    MOCK_METHOD(const int, glyphToBitmap, (FT_Glyph*, FT_Render_Mode, const FT_Vector*, FT_Bool), (override));
    MOCK_METHOD(const int, bitmapEmbolden, (FT_Library, FT_Bitmap*, FT_Pos, FT_Pos), (override));
    MOCK_METHOD(const int, outlineEmbolden, (FT_Outline*, FT_Pos), (override));
    MOCK_METHOD(const int, glyphStroke, (FT_Glyph*, FT_Stroker, FT_Bool), (override));
    MOCK_METHOD(const int, newFace, (FT_Library, const char*, FT_Long, FT_Face*), (override));
    MOCK_METHOD(const int, strokerNew, (FT_Library, FT_Stroker*), (override));
    MOCK_METHOD(const int, selectCharmap, (FT_Face, FT_Encoding), (override));
    MOCK_METHOD(const int, initFreeType, (FT_Library*), (override));
    MOCK_METHOD(const int, getKerning, (FT_Face, FT_UInt, FT_UInt, FT_UInt, FT_Vector*), (override));

    MOCK_METHOD(const int, getErrorCode, (), (const override));
    MOCK_METHOD(void, setErrorCode, (const int), (override));
    MOCK_METHOD(const std::string, getErrorDesc, (), (const override));
};
} // namespace text
} // namespace gfx
} // namespace he