#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/draw/ShapeRectangle.hpp"
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
    ~TextSCT() = default;

    void setDaultTextObject()
    {
        t_text1 = std::make_shared<he::gfx::draw::Text>("text1");
        t_text1->setFont(f_fontCalibriPath);
        t_text1->setCharacterSize(40); 
        t_text1->setString("Test"); 
        t_text1->update();
    }

    std::shared_ptr<he::gfx::draw::Text> t_text1;
};

TEST_F(TextSCT, textDraw_shouldDrawTextInDefaultPosition)
{
    createCustomScreen();
    setDaultTextObject();

    auto textBounds = t_text1->getLocalBounds();
    he::gfx::draw::ShapeRectangle shape1("backgroundRectangle", {textBounds.width*2, textBounds.height*2});
    he::gfx::draw::ShapeRectangle shape2("boundsRectangle", {textBounds.width, textBounds.height});
    shape1.setColor(gfx::Color::Red);
    shape2.setColor(gfx::Color::Black);

    addDrawableToMainLayer(std::make_shared<he::gfx::draw::ShapeRectangle>(shape1));
    addDrawableToMainLayer(std::make_shared<he::gfx::draw::ShapeRectangle>(shape2));
    addDrawableToMainLayer(t_text1);

    display(200);
}

TEST_F(TextSCT, textDraw_shouldDrawTextInDefaultPositionAfterSetNewCharacterSize)
{
    int displayTime = 50;
    createCustomScreen();
    setDaultTextObject();
    t_text1->setString("character size 40");
    t_text1->update();

    auto textBounds = t_text1->getLocalBounds();

    std::shared_ptr<he::gfx::draw::ShapeRectangle> backgroundRectangle = 
        std::make_shared<he::gfx::draw::ShapeRectangle>("backgroundRectangle", geometry::Size2Df{textBounds.width*2, textBounds.height*2});
    std::shared_ptr<he::gfx::draw::ShapeRectangle> boundsRectangle = 
        std::make_shared<he::gfx::draw::ShapeRectangle>("boundsRectangle", geometry::Size2Df{textBounds.width, textBounds.height});
    backgroundRectangle->setColor(gfx::Color::Red);
    boundsRectangle->setColor(gfx::Color::Blue);

    addDrawableToMainLayer(backgroundRectangle);
    addDrawableToMainLayer(boundsRectangle);
    addDrawableToMainLayer(t_text1);

    display(displayTime);
    t_text1->setString("character size 0");
    t_text1->setCharacterSize(0);
    t_text1->update();
    textBounds = t_text1->getLocalBounds();
    backgroundRectangle->setSize({textBounds.width*2, textBounds.height*2});
    boundsRectangle->setSize({textBounds.width, textBounds.height});
    display(displayTime);
    t_text1->setString("character size 20");
    t_text1->setCharacterSize(20);
    t_text1->update();
    textBounds = t_text1->getLocalBounds();
    backgroundRectangle->setSize({textBounds.width*2, textBounds.height*2});
    boundsRectangle->setSize({textBounds.width, textBounds.height});
    display(displayTime);
    t_text1->setString("character size 40");
    t_text1->setCharacterSize(40);
    t_text1->update();
    textBounds = t_text1->getLocalBounds();
    backgroundRectangle->setSize({textBounds.width*2, textBounds.height*2});
    boundsRectangle->setSize({textBounds.width, textBounds.height});
    t_text1->update();

    display(displayTime);
}

TEST_F(TextSCT, textDraw_shouldDrawTextInMovedPosition)
{
    createCustomScreen();
    setDaultTextObject();

    auto textBounds = t_text1->getLocalBounds();
    he::gfx::draw::ShapeRectangle shapeBackgroundRectangle("backgroundRectangle", {textBounds.width*2, textBounds.height*2});
    he::gfx::draw::ShapeRectangle shapeBoundsRectangle("boundsRectangle", {textBounds.width, textBounds.height});
    he::gfx::draw::ShapeRectangle shapeQuartRectangle("quartRectangle", {f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    shapeBackgroundRectangle.setColor(gfx::Color::Red);
    shapeBoundsRectangle.setColor(gfx::Color::Blue);
    shapeQuartRectangle.setColor(gfx::Color::Red);

    t_text1->setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    shapeBoundsRectangle.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    shapeBackgroundRectangle.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});

    addDrawableToMainLayer(std::make_shared<he::gfx::draw::ShapeRectangle>(shapeQuartRectangle));
    addDrawableToMainLayer(std::make_shared<he::gfx::draw::ShapeRectangle>(shapeBackgroundRectangle));
    addDrawableToMainLayer(std::make_shared<he::gfx::draw::ShapeRectangle>(shapeBoundsRectangle));
    addDrawableToMainLayer(t_text1);

    display(200);
}

TEST_F(TextSCT, textDraw_afterMovedToCenterAndChangeOriginToCenter_shouldDrawTextInCenterPosition)
{
    createCustomScreen();
    setDaultTextObject();

    auto textBounds = t_text1->getLocalBounds();
    he::gfx::draw::ShapeRectangle shapeQuartRectangle("quartRectangle", {f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    shapeQuartRectangle.setColor(gfx::Color::Red);

    addDrawableToMainLayer(std::make_shared<he::gfx::draw::ShapeRectangle>(shapeQuartRectangle));
    addDrawableToMainLayer(t_text1);
    display(75);

    t_text1->setOriginInCenter();
    display(75);

    t_text1->setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    display(75);
}
}// namespace he::gfx