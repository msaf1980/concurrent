set(CONCURRENT_VERSION "0.0.1")

set_version(CONCURRENT_VERSION CONCURRENT_VERSION_MAJOR CONCURRENT_VERSION_MINOR CONCURRENT_VERSION_PATCH CONCURRENT_VERSION_REVISION)

set(
    CONCURRENT_SOURCES
    mpmc_ring_queue.c
    queue.c
    queuedef.c
)

if(BUILD_SHARED_LIBS)
    add_library(concurrent_shared SHARED ${CONCURRENT_SOURCES})
    set_target_properties(concurrent_shared
        PROPERTIES OUTPUT_NAME concurrent
                   SOVERSION "${CONCURRENT_VERSION_MAJOR}"
                   VERSION "${CONCURRENT_VERSION}"
    )
endif()
add_library(concurrent STATIC ${CONCURRENT_SOURCES})
set_target_properties(concurrent PROPERTIES OUTPUT_NAME concurrent)

include(GNUInstallDirs)
install(TARGETS concurrent
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
# Install headers
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/concurrent"
        DESTINATION "include"
)

configure_file(${PROJECT_SOURCE_DIR}/libconcurrent.pc.in ${CMAKE_BINARY_DIR}/libconcurrent.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/libconcurrent.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

if (BUILD_TEST)
    add_subdirectory(test)
endif() # END TEST
