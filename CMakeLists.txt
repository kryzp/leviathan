cmake_minimum_required(VERSION 3.0.0)
project(leviathan)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}

	src/core/app.cpp
	src/core/log.cpp
	src/core/time.cpp
	
	src/containers/string.cpp

	src/graphics/sprite_batch.cpp
	src/graphics/framebuffer.cpp
	src/graphics/image.cpp
	src/graphics/mesh.cpp
	src/graphics/material.cpp
	src/graphics/texture.cpp
	src/graphics/shader.cpp

	src/io/endian.cpp
	src/io/stream.cpp
	src/io/file_stream.cpp
	src/io/memory_stream.cpp
	src/io/binary_stream.cpp

	src/input/input.cpp

	src/ui/ui_component.cpp
	src/ui/ui_constraints.cpp

	src/math/calc.cpp
	src/math/ease.cpp
	src/math/circle.cpp
	src/math/colour.cpp
	src/math/line.cpp
	src/math/mat3x2.cpp
	src/math/mat4x4.cpp
	src/math/quad.cpp
	src/math/vec2.cpp
	src/math/polygon.cpp
	src/math/transform.cpp

	src/physics/collider.cpp
	
	src/backend/system_sdl2.cpp
	src/backend/renderer_opengl.cpp
	src/third_party/glad/glad.c
)

target_include_directories(${PROJECT_NAME}
	
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(OPENGL_ENABLED true CACHE BOOL "Use OpenGL as the renderer implementation")
set(SDL2_ENABLED true CACHE BOOL "Use SDL2 as the system implementation")
set(SDL2_INCLUDE_DIRS "D:/_DEV/libs/SDL2-2.0.16/include/") # disgusting
set(SDL2_LIBRARIES "D:/_DEV/libs/SDL2-2.0.16/lib/x64/SDL2.lib" "D:/_DEV/libs/SDL2-2.0.16/lib/x64/SDL2main.lib") # disgusting - the sequel

set(DEBUG_MODE true)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

if (DEBUG_MODE)
	add_compile_definitions(LEV_DEBUG)
endif()

if (OPENGL_ENABLED)
	add_compile_definitions(LEV_USE_OPENGL)
endif()

if (SDL2_ENABLED)
	add_compile_definitions(LEV_USE_SDL2)
	target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
endif()

add_executable(lv_test testing/src/main.cpp)
target_include_directories(lv_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_link_libraries(lv_test PUBLIC ${PROJECT_NAME})
