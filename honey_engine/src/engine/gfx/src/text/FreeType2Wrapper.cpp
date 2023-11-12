#include "gfx/text/FreeType2Wrapper.hpp"

namespace he::gfx::text
{
////////////////////////////////////////////////////////////
void FreeType2Wrapper::strokerSet(FT_Stroker stroker, FT_Fixed radius, FT_Stroker_LineCap line_cap, FT_Stroker_LineJoin line_join, FT_Fixed miter_limit)
{
    FT_Stroker_Set(stroker, radius, line_cap, line_join, miter_limit);
}


////////////////////////////////////////////////////////////
int FreeType2Wrapper::setPixelSizes(FT_Face face, FT_UInt pixel_width, FT_UInt pixel_height)
{
    return FT_Set_Pixel_Sizes(face, pixel_width, pixel_height);
}


////////////////////////////////////////////////////////////
bool FreeType2Wrapper::isScalable(FT_Face face)
{
    return FT_IS_SCALABLE(face);
}


////////////////////////////////////////////////////////////
bool FreeType2Wrapper::hasKerning(FT_Face face)
{
    return FT_HAS_KERNING(face);
}


////////////////////////////////////////////////////////////
const unsigned int FreeType2Wrapper::getCharIndex(FT_Face face, FT_ULong charcode)
{
    return FT_Get_Char_Index(face, charcode);
}


////////////////////////////////////////////////////////////
const signed long FreeType2Wrapper::multiplierFix(FT_Long a, FT_Long b)
{
    return FT_MulFix(a, b);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::loadChar(FT_Face face, FT_ULong char_code, FT_Int32 load_flags)
{
    return FT_Load_Char(face, char_code, load_flags);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::getGlyph(FT_GlyphSlot slot, FT_Glyph *aglyph)
{
    return FT_Get_Glyph(slot, aglyph);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::glyphToBitmap(FT_Glyph* the_glyph, FT_Render_Mode render_mode, const FT_Vector* origin, FT_Bool destroy)
{
    return FT_Glyph_To_Bitmap(the_glyph, render_mode, origin, destroy);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::bitmapEmbolden(FT_Library library, FT_Bitmap* bitmap, FT_Pos xStrength, FT_Pos yStrength)
{
    return FT_Bitmap_Embolden(library, bitmap, xStrength, yStrength);
}


////////////////////////////////////////////////////////////
void FreeType2Wrapper::doneGlyph(FT_Glyph glyph)
{
    FT_Done_Glyph(glyph);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::outlineEmbolden(FT_Outline* outline, FT_Pos strength)
{
    return FT_Outline_Embolden(outline, strength);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::glyphStroke(FT_Glyph* pglyph, FT_Stroker stroker, FT_Bool destroy)
{
    return FT_Glyph_Stroke(pglyph, stroker, destroy);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::newFace(FT_Library library, const char* filepathname, FT_Long face_index, FT_Face *aface)
{
    return FT_New_Face(library, filepathname, face_index, aface);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::strokerNew(FT_Library library, FT_Stroker* astroker)
{
    return FT_Stroker_New(library, astroker);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::selectCharmap(FT_Face face, FT_Encoding encoding)
{
    return FT_Select_Charmap(face, encoding);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::getKerning(FT_Face face, FT_UInt left_glyph, FT_UInt right_glyph, FT_UInt kern_mode, FT_Vector* akerning)
{
    return FT_Get_Kerning(face, left_glyph, right_glyph, kern_mode, akerning);
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::getErrorCode() const
{
    return m_errorCode;
}


////////////////////////////////////////////////////////////
void FreeType2Wrapper::setErrorCode(const int errorCode)
{
    m_errorCode = errorCode;
}


////////////////////////////////////////////////////////////
const int FreeType2Wrapper::initFreeType(FT_Library* alibrary)
{
    return FT_Init_FreeType(alibrary);
}


//////////////////////////////////////////////////////////////////////
const std::string FreeType2Wrapper::getErrorDesc() const
{
    switch (m_errorCode)
    {
        case FT_Err_Cannot_Open_Resource:
            return "cannot open resource";
        case FT_Err_Unknown_File_Format:
            return "unknown file format";
        case FT_Err_Invalid_File_Format:
            return "broken file";
        case 1001:
            return "wrong file path string";
        default:
            return "Unknown";
    }
}
} // namespace he::gfx::text