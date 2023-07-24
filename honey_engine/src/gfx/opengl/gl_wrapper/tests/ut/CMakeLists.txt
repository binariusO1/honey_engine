set(MODULE_NAME_UT ${MODULE_NAME}_UT)
	# message([Cmake] " add_test_executable " ${TEST_NAME2} )

add_test_executable(${MODULE_NAME_UT}
	TESTS ${MODULE_NAME}
	SOURCES
		WindowTests.cpp
	LABELS
		${UT_LABELS}
)

set( GLFW_LIBS glfw3 )

target_link_libraries(
	${MODULE_NAME_UT}
	PUBLIC
		${GLFW_LIBS}
		gtest_main
		opengl::gl_wrapper
)
