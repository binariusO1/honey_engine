set(MODULE_NAME gfx)

set(INTERNAL_FILES 
	# src/Font.cpp
	# src/Image.cpp
	# src/Page.cpp
	# src/Texture.cpp
	# src/VertexArray2d.cpp
	)

set(PUBLIC_HEADERS 
	include/Color.hpp
	include/Color.inl
	include/ColorBasic.hpp
	include/ColorBasic.inl
	include/Vertex.hpp
	include/VertexArray.hpp
	# include/VertexArray2d.hpp
	# include/graphics/Image.hpp
	# include/graphics/Texture.hpp
	# include/text/Font.hpp
	# include/text/Glyph.hpp
	# include/text/Page.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_library(gfx::${MODULE_NAME} ALIAS ${MODULE_NAME})

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
		# sys::config
		# sys::input_stream
		render::gl_wrapper
)