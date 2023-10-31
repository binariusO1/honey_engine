set(MODULE_NAME_UT ${MODULE_NAME}_UT)

message([Cmake] " add_test_executable " ${TEST_NAME2} )

set(SOURCES
	ButtonTests.cpp
	FiguresTests.cpp
	ImageTests.cpp
	LayerTests.cpp
	Point2dTests.cpp
	SceneTests.cpp
	SceneManagerTests.cpp
	ShapeTests.cpp
	SpriteTests.cpp
	TextureTests.cpp
	TextTests.cpp
	Transformable2dTests.cpp
)

if(COVERAGE)
	list(APPEND SOURCES 
		../../src/draw/Button.cpp
		../../src/draw/Shape.cpp
		../../src/draw/Sprite.cpp
		../../src/draw/Text.cpp
		../../src/graphic/Image.cpp
		../../src/graphic/Texture.cpp
	)
endif()

add_test_executable(${MODULE_NAME_UT}
	TESTS ${MODULE_NAME}
	SOURCES
	${SOURCES}
	LABELS
		${UT_LABELS}
)

if(COVERAGE)
	target_link_libraries(
		${MODULE_NAME_UT}
		PUBLIC
			gcov
	)
endif()

set( GLFW_LIBS glfw3 )

target_link_libraries(
	${MODULE_NAME_UT}
	PRIVATE
		he::opengl_mocks
		he::render_mocks
	PUBLIC
		gtest_main
		gmock
		he::gfx
)

# coverage
if (COVERAGE)
	list(APPEND CXX_FLAGS_COVERAGE "-std=c++11" "-Wno-deprecated-register")
	list(APPEND CXX_FLAGS_COVERAGE "-O0" "-g")
	list(APPEND CXX_FLAGS_COVERAGE "--coverage")
	list(APPEND CXX_FLAGS_COVERAGE "-fprofile-arcs" "-ftest-coverage")
	target_compile_options(${MODULE_NAME_UT} PUBLIC ${CXX_FLAGS_COVERAGE})
endif()