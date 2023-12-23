set(MODULE_NAME libs)

set(INTERNAL_FILES
	src/glad.c
	src/GlfwWrapper.cpp
	src/GlWrapper.cpp
	src/stb_image.cpp
	src/Shader.cpp
)

set(PUBLIC_HEADERS
	include/libs/gl/DrawType.hpp
	include/libs/gl/GlWrapper.hpp
	include/libs/gl/IGlWrapper.hpp
	include/libs/gl/ConnectionType.hpp
	include/libs/gl/Shader.hpp
	include/libs/glad/glad.h
	include/libs/glfw/GlfwWrapper.hpp
	include/libs/glfw/IGlfwWrapper.hpp
	include/libs/KHR/khrplatform.h
	include/libs/stb/stb_image.h
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
)

add_ut_subdirectory(tests)