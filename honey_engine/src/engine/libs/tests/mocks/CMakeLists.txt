set(MODULE_NAME_MOCKS opengl_mocks)

set(PUBLIC_HEADERS
    include/glfw/GlfwMock.hpp
    include/gl/GlWrapperMock.hpp
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
		he::libs
    PRIVATE

)