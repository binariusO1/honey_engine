set(MODULE_NAME gfx)

set(INTERNAL_FILES 
	src/text/Font.cpp
	src/text/FreeType2Wrapper.cpp
	src/text/Page.cpp
	src/graphic/Image.cpp
	src/graphic/Texture.cpp
	src/draw/Button.cpp
	src/draw/Shape.cpp
	src/draw/Sprite.cpp
	src/draw/Text.cpp
	src/geometry/figures/Cross.cpp
	src/geometry/figures/Hexagon.cpp
	src/geometry/figures/Polygon.cpp
	src/geometry/figures/Rectangle.cpp
	src/geometry/figures/StretchedHexagon.cpp
	src/geometry/figures/Transform.cpp
	src/geometry/figures/Transformable2d.cpp
	src/geometry/figures/Utils.cpp
	src/render/BaseLayer.cpp
	src/render/Layer.cpp
	src/render/CopyPropagationLayer.cpp
	src/render/Render.cpp
	src/render/Scene.cpp
	src/render/SceneManager.cpp
	)

set(PUBLIC_HEADERS 
    include/gfx/draw/Button.hpp
	include/gfx/draw/IDrawable.hpp
	include/gfx/draw/Shape.hpp
	include/gfx/draw/Sprite.hpp
	include/gfx/draw/Text.hpp
	include/gfx/event/Event.hpp
	include/gfx/geometry/Angle.hpp
	include/gfx/geometry/Angle.inl
	include/gfx/geometry/Line.hpp
	include/gfx/geometry/Matrix.hpp
	include/gfx/geometry/Point2d.hpp
	include/gfx/geometry/Point3d.hpp
	include/gfx/geometry/Size2d.hpp
	include/gfx/geometry/Vector2d.hpp
	include/gfx/geometry/Vector4d.hpp
	include/gfx/geometry/figures/Cross.hpp
	include/gfx/geometry/figures/Figure.hpp
	include/gfx/geometry/figures/Hexagon.hpp
	include/gfx/geometry/figures/Polygon.hpp
	include/gfx/geometry/figures/Rectangle.hpp
	include/gfx/geometry/figures/StretchedHexagon.hpp
	include/gfx/geometry/transform/ITransformable.hpp
	include/gfx/geometry/transform/Transform.hpp
	include/gfx/geometry/transform/Transformable2d.hpp
	include/gfx/geometry/Utils.hpp
    include/gfx/graphic/Image.hpp
    include/gfx/graphic/ITexture.hpp
    include/gfx/graphic/Texture.hpp
    include/gfx/render/BaseLayer.hpp
    include/gfx/render/ILayer.hpp
    include/gfx/render/IRender.hpp
    include/gfx/render/IRenderable.hpp
    include/gfx/render/IScene.hpp
    include/gfx/render/IStateMachine.hpp
    include/gfx/render/Layer.hpp
    include/gfx/render/OpenCloseState.hpp
    include/gfx/render/CopyPropagationLayer.hpp
    include/gfx/render/PropagationSettings.hpp
    include/gfx/render/Render.hpp
    include/gfx/render/RenderSettings.hpp
    include/gfx/render/Scene.hpp
    include/gfx/render/SceneStateMachine.hpp
    include/gfx/render/SceneManager.hpp
    include/gfx/render/VisibilityState.hpp
    include/gfx/text/Font.hpp
    include/gfx/text/FreeType2Wrapper.hpp
    include/gfx/text/Glyph.hpp
    include/gfx/text/IFont.hpp
    include/gfx/text/IFreeType2Wrapper.hpp
    include/gfx/text/Page.hpp
    include/gfx/Color.hpp
    include/gfx/Color.inl
    include/gfx/ColorBasic.hpp
    include/gfx/ColorBasic.inl
    include/gfx/Vertex.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_library(he::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(${MODULE_NAME}
	PRIVATE
    	logger
	PUBLIC
		he::common::exception
		he::common::math
		he::sys
		he::window
)

add_ut_subdirectory(tests)