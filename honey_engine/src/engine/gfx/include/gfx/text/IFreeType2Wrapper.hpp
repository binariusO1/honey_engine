#pragma once

#include <ft2build.h>
#include <string>
#include FT_BITMAP_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H

namespace he
{
namespace gfx
{
namespace text
{
class IFreeType2Wrapper
{
public:
    virtual ~IFreeType2Wrapper() = default;

public:
    virtual void doneGlyph(FT_Glyph glyph) = 0;
    virtual void strokerSet(FT_Stroker stroker, FT_Fixed radius, FT_Stroker_LineCap line_cap, FT_Stroker_LineJoin line_join, FT_Fixed miter_limit) = 0;
    virtual int setPixelSizes(FT_Face face, FT_UInt pixel_width, FT_UInt pixel_height) = 0;
    virtual bool isScalable(FT_Face face) = 0;
    virtual bool hasKerning(FT_Face face) = 0;
    virtual const unsigned int getCharIndex(FT_Face face, FT_ULong charcode) = 0;
    virtual const signed long multiplierFix(FT_Long a, FT_Long b) = 0;
    virtual const int loadChar(FT_Face face, FT_ULong char_code, FT_Int32 load_flags) = 0;
    virtual const int getGlyph(FT_GlyphSlot slot, FT_Glyph *aglyph) = 0;
    virtual const int glyphToBitmap(FT_Glyph* the_glyph, FT_Render_Mode render_mode, const FT_Vector* origin, FT_Bool destroy) = 0;
    virtual const int bitmapEmbolden(FT_Library library, FT_Bitmap* bitmap, FT_Pos xStrength, FT_Pos yStrength) = 0;
    virtual const int outlineEmbolden(FT_Outline* outline, FT_Pos strength) = 0;
    virtual const int glyphStroke(FT_Glyph* pglyph, FT_Stroker stroker, FT_Bool destroy) = 0;
    virtual const int newFace(FT_Library library, const char* filepathname, FT_Long face_index, FT_Face *aface ) = 0;
    virtual const int strokerNew(FT_Library library, FT_Stroker* astroker) = 0;
    virtual const int selectCharmap(FT_Face face, FT_Encoding encoding) = 0;
    virtual const int initFreeType(FT_Library* alibrary) = 0;
    virtual const int getKerning(FT_Face face, FT_UInt left_glyph, FT_UInt right_glyph, FT_UInt kern_mode, FT_Vector *akerning) = 0;
                
public:
    virtual const int getErrorCode() const = 0;
    virtual void setErrorCode(const int errorCode) = 0;
    virtual const std::string getErrorDesc() const = 0;
};
} // namespace text
} // namespace gfx
} // namespace he