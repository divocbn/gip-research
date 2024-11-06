cmake_minimum_required(VERSION 3.10)

project(gip-03-viewer)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Add include directory to the build
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    # This shouldnt be here, TODO: REWORK
    ${CMAKE_SOURCE_DIR}/vendors/gip-gfx/include
    ${CMAKE_SOURCE_DIR}/vendors/cimgui
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/generator/output
)

# Add files recurse to executable
file(GLOB_RECURSE PROJECT_SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)


# CIMGUI Implementation
# Kinda weird, because cimgui doesn't ship with a implementation for sdl2 & opengl
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/cimgui.cpp 
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui/imgui.cpp 
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui/imgui_draw.cpp 
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui/imgui_demo.cpp 
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui/backends/imgui_impl_sdl2.cpp
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui/backends/imgui_impl_sdlrenderer2.cpp
    ${CMAKE_SOURCE_DIR}/vendors/cimgui/imgui/backends/imgui_impl_opengl3.cpp
)

# Set C++11 only for cimgui implementation
set_source_files_properties(${IMGUI_SOURCES} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${IMGUI_SOURCES} PROPERTIES COMPILE_FLAGS "-std=c++11")

add_library(cimgui_impl STATIC ${IMGUI_SOURCES})

target_compile_definitions(cimgui_impl PUBLIC "-DIMGUI_IMPL_API=extern \"C\" ")
target_link_libraries(cimgui_impl PUBLIC gip_gfx)

# Project
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

# Set C language for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC GLEW::GLEW cimgui cimgui_impl)

# Allow other targets to include our headers
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)