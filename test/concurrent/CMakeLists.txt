# Scan source dir for standart source files and exclude main.cpp

include_directories(../../include ../../src ..)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(REQUIRED_SOURCES)

include(CheckFunctionExists)
set(CMAKE_REQUIRED_LIBRARIES Threads::Threads)
check_function_exists(pthread_barrier_init HAVE_PTHREAD_BARRIER)
if(HAVE_PTHREAD_BARRIER)
    ADD_DEFINITIONS(-DHAVE_PTHREAD_BARRIER=1)   
else()
    list(APPEND REQUIRED_SOURCES ../pthread_barrier.c)
    ADD_DEFINITIONS(-DHAVE_PTHREAD_BARRIER=0)
endif()
check_function_exists(pthread_spin_init HAVE_PTHREAD_SPIN)

set(TEST_LIBRARIES concurrent_static Threads::Threads)

# Build tests
add_executable(test_mpmc_ring_queue mpmc_ring_queue_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_mpmc_ring_queue ${TEST_LIBRARIES})
target_compile_options(test_mpmc_ring_queue PRIVATE -Wdeclaration-after-statement)
add_test(
    NAME test_mpmc_ring_queue
    COMMAND $<TARGET_FILE:test_mpmc_ring_queue>
)

add_executable(test_mpmc_ring_queue_scale mpmc_ring_queue_scale_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_mpmc_ring_queue_scale ${TEST_LIBRARIES})
target_compile_options(test_mpmc_ring_queue_scale PRIVATE -Wdeclaration-after-statement)
add_test(
    NAME test_mpmc_ring_queue_scale
    COMMAND $<TARGET_FILE:test_mpmc_ring_queue_scale>
)

add_executable(test_queuedef queuedef_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_queuedef ${TEST_LIBRARIES})
add_test(
    NAME test_queuedef
    COMMAND $<TARGET_FILE:test_queuedef>
)

add_executable(test_spinlock spinlock_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_spinlock ${TEST_LIBRARIES})
add_test(
    NAME test_spinlock
    COMMAND $<TARGET_FILE:test_spinlock>
)

add_executable(test_spinlock_scale spinlock_scale_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_spinlock_scale ${TEST_LIBRARIES})
add_test(
    NAME test_spinlock_scale
    COMMAND $<TARGET_FILE:test_spinlock_scale>
)

add_executable(test_spinlock_long spinlock_long_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_spinlock_long ${TEST_LIBRARIES})
add_test(
    NAME test_spinlock_long
    COMMAND $<TARGET_FILE:test_spinlock_scale>
)

add_executable(test_barrier barrier_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_barrier ${TEST_LIBRARIES})
add_test(
    NAME test_barrier
    COMMAND $<TARGET_FILE:test_barrier>
)

if(HAVE_PTHREAD_SPIN)
    add_executable(test_pthread_spin_scale pthread_spin_scale_test.c ${REQUIRED_SOURCES})
    target_link_libraries(test_pthread_spin_scale ${TEST_LIBRARIES})

    add_executable(test_pthread_spin_long pthread_spin_long_test.c ${REQUIRED_SOURCES})
    target_link_libraries(test_pthread_spin_long ${TEST_LIBRARIES})
endif()

add_executable(test_pthread_mutex_scale pthread_mutex_scale_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_pthread_mutex_scale ${TEST_LIBRARIES})

add_executable(test_pthread_mutex_long pthread_mutex_long_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_pthread_mutex_long ${TEST_LIBRARIES})

add_executable(bench_queue queue_bench.c ${REQUIRED_SOURCES})
target_link_libraries(bench_queue ${TEST_LIBRARIES})
