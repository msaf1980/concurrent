if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Plain")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(COVERAGE ON)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(allowedBuildTypes Plain Debug ASanDebug TSanDebug Release ASan TSan RelWithDebInfo MinSizeRel)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowedBuildTypes}")

if(CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
endif()

if (COVERAGE)
    message(FATAL_ERROR "coverage not implemented")
endif()