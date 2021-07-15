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
	add_compile_options(-Wall)
	add_compile_options(-Wextra)
	add_compile_options(-W)
	add_compile_options(-Wpedantic)
	add_compile_options(-Wconversion)
	add_compile_options(-Wdeclaration-after-statement)
	add_compile_options(-Wwrite-strings)

	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		append_flag(CMAKE_C_FLAGS_DEBUG "-O0" "-O")
		append_flag(CMAKE_CXX_FLAGS_DEBUG "-O0" "-O")
		add_definitions(-DDEBUG)
	endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

	# ASAN
	set(ASAN_COMPILER_FLAGS "-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-common")
	set(ASAN_LINKER_FLAGS "-lasan")
	# For enable UBSan
	#set(ASAN_COMPILER_FLAGS "${ASAN_COMPILER_FLAGS} -fsanitize=undefined")
	#set(ASAN_LINKER_FLAGS "${ASAN_LINKER_FLAGS} -lubsan")
	set(CMAKE_C_FLAGS_ASAN
    	"${CMAKE_C_FLAGS_DEBUG} ${ASAN_COMPILER_FLAGS}" CACHE STRING
    	"Flags used by the C compiler for ASan build type or configuration." FORCE)

	set(CMAKE_CXX_FLAGS_ASAN
    	"${CMAKE_CXX_FLAGS_DEBUG} ${ASAN_COMPILER_FLAGS}" CACHE STRING
    	"Flags used by the C++ compiler for ASan build type or configuration." FORCE)

	set(CMAKE_EXE_LINKER_FLAGS_ASAN
    	"${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${ASAN_LINKER_FLAGS}" CACHE STRING
    	"Linker flags to be used to create executables for ASan build type." FORCE)

	set(CMAKE_SHARED_LINKER_FLAGS_ASAN
		"${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${ASAN_LINKER_FLAGS}" CACHE STRING
		"Linker lags to be used to create shared libraries for ASan build type." FORCE)	

	# TSAN
	set(TSAN_COMPILER_FLAGS "-fsanitize=thread -fno-omit-frame-pointer -fno-optimize-sibling-calls")
	set(TSAN_LINKER_FLAGS "-ltsan")
	set(CMAKE_C_FLAGS_TSAN
    	"${CMAKE_C_FLAGS_DEBUG} ${TSAN_COMPILER_FLAGS}" CACHE STRING
    	"Flags used by the C compiler for TSan build type or configuration." FORCE)

	set(CMAKE_CXX_FLAGS_TSAN
    	"${CMAKE_CXX_FLAGS_DEBUG} ${TSAN_COMPILER_FLAGS}" CACHE STRING
    	"Flags used by the C++ compiler for TSan build type or configuration." FORCE)

	set(CMAKE_EXE_LINKER_FLAGS_TSAN
    	"${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${TSAN_LINKER_FLAGS}" CACHE STRING
    	"Linker flags to be used to create executables for TSan build type." FORCE)

	set(CMAKE_SHARED_LINKER_FLAGS_TSAN
		"${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${TSAN_LINKER_FLAGS}" CACHE STRING
		"Linker lags to be used to create shared libraries for TSan build type." FORCE)

	# Coverage
	set(COVERAGE_FLAGS "--coverage")
	set(CMAKE_C_FLAGS_COVERAGE
    	"${CMAKE_C_FLAGS_DEBUG} ${COVERAGE_FLAGS}" CACHE STRING
    	"Flags used by the C compiler for Coverage build type or configuration." FORCE)

	set(CMAKE_CXX_FLAGS_COVERAGE
    	"${CMAKE_CXX_FLAGS_DEBUG} ${COVERAGE_FLAGS}" CACHE STRING
    	"Flags used by the C++ compiler for Coverage build type or configuration." FORCE)

	set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    	"${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" CACHE STRING
    	"Linker flags to be used to create executables for Coverage build type." FORCE)

	set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
		"${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${COVERAGE_FLAGS}" CACHE STRING
		"Linker lags to be used to create shared libraries for Coverage build type." FORCE)

	# if(CMAKE_BUILD_TYPE STREQUAL "ASan")
	# 	add_definitions(-DDEBUG)
	# endif(CMAKE_BUILD_TYPE STREQUAL "ASan")

	# if(CMAKE_BUILD_TYPE STREQUAL "TSan")
	# 	add_definitions(-DDEBUG)
	# endif(CMAKE_BUILD_TYPE STREQUAL "TSan")

	# if(CMAKE_BUILD_TYPE STREQUAL "Release")
	# endif(CMAKE_BUILD_TYPE STREQUAL "Release")

	if(PROFILE)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lprofiler")
	endif()

endif(CMAKE_COMPILER_IS_GNU OR CMAKE_COMPILER_IS_CLANG)
