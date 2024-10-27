cmake_minimum_required(VERSION 3.10)

project(gip-03)

# Add include directory to the build
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    # This shouldnt be here, TODO: REWORK
    ${CMAKE_SOURCE_DIR}/vendors/gip-gfx/include
)

# Add files recurse to executable
file(GLOB_RECURSE PROJECT_SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC gip_gfx)

# Allow other targets to include our headers
target_include_directories(gip-03 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)