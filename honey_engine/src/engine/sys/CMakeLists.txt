set(MODULE_NAME sys)

set(INTERNAL_FILES
	src/ImageLoader.cpp
)

set(PUBLIC_HEADERS 
	include/sys/honey_engine.h
	include/sys/ImageLoader.hpp
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
		he::libs
)

# add_ut_subdirectory(tests)