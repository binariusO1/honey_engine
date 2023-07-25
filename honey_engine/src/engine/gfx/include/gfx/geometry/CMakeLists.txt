set(MODULE_NAME geometry)

set(PUBLIC_HEADERS 
	include/geometry/Angle.hpp
	include/geometry/Angle.inl
	include/geometry/Line.hpp
	include/geometry/Matrix.hpp
	include/geometry/Point2d.hpp
	include/geometry/Point3d.hpp
	include/geometry/Size2d.hpp
	include/gfx/geometry/Vector2d.hpp
	include/geometry/Vector4d.hpp
	include/geometry/figures/Cross.hpp
	include/geometry/figures/Figure.hpp
	include/geometry/figures/Hexagon.hpp
	include/geometry/figures/Polygon.hpp
	include/geometry/figures/Rectangle.hpp
	include/geometry/figures/StretchedHexagon.hpp
	include/geometry/transform/Transform.hpp
	include/geometry/transform/Transformable.hpp
	include/geometry/transform/Transformable2d.hpp
	include/geometry/Utils.hpp
)

set(INTERNAL_FILES
	src/Cross.cpp
	src/Hexagon.cpp
	src/Polygon.cpp
	src/Rectangle.cpp
	src/StretchedHexagon.cpp
	src/Transform.cpp
	src/Transformable2d.cpp
	src/Utils.cpp
)

add_library(${MODULE_NAME} ${INTERNAL_FILES} ${PUBLIC_HEADERS})
set_target_properties(${MODULE_NAME_MOCKS} PROPERTIES LINKER_LANGUAGE CXX)
add_library(he::gfx::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(${MODULE_NAME}
	PUBLIC
	PRIVATE
		math
)

add_ut_subdirectory(tests)