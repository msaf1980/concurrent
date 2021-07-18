# For run tests under valgrind:
# ctest  -D ExperimentalMemCheck
# or (if configured with make backend)
# run target test-valgrind
if (ENABLE_VALGRIND)
    find_program(MEMORYCHECK_COMMAND valgrind)
    set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=255" )
    set(MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/valgrind_suppress.txt" )
    # set(DART_CONFIG DartConfiguration.tcl)
    # add_custom_target(${DART_CONFIG}
    #     COMMAND echo "MemoryCheckCommand: ${MEMORYCHECK_COMMAND}" >> ${DART_CONFIG}
    #     COMMENT "Generating ${DART_CONFIG}"
    # )    
    set(LOGFILE memcheck.log)
    add_custom_target(test-valgrind
        COMMAND ctest -O ${LOGFILE} -D ExperimentalMemCheck
        COMMAND tail -n1 ${LOGFILE} | grep 'Memory checking results:' > /dev/null
        COMMAND rm -f ${LOGFILE}
        DEPENDS ${DART_CONFIG}
    )
endif()
