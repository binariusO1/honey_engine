set(SOURCES
 src/Main.cpp
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME}
	PRIVATE
		# common::geometry
		# gfx::gui
		# gfx::render
		# gfx::draw2d
		logger
		# gfx::scenes
		# sys::config
		# sys::resource
		# utils
		render::gl_wrapper
)