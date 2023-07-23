set(MODULE_NAME glad)

set(INTERNAL_FILES 
	src/glad.c
)

set(PUBLIC_HEADERS 
	include/glad/glad.h
	include/KHR/khrplatform.h
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
)