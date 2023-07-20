set(MODULE_NAME math)

set(INTERNAL_FILES 
)

set(PUBLIC_HEADERS 
	include/${MODULE_NAME}/Constants.hpp
	include/${MODULE_NAME}/Functions.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
add_library(gfx::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
	PRIVATE
		src
)