set(MODULE_NAME gl_wrapper)

set(INTERNAL_FILES
	src/GlResource.cpp
	# src/RenderWrapper.cpp
	src/Shader.cpp
	src/Window.cpp
	src/WindowProcessInput.cpp
)

set(PUBLIC_HEADERS 
	include/gl_wrapper/window/Window.hpp
	include/gl_wrapper/render/PrimitiveType.hpp
	# include/gl_wrapper/render/RenderWrapper.hpp
	include/gl_wrapper/render/Shader.hpp
	include/gl_wrapper/resources/GlResource.hpp
	include/gl_wrapper/Source.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_library(${PRE_MODULE_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

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
		geometry
		gfx
		opengl::glad
		opengl::sfml2
		# render
		stb
		common::utils
)