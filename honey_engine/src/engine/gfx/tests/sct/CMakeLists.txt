set(MODULE_NAME_SCT ${MODULE_NAME}_SCT)
message([Cmake] " add_test_executable " ${TEST_NAME2} )

add_test_executable(${MODULE_NAME_SCT}
	TESTS ${MODULE_NAME}
	SOURCES
		InteractiveSCT.cpp
		RenderSCT.cpp
	LABELS
		${SCT_LABELS}
)

set( GLFW_LIBS glfw3 )

target_link_libraries(
	${MODULE_NAME_SCT}
	PRIVATE
		he::render_mocks
	PUBLIC
		${GLFW_LIBS}
		GTest::gmock_main
		he::gfx
		he::window
)
