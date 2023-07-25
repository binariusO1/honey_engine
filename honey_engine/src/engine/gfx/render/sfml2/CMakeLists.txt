set(MODULE_NAME sfml2)

set(INTERNAL_FILES
)

set(PUBLIC_HEADERS 
	include/sfml2/window/Event.hpp
	include/sfml2/window/Keyboard.hpp
	include/sfml2/window/Mouse.hpp
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
	PUBLIC
    	logger
		stb
)