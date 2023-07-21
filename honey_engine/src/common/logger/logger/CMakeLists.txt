set(MODULE_NAME logger)

set(INTERNAL_FILES 
)

set(PUBLIC_HEADERS 
	include/logger/Logger.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_library(${PRE_MODULE_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
)

target_link_libraries(${MODULE_NAME}
	PUBLIC
    	log_stream
)

add_ut_subdirectory(tests)