#pragma once

#include "gfx/text/IFreeType2Wrapper.hpp"

namespace he
{
namespace gfx
{
namespace text
{
class FreeType2Wrapper : public IFreeType2Wrapper
{
public:
    ~FreeType2Wrapper() override = default;

public:
    void doneGlyph(FT_Glyph glyph) override;
    void strokerSet(FT_Stroker stroker, FT_Fixed radius, FT_Stroker_LineCap line_cap, FT_Stroker_LineJoin line_join, FT_Fixed miter_limit) override;
    int setPixelSizes(FT_Face face, FT_UInt pixel_width, FT_UInt pixel_height) override;
    bool isScalable(FT_Face face) override;
    bool hasKerning(FT_Face face) override;
    const unsigned int getCharIndex(FT_Face face, FT_ULong charcode) override;
    const signed long multiplierFix(FT_Long a, FT_Long b) override;
    const int loadChar(FT_Face face, FT_ULong char_code, FT_Int32 load_flags) override;
    const int getGlyph(FT_GlyphSlot slot, FT_Glyph *aglyph) override;
    const int glyphToBitmap(FT_Glyph* the_glyph, FT_Render_Mode render_mode, const FT_Vector* origin, FT_Bool destroy) override;
    const int bitmapEmbolden(FT_Library library, FT_Bitmap* bitmap, FT_Pos xStrength, FT_Pos yStrength) override;
    const int outlineEmbolden(FT_Outline* outline, FT_Pos strength) override;
    const int glyphStroke(FT_Glyph* pglyph, FT_Stroker stroker, FT_Bool destroy) override;
    const int newFace(FT_Library library, const char* filepathname, FT_Long face_index, FT_Face *aface ) override;
    const int strokerNew(FT_Library library, FT_Stroker* astroker) override;
    const int selectCharmap(FT_Face face, FT_Encoding encoding) override;
    const int initFreeType(FT_Library* alibrary) override;
    const int getKerning(FT_Face face, FT_UInt left_glyph, FT_UInt right_glyph, FT_UInt kern_mode, FT_Vector *akerning) override;

public:
    const int getErrorCode() const override;
    void setErrorCode(const int errorCode) override;
    const std::string getErrorDesc() const override;

private:
    FT_Error m_errorCode{0};
};
} // namespace text
} // namespace gfx
} // namespace he