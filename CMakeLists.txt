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

set_version(0 0 1)

include_directories (include)

add_subdirectory (src/concurrent)
add_subdirectory (doc)

if (BUILD_TEST)
    include(CTest)
    add_subdirectory(test/concurrent)
endif() # END TEST

include(${CMAKE_SOURCE_DIR}/options_print.cmake)
