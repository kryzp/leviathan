﻿cmake_minimum_required(VERSION 3.0.0)
project(leviathan)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME}

	public/lev/core/app.cpp
	public/lev/core/log.cpp
	public/lev/core/time.cpp
	public/lev/core/memutil.cpp
	public/lev/core/strutil.cpp

	public/lev/assets/asset_mgr.cpp

	public/lev/audio/sound.cpp

	public/lev/graphics/gfxutil.cpp
	public/lev/graphics/sprite_batch.cpp
	public/lev/graphics/render_target.cpp
	public/lev/graphics/render_pass.cpp
	public/lev/graphics/image.cpp
	public/lev/graphics/font.cpp
	public/lev/graphics/mesh.cpp
	public/lev/graphics/material.cpp
	public/lev/graphics/texture.cpp
	public/lev/graphics/shader.cpp
	public/lev/graphics/compare.cpp
	public/lev/graphics/blend.cpp

	public/lev/io/endian.cpp
	public/lev/io/stream.cpp
	public/lev/io/file_stream.cpp
	public/lev/io/memory_stream.cpp

	public/lev/input/input.cpp

	public/lev/math/calc.cpp
	public/lev/math/ease.cpp
	public/lev/math/circle.cpp
	public/lev/math/colour.cpp
	public/lev/math/mat3x2.cpp
	public/lev/math/mat4x3.cpp
	public/lev/math/mat4x4.cpp
	public/lev/math/line.cpp
	public/lev/math/quad.cpp
	public/lev/math/triangle.cpp
	public/lev/math/polygon.cpp
	public/lev/math/timer.cpp
	public/lev/math/quaternion.cpp
	public/lev/math/complex.cpp

	private/backend/system_sdl2.cpp
	private/backend/renderer_opengl.cpp
	private/backend/audio_irrklang.cpp

	private/third_party/glad/glad.c
)

target_include_directories(${PROJECT_NAME}
	
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/public
	
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/private
)

set(OPENGL_ENABLED true CACHE BOOL "Use OpenGL as the renderer implementation")
set(SDL2_ENABLED true CACHE BOOL "Use SDL2 as the system implementation")
set(IRRKLANG_ENABLED true CACHE BOOL "Use Irrklang as the audio implementation")

set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.0.20/include/")
set(SDL2_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.0.20/lib/x64/SDL2.lib" "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2-2.0.20/lib/x64/SDL2main.lib")

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

add_compile_definitions(LEV_DEBUG)

if (OPENGL_ENABLED)
	add_compile_definitions(LEV_USE_OPENGL)
endif()

if (SDL2_ENABLED)
	add_compile_definitions(LEV_USE_SDL2)
	target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
endif()

if (IRRKLANG_ENABLED)
	add_compile_definitions(LEV_USE_IRRKLANG)
endif()

add_executable(lv_test testing/src/test_misc.cpp)
target_include_directories(lv_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_link_libraries(lv_test PUBLIC ${PROJECT_NAME})

add_custom_command(TARGET lv_test
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/testing/res/ $<TARGET_FILE_DIR:lv_test>/res
)
