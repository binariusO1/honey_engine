#pragma once

#include <filesystem>
#include <memory>
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/draw/IDrawable.hpp"
#include "gfx/geometry/transform/Transformable2d.hpp"
#include "gfx/text/Font.hpp"
#include "gfx/Vertex.hpp"

namespace he
{
namespace gfx
{
namespace text
{
} // namespace text
namespace draw
{
class Text : public IDrawable , protected he::gfx::geometry::transform::Transformable2d
{
public:
    enum Style
    {
        Regular       = 0,      //!< Regular characters, no style
        Bold          = 1 << 0, //!< Bold characters
        Italic        = 1 << 1, //!< Italic characters
        Underlined    = 1 << 2, //!< Underlined characters
        StrikeThrough = 1 << 3  //!< Strike through characters
    };

public:
    Text(const std::string&);
    Text(const Text& copy);
    Text(Text&& copy) = default;
    ~Text() override;

public:
    void setString(const std::string& string);
    const std::string& getString() const;
    void setFont(const text::Font& font);
    void setFont(const std::filesystem::path& filepath);
    void setStyle(const std::uint32_t style);
    const std::uint32_t getStyle() const;

public:
    void setColor(const he::gfx::Color&) override;
    void setOrigin(const he::gfx::geometry::Point2Df&) override;
    void setOriginInCenter() override;
    void setPosition(const he::gfx::geometry::Point2Df&) override;
    void setRotation(const he::gfx::geometry::Angle&) override {};
    he::gfx::Color getColor() const override;
    const std::string getName() const override { return m_context.name; };
    const he::gfx::geometry::Point2Df& getOrigin() const override { return Transformable2d::getOrigin(); };
    const he::gfx::geometry::Point2Df& getPosition() const override;
    const he::gfx::geometry::Angle& getRotation() const override { return Transformable2d::getRotation(); };

    unsigned int getTextureId() const override;
    const he::gfx::VertexArray2d& getVertexArray() const override;

private:
    void updateVertexArray();
    void computeTextStyle();
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&) const override;
    void addLine(gfx::VertexArray2d& vertices, const gfx::Color& color,
             float            lineLength,
             float            lineTop,
             float            offset,
             float            thickness,
             float            outlineThickness = 0);
    void addGlyphQuad(gfx::VertexArray2d& vertices, const geometry::Point2Df& position, const gfx::Color& color, const gfx::text::Glyph& glyph, float italicShear);

private:
    IDrawable::Context m_context;
    std::string m_string;
    he::gfx::geometry::Line<float> m_bounds;
    std::shared_ptr<gfx::text::Font> m_font{nullptr};
    unsigned int m_fontTextureId{0};
    std::uint32_t m_style{Regular};             //!< Text style (see Style enum)
    gfx::VertexArray2d m_vertexArray{};        //!< Vertex array containing the fill geometry
    gfx::VertexArray2d m_outlineVertices{}; //!< Vertex array containing the outline geometry
    float m_outlineThickness{0.f};                     //!< Thickness of the text's outline
    gfx::Color                              m_outlineColor{Color::Black};                //!< Text outline color
    float                                   m_letterSpacingFactor{1.f};                  //!< Spacing factor between letters
    float                                   m_lineSpacingFactor{1.f};                    //!< Spacing factor between lines
    mutable bool m_geometryNeedUpdate{false};
    unsigned int m_characterSize{0};       ///< Base size of characters, in pixels
    bool m_originInCenter{false};
};
} // namespace draw
} // namespace gfx
} // namespace he