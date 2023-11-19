#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/draw/Shape.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/draw/Text.hpp"

using namespace ::testing;

namespace  he::gfx
{

namespace
{
} // namespace
    
class TextSCT : public testing::Test , public RenderFixture
{
public:
    TextSCT()
    {
    }

    void setDaultTextObject()
    {
        t_text1 = std::make_shared<he::gfx::draw::Text>("text1");
        t_text1->setFont(t_fontCalibriPath);
        t_text1->setString("Test"); 
        t_text1->setCharacterSize(40); 
    }

    ~TextSCT() = default;

    std::shared_ptr<he::gfx::draw::Text> t_text1;
};

TEST_F(TextSCT, textDraw_shouldDrawTextInDefaultPosition)
{
    createCustomScreen();
    setDaultTextObject();

    auto textBounds = t_text1->getLocalBounds();
    he::gfx::geometry::figures::Rectangle backgroundRectangle(he::gfx::geometry::Size2Df{textBounds.p2.x*2, textBounds.p2.y*2});
    he::gfx::geometry::figures::Rectangle boundsRectangle(he::gfx::geometry::Size2Df{textBounds.p2.x, textBounds.p2.y});
    he::gfx::draw::Shape shape1("backgroundRectangle", backgroundRectangle);
    he::gfx::draw::Shape shape2("boundsRectangle", boundsRectangle);
    shape1.setColor(gfx::Color::Red);
    shape2.setColor(gfx::Color::Black);

    addDrawableToMainLayer(std::make_shared<he::gfx::draw::Shape>(shape1));
    addDrawableToMainLayer(std::make_shared<he::gfx::draw::Shape>(shape2));
    addDrawableToMainLayer(t_text1);

    display(200);
}

TEST_F(TextSCT, textDraw_shouldDrawTextInDefaultPositionAfterSetNewCharacterSize)
{
    createCustomScreen();
    setDaultTextObject();

    auto textBounds = t_text1->getLocalBounds();
    he::gfx::geometry::figures::Rectangle backgroundRectangle(he::gfx::geometry::Size2Df{textBounds.p2.x*2, textBounds.p2.y*2});
    he::gfx::geometry::figures::Rectangle boundsRectangle(he::gfx::geometry::Size2Df{textBounds.p2.x, textBounds.p2.y});
    he::gfx::draw::Shape shape1("backgroundRectangle", backgroundRectangle);
    he::gfx::draw::Shape shape2("boundsRectangle", boundsRectangle);
    shape1.setColor(gfx::Color::Red);
    shape2.setColor(gfx::Color::Black);

    addDrawableToMainLayer(std::make_shared<he::gfx::draw::Shape>(shape1));
    addDrawableToMainLayer(std::make_shared<he::gfx::draw::Shape>(shape2));
    addDrawableToMainLayer(t_text1);

    display(50);
    t_text1->setCharacterSize(0);
    display(50);
    t_text1->setCharacterSize(20);
    display(50);
    t_text1->setCharacterSize(40);
    display(50);
}

TEST_F(TextSCT, textDraw_shouldDrawTextInMovedPosition)
{
    createCustomScreen();
    setDaultTextObject();

    auto textBounds = t_text1->getLocalBounds();
    he::gfx::geometry::figures::Rectangle quartRectangle(he::gfx::geometry::Size2Df{defaultWindowWidth/2.f, defaultWindowHeight/2.f});
    he::gfx::geometry::figures::Rectangle backgroundRectangle(he::gfx::geometry::Size2Df{textBounds.p2.x*2, textBounds.p2.y*2});
    he::gfx::geometry::figures::Rectangle boundsRectangle(he::gfx::geometry::Size2Df{textBounds.p2.x, textBounds.p2.y});
    he::gfx::draw::Shape shapeBackgroundRectangle("backgroundRectangle", backgroundRectangle);
    he::gfx::draw::Shape shapeBoundsRectangle("boundsRectangle", boundsRectangle);
    he::gfx::draw::Shape shapeQuartRectangle("quartRectangle", quartRectangle);
    shapeBackgroundRectangle.setColor(gfx::Color::Red);
    shapeBoundsRectangle.setColor(gfx::Color::Black);
    shapeQuartRectangle.setColor(gfx::Color::Red);

    t_text1->setPosition({defaultWindowWidth/2.f, defaultWindowHeight/2.f});
    shapeBoundsRectangle.setPosition({defaultWindowWidth/2.f, defaultWindowHeight/2.f});
    shapeBackgroundRectangle.setPosition({defaultWindowWidth/2.f, defaultWindowHeight/2.f});

    addDrawableToMainLayer(std::make_shared<he::gfx::draw::Shape>(shapeQuartRectangle));
    addDrawableToMainLayer(std::make_shared<he::gfx::draw::Shape>(shapeBackgroundRectangle));
    addDrawableToMainLayer(std::make_shared<he::gfx::draw::Shape>(shapeBoundsRectangle));
    addDrawableToMainLayer(t_text1);

    display(200);
}

}// namespace he::gfx