set(MODULE_NAME window)

set(INTERNAL_FILES
	src/events/EventManager.cpp
	src/window/FrameGuard.cpp
	src/window/Window.cpp
	src/window/WindowProcessEvent.cpp
)

set(PUBLIC_HEADERS 
	include/window/events/Event.hpp
	include/window/events/EventManager.hpp
	include/window/events/IEventInputListener.hpp
	include/window/events/IEventInputSender.hpp
	include/window/events/Keyboard.hpp
    include/window/events/Mouse.hpp
    include/window/events/WindowState.hpp
	include/window/window/FrameGuard.hpp
	include/window/window/Window.hpp
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
		he::common::exception
		he::libs
		# stb
		common::utils
)

add_ut_subdirectory(tests)