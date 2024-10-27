#  SPDX-License-Identifier: Apache-2.0
#  SPDX-FileCopyrightText: 2023--2024 Tom Vierjahn <tom.vierjahn@acm.org>
#  SPDX-FileCopyrightText: 2023--2024 Visual Computing WH <vc@w-hs.de>

cmake_minimum_required(VERSION 3.23)
project(gip_gfx_example_02)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

### uncomment the next two lines when using this example outside gip_gfx #######
# find_package(sdl2)
# find_package(gip_gfx)
################################################################################

add_executable(hello_rectangle
  hello_rectangle.c
  )
target_link_libraries(hello_rectangle
  PUBLIC gip_gfx
  )

### remove the next lines when using this example outside gip_gfx ###############
install(TARGETS hello_rectangle DESTINATION bin/gip_gfx/examples)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/hello_rectangle.c
  DESTINATION share/gip_gfx/examples/02_hello_rectangle)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt.in
  DESTINATION share/gip_gfx/examples/02_hello_rectangle
  RENAME CMakeLists.txt)
################################################################################
