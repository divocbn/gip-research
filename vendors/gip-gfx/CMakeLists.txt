#  SPDX-License-Identifier: Apache-2.0
#  SPDX-FileCopyrightText: 2023 Tom Vierjahn <tom.vierjahn@acm.org>
#  SPDX-FileCopyrightText: 2023 Visual Computing WH <vc@w-hs.de>

cmake_minimum_required(VERSION 3.23)
project(gip_gfx)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

set(gip_gfx_SOURCES src/gip_gfx/gip_gfx.c)
set(gip_gfx_PUBLIC_HEADERS include/gip_gfx/gip_gfx.h)

add_library(gip_gfx STATIC)
target_sources(gip_gfx PRIVATE ${gip_gfx_SOURCES})
target_sources(gip_gfx INTERFACE FILE_SET HEADERS TYPE HEADERS
  BASE_DIRS include/
  FILES include/gip_gfx/gip_gfx.h)

target_include_directories(gip_gfx PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(gip_gfx PUBLIC SDL2::SDL2 PUBLIC SDL2::SDL2main)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/gip_gfx-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/gip_gfx-config.cmake
  INSTALL_DESTINATION lib/cmake/gip_gfx
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/gip_gfx-config-version.cmake
  VERSION 23.10
  COMPATIBILITY ExactVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gip_gfx-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/gip_gfx-config-version.cmake
        DESTINATION lib/cmake/gip_gfx )


install(TARGETS gip_gfx EXPORT gip_gfx-targets FILE_SET HEADERS)
install(EXPORT gip_gfx-targets
  FILE gip_gfx-targets.cmake
  DESTINATION lib/cmake/gip_gfx
)

add_subdirectory(examples)
