set(MODULE_NAME math)

set(PUBLIC_HEADERS 
	include/${MODULE_NAME}/Constants.hpp
	include/${MODULE_NAME}/Functions.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)
add_library(he::common::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
	PRIVATE
)