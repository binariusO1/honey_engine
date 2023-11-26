#pragma once

#include "gfx/geometry/Point2d.hpp"
#include "gfx/text/Page.hpp"

namespace he
{
namespace gfx
{
namespace text
{
class TextFixture
{
public:

const char* t_fontCalibriPath = "\\data\\gfx\\fonts\\calibri.ttf";
const unsigned int t_defaultCharacterSize{26};
const geometry::Point2Df t_point1{0.f, 5.f};
const geometry::Point2Df t_point2{4.f, 6.f}; 
const geometry::Point2Df t_point3{1.f, 3.f};
const geometry::Point2Df t_point4{2.f, 8.f}; 
const geometry::Line<float> t_line1{t_point1, t_point2};
const geometry::Line<float> t_line2{t_point3, t_point4};
const he::gfx::text::Glyph t_glyph1{1.1f, 1, 11, t_line1, t_line2};
const he::gfx::text::Glyph t_glyph2{1.2f, 102, 22, t_line1, t_line2};
const he::gfx::text::Row t_row1{1, 2};
const he::gfx::text::Row t_row2{2, 4};
const he::gfx::text::Row t_row3{4, 6};
};
} // namespace text
} // namespace gfx
} // namespace he