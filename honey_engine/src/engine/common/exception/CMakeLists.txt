set(MODULE_NAME exception)

set(INTERNAL_FILES
)

set(PUBLIC_HEADERS 
	include/exception/invalid_initialization.hpp
	include/exception/render_error.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
add_library(he::common::${MODULE_NAME} ALIAS ${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
	PRIVATE
		src
)