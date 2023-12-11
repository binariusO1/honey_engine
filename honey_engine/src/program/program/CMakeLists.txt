set(MODULE_NAME program)

set(INTERNAL_FILES 
	src/MainProgram.cpp
	src/TestProgram.cpp
	)

set(PUBLIC_HEADERS 
    include/Program.hpp
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
		he::gfx
		he::sys
		he::window
)