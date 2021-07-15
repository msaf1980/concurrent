if(CMAKE_COMPILER_IS_GNU)
    # some warnings we want are not available with old GCC versions
    # note: starting with CMake 2.8 we could use CMAKE_C_COMPILER_VERSION
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_GREATER 4.5 OR GCC_VERSION VERSION_EQUAL 4.5)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wlogical-op")
    endif()
    if (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
    endif()
endif(CMAKE_COMPILER_IS_GNU)

if(CMAKE_COMPILER_IS_GNU OR CMAKE_COMPILER_IS_CLANG)
	if(PROFILE)
		append_flag(CMAKE_EXE_LINKER_FLAGS "-lprofiler" "-lprofiler")
	endif()

	add_compile_options(-Wall)
	add_compile_options(-Wextra)
	add_compile_options(-W)
	add_compile_options(-Wpedantic)
	add_compile_options(-Wconversion)
	add_compile_options(-Wdeclaration-after-statement)
	add_compile_options(-Wwrite-strings)

	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		append_flag(CMAKE_C_FLAGS "-O0")
		append_flag(CMAKE_CXX_FLAGS "-O0")
		add_definitions(-DDEBUG)
	endif(CMAKE_BUILD_TYPE MATCHES "Debug")
	# if(CMAKE_BUILD_TYPE STREQUAL "Release")
	# endif(CMAKE_BUILD_TYPE STREQUAL "Release")

	# ASAN
	if(CMAKE_BUILD_TYPE MATCHES "ASan")
	    set(ASAN_COMPILER_FLAGS "-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-common")
	    set(ASAN_LINKER_FLAGS "-lasan")
	    # For enable UBSan
	    #set(ASAN_COMPILER_FLAGS "${ASAN_COMPILER_FLAGS} -fsanitize=undefined")
	    #set(ASAN_LINKER_FLAGS "${ASAN_LINKER_FLAGS} -lubsan")
	    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_COMPILER_FLAGS}")
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_COMPILER_FLAGS}")
	    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_LINKER_FLAGS}")
	    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ASAN_LINKER_FLAGS}")
	endif()
	
	# TSAN
	if(CMAKE_BUILD_TYPE MATCHES "TSan")
	    set(TSAN_COMPILER_FLAGS "-fsanitize=thread -fno-omit-frame-pointer -fno-optimize-sibling-calls")
	    set(TSAN_LINKER_FLAGS "-ltsan")
	    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TSAN_COMPILER_FLAGS}")
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TSAN_COMPILER_FLAGS}")
	    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${TSAN_LINKER_FLAGS}")
	    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${TSAN_LINKER_FLAGS}")
	endif()
	
	# Coverage
	if(CMAKE_BUILD_TYPE MATCHES "Coverage")
		append_flag(CMAKE_C_FLAGS "--coverage")
		append_flag(CMAKE_CXX_FLAGS "--coverage")
		append_flag(CMAKE_C_FLAGS_COVERAGE "--coverage")
		append_flag(CMAKE_EXE_LINKER_FLAGS_COVERAGE "--coverage")
	endif()

endif(CMAKE_COMPILER_IS_GNU OR CMAKE_COMPILER_IS_CLANG)
