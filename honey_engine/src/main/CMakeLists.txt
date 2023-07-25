
target_link_libraries(${EXECUTABLE_NAME}
	PRIVATE
		logger
	PUBLIC
		he::gfx
		he::sys
		he::window
)