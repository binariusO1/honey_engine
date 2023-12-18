#pragma once

#include <filesystem>
#include "gfx/geometry/Line.hpp"
#include "gfx/draw/IShape.hpp"
#include "gfx/text/Style.hpp"
#include "gfx/Vertex.hpp"

namespace he
{
namespace gfx
{
namespace text
{
class IFont;
class Font;
struct Glyph;
} // namespace text
namespace draw
{
class Text : public IShape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>
{
public:
    Text(const std::string& name);
    Text(const Text& copy);
    Text(Text&& copy) = default;
    ~Text() override;

public:
    void setString(const std::string& string);
    const std::string& getString() const;
    void setFont(const text::Font& font);
    void setFont(const std::filesystem::path& filepath);
    void setStyle(const text::Style style);
    text::Style getStyle() const;
    void setCharacterSize(const unsigned int);
    const he::gfx::geometry::Size2Df getLocalBounds() const;

public:
    void setColor(const he::gfx::Color&) override;
    bool setOrigin(const he::gfx::geometry::Point2Df&) override;
    void setOriginInCenter() override;
    bool setPosition(const he::gfx::geometry::Point2Df&) override;
    bool setRotation(const he::gfx::geometry::Angle&, const int axis = 0) override;
    const he::gfx::Color getColor() const override;
    const std::string getName() const override { return m_context.name; };
    const he::gfx::geometry::Point2Df& getOrigin() const override;
    const he::gfx::geometry::Point2Df& getPosition() const override;
    const he::gfx::geometry::Angle& getRotation(const int axis = 0) const override;

    unsigned int getTextureId() const;
    const he::gfx::VertexArray2d& getVertexArray() const override;
    void update();

private:
    void updateVertexArray() override;

private:
    void computeTextStyle();
    void addLine(gfx::VertexArray2d& vertices, const gfx::Color& color,
             float            lineLength,
             float            lineTop,
             float            offset,
             float            thickness,
             float            outlineThickness = 0);
    void addGlyphQuad(gfx::VertexArray2d& vertices, const geometry::Point2Df& position, const gfx::Color& color, const gfx::text::Glyph& glyph, float italicShear);
    void updateLocalBounds(he::gfx::geometry::Line<float>, const float, const float);
    
protected:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&, render::TransformMatrix&) override;
    virtual void createNewFont(const text::Font& font);

protected:
    std::shared_ptr<gfx::text::IFont> m_font{nullptr};

private:
    std::string m_string;
    he::gfx::geometry::Size2Df m_bounds;
    unsigned int m_fontTextureId{0};
    text::Style m_style{gfx::text::FontStyle::Regular};             //!< Text style (see Style enum)
    gfx::VertexArray2d m_outlineVertices{}; //!< Vertex array containing the outline geometry
    float m_outlineThickness{0.f};                     //!< Thickness of the text's outline
    gfx::Color m_outlineColor{Color::Black};                //!< Text outline color
    float m_letterSpacingFactor{1.f};                  //!< Spacing factor between letters
    float m_lineSpacingFactor{1.f};                    //!< Spacing factor between lines
};
} // namespace draw
} // namespace gfx
} // namespace he