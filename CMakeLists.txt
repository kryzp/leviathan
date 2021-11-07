cmake_minimum_required(VERSION 3.0.0)
project(leviathan)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}

	src/core/app.cpp
	
	src/math/calc.cpp
	src/math/vec2.cpp

	src/input/input.cpp
	
	src/internal/platform_sdl2.cpp
	src/internal/graphics_opengl.cpp
)

target_include_directories(${PROJECT_NAME}
	
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(OPENGL_ENABLED true CACHE BOOL "Use OpenGL as the graphics implementation")
set(SDL2_ENABLED true CACHE BOOL "Use SDL2 as the system implementation")
set(SDL2_INCLUDE_DIRS "D:\\_DEV\\libs\\SDL2-2.0.16\\include\\")
set(SDL2_LIBRARIES "D:\\_DEV\\libs\\SDL2-2.0.16\\lib\\x64\\SDL2.lib" "D:\\_DEV\\libs\\SDL2-2.0.16\\lib\\x64\\SDL2main.lib")

set(DEBUG_MODE true)

if (DEBUG_MODE)
	add_compile_definitions(LEVIATHAN_DEBUG)
endif()

if (OPENGL_ENABLED)
	add_compile_definitions(LEVIATHAN_USE_OPENGL)
endif()

if (SDL2_ENABLED)
	add_compile_definitions(LEVIATHAN_USE_SDL2)
	target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
endif()

add_executable(lv_test testing/main.cpp)
target_include_directories(lv_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
target_link_libraries(lv_test PUBLIC ${PROJECT_NAME})
