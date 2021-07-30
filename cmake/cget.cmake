function(cget_install)
    execute_process(
        COMMAND cget install --build_type ${CMAKE_BUILD_TYPE} -B ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endfunction ()
