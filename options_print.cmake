message(STATUS "")
message(STATUS "BUILD SUMMARY")
message(STATUS "  CMAKE_GENERATOR       : ${CMAKE_GENERATOR}")
message(STATUS "  Compiler ID           : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Compiler C            : ${CMAKE_C_COMPILER}")
message(STATUS "  Compiler CXX          : ${CMAKE_CXX_COMPILER}")
message(STATUS "  Build type            : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Architecture          : ${ARCH_TYPE}")
message(STATUS "")
message(STATUS "  Install prefix : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Binary dir     : ${BINDIR}")
message(STATUS "  Lib dir        : ${LIBDIR}")
message(STATUS "  Version        : ${VERSION}")
message(STATUS "")
if(CMAKE_BUILD_TYPE STREQUAL "Plain")
    message(STATUS "CMAKE_CXX_FLAGS            : ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_C_FLAGS              : ${CMAKE_C_FLAGS}")
    message(STATUS "CMAKE_SHARED_LINKER_FLAGS  : ${CMAKE_SHARED_LINKER_FLAGS}")
    message(STATUS "CMAKE_EXE_LINKER_FLAGS     : ${CMAKE_EXE_LINKER_FLAGS}")
else()
    message(STATUS "CMAKE_CXX_FLAGS            : ${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")
    message(STATUS "CMAKE_C_FLAGS              : ${CMAKE_C_FLAGS_${BUILD_TYPE}}")
    message(STATUS "CMAKE_SHARED_LINKER_FLAGS  : ${CMAKE_SHARED_LINKER_FLAGS_${BUILD_TYPE}}")
    message(STATUS "CMAKE_EXE_LINKER_FLAGS     : ${CMAKE_EXE_LINKER_FLAGS_${BUILD_TYPE}}")
endif()
message(STATUS "")
if(MEMORYCHECK_COMMAND)
    message(STATUS "MEMORYCHECK_COMMAND          : ${MEMORYCHECK_COMMAND}")
    message(STATUS "MEMORYCHECK_OPTIONS          : ${MEMORYCHECK_COMMAND_OPTIONS}")
    message(STATUS "MEMORYCHECK_SUPPRESSIONS     : ${MEMORYCHECK_SUPPRESSIONS_FILE}")
    message(STATUS "For run tests under valgrind : ctest  -D ExperimentalMemCheck")
    message(STATUS "")
endif()
