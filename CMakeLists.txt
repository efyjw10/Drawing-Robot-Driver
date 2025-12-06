cmake_minimum_required(VERSION 3.15...4.0)

project(
    DrawingRobotDriver,
    VERSION 1.0
    LANGUAGES C
)

# Identify files to be compiled for library and executables
file(GLOB_RECURSE DrawingRobotDriverFiles   CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE TestFiles                 CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c")

add_library(rs232 "lib/rs232/rs232.c")
add_library(log "lib/log/log.c")

add_executable(DrawingRobotDriver "${DrawingRobotDriverFiles}")
add_executable(TestApplication "${TestFiles}")

target_include_directories(DrawingRobotDriver   PUBLIC "lib" "src")
target_include_directories(TestApplication      PUBLIC "lib" "src")

target_link_directories(DrawingRobotDriver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/build")
target_link_directories(TestApplication PUBLIC    "${CMAKE_CURRENT_SOURCE_DIR}/build")

target_link_libraries(DrawingRobotDriver log rs232)
target_link_libraries(TestApplication log rs232)

set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

target_compile_definitions(DrawingRobotDriver PRIVATE PageWidth=100 MinimumFontSize=4 MaximumFontSize=10 DefaultFontSize=18 LineSpacing=5)