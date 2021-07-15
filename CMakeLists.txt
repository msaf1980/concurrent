cmake_minimum_required(VERSION 3.5)

option(BUILD_TEST "Build tests" ON)

include (CheckFunctionExists)
include (CheckIncludeFiles)
include (CheckSymbolExists)

project (libconcurrent)

include_directories (include)

add_subdirectory (src/concurrent)
#add_subdirectory (src/threads)
add_subdirectory (doc)

if (BUILD_TEST)
    include(CTest)
    add_subdirectory(test/concurrent)
    #add_subdirectory(test/threads)
endif() # END TEST
