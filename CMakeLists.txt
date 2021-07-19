cmake_minimum_required(VERSION 3.14)

option(BUILD_TEST "Build tests" ON)
option(PROFILE "Profile with google perftools" OFF)

include (CheckFunctionExists)
include (CheckIncludeFiles)
include (CheckSymbolExists)

project (libconcurrent)

file(GLOB cmakeFiles cmake/*.cmake)
foreach(cmakeFile ${cmakeFiles})
    message("INCLUDE ${cmakeFile}")
    include(${cmakeFile})
endforeach(cmakeFile)

if (BUILD_TEST)
    include(CTest)
endif()

include_directories (include)

include(detect.cmake)
include(add.cmake)

include(${CMAKE_SOURCE_DIR}/options_print.cmake)
