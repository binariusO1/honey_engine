if(NOT COVERAGE)
	set(MODULE_NAME_SCT ${MODULE_NAME}_SCT)
	message([Cmake] " add_test_executable " ${MODULE_NAME_SCT} )

	add_test_executable(${MODULE_NAME_SCT}
		TESTS ${MODULE_NAME}
		SOURCES
			InteractiveSCT.cpp
			RenderSCT.cpp
		LABELS
			${SCT_LABELS}
	)

	set( GLFW_LIBS glfw3 )
	set( FREETYPE_LIBS freetype )

	target_link_libraries(
		${MODULE_NAME_SCT}
		PRIVATE
			he::render_mocks
		PUBLIC
			${GLFW_LIBS}
			GTest::gmock_main
			he::gfx
			he::window
			${FREETYPE_LIBS}
	)
endif()