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
add_library(logger "lib/logger/logger.c")

target_compile_definitions(logger PRIVATE logPath="$<TARGET_FILE_DIR:DrawingRobotDriver>/log.txt")

add_executable(DrawingRobotDriver "${DrawingRobotDriverFiles}")

target_include_directories(DrawingRobotDriver   PUBLIC "lib" "src")

target_link_directories(DrawingRobotDriver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/build")

target_link_libraries(DrawingRobotDriver rs232 logger)

set_target_properties(DrawingRobotDriver PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG "Debug"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE "Release"
)

add_custom_target(inputFiles
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/InputFiles"
        "$<TARGET_FILE_DIR:DrawingRobotDriver>"
    COMMENT "Copying text file"
)

target_compile_definitions(DrawingRobotDriver PRIVATE   PageWidth=100 
                                                        MinimumFontSize=4 
                                                        MaximumFontSize=10 
                                                        DefaultFontSize=18 
                                                        LineSpacing=5 
                                                        CharacterSetSize=128
                                                        fontFilePath="$<TARGET_FILE_DIR:DrawingRobotDriver>/SingleStrokeFont.txt"
                                                        inputFilePath="$<TARGET_FILE_DIR:DrawingRobotDriver>/test.txt"
                                                        logPath="$<TARGET_FILE_DIR:DrawingRobotDriver>/log.txt")
                                       

add_dependencies(DrawingRobotDriver inputFiles)
