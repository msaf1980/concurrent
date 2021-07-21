option(PROFILE "Profile with google perftools" OFF)
option(COVERAGE "Coverage" OFF)
option(ENABLE_VALGRIND "Tests under valgring (test-valgrind target)" ON)
option(EXPORT_COMPILE "Export compile database" OFF)

if(EXPORT_COMPILE)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
