
target_link_libraries(${EXECUTABLE_NAME}
	PRIVATE
		logger
	PUBLIC
		he::gfx
		he::program
		he::sys
		he::window
)