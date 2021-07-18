if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Plain")
endif()        

set(allowedBuildTypes Plain Debug ASanDebug TSanDebug Release ASan TSan RelWithDebInfo MinSizeRel)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowedBuildTypes}")

if(CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
endif()
