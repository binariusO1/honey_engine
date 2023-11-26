set(MODULE_NAME_MOCKS render_mocks)

set(PUBLIC_HEADERS
	include/draw/TextMock.hpp
	include/graphic/TextureMock.hpp
    include/render/LayerMock.hpp
    include/render/RenderMock.hpp
    include/render/SceneMock.hpp
    include/render/SceneStateMachineMock.hpp
	include/text/FreeType2Mock.hpp
	include/text/FontMock.hpp
)

set(INTERNAL_FILES
)

add_library(${MODULE_NAME_MOCKS} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME_MOCKS} PROPERTIES LINKER_LANGUAGE CXX)
add_library(he::${MODULE_NAME_MOCKS} ALIAS ${MODULE_NAME_MOCKS})

target_include_directories(${MODULE_NAME_MOCKS}
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(${MODULE_NAME_MOCKS}
    PUBLIC
		gtest_main
		gmock_main
    PRIVATE
		he::gfx
		he::window
		freetype
)