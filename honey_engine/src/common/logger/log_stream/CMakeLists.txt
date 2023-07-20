set(MODULE_NAME log_stream)

set(INTERNAL_FILES 
	src/LogStream.cpp
)

set(PUBLIC_HEADERS 
	include/log_stream/LogSeverity.hpp
	include/log_stream/LogStream.hpp
	include/log_stream/LogBuffer.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_library(${PRE_MODULE_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

# hide visibility for intelliSense's prompts
set_target_properties(${MODULE_NAME} PROPERTIES FOLDER HiddenTargets)

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
)

target_link_libraries(${MODULE_NAME}
	PUBLIC
    	logger
	PRIVATE
		sys::config
)


# add_ut_subdirectory(tests)