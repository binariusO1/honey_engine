set(MODULE_NAME window)

set(INTERNAL_FILES
	src/Window.cpp
	src/WindowProcessInput.cpp
)

set(PUBLIC_HEADERS 
	include/window/Window.hpp
	include/Libraries.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_library(window::${MODULE_NAME} ALIAS ${MODULE_NAME})

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
		exception
		geometry
		gfx
		render::glad
		render::sfml2
		stb
		common::utils
)

add_ut_subdirectory(tests)