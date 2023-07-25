set(MODULE_NAME utils)

set(INTERNAL_FILES 
	src/Chronometer.cpp
	src/TimeScheduler.cpp
	src/WorkerThread.cpp
)

set(PUBLIC_HEADERS 
	include/${MODULE_NAME}/Chronometer.hpp
	include/${MODULE_NAME}/Timer.hpp
	include/${MODULE_NAME}/TimeScheduler.hpp
	include/${MODULE_NAME}/WorkerThread.hpp
	include/task_utils/ITask.hpp
	include/task_utils/ITaskMaster.hpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
add_library(common::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(${MODULE_NAME}
	PUBLIC
    	logger
	PRIVATE
)

add_ut_subdirectory(tests)