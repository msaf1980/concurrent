# Scan source dir for standart source files and exclude main.cpp

include_directories(${PROJECT_SOURCE_DIR}/src)

set(REQUIRED_SOURCES)

if(NOT HAVE_PTHREAD_BARRIER)
    add_definitions(-DNO_PTHREAD_BARRIER=1)
    list(APPEND REQUIRED_SOURCES ${PROJECT_SOURCE_DIR}/src/pthread_barrier.c)
endif()

set(TEST_LIBRARIES concurrent Threads::Threads)

# Build tests
add_executable(test_mpmc_ring_queue mpmc_ring_queue_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_mpmc_ring_queue ${TEST_LIBRARIES})
target_compile_options(test_mpmc_ring_queue PRIVATE -Wdeclaration-after-statement)
add_test(
    NAME test_mpmc_ring_queue
    COMMAND $<TARGET_FILE:test_mpmc_ring_queue>
)
set_tests_properties(test_mpmc_ring_queue PROPERTIES LABELS "mpmc_ring_queue")

add_executable(test_mpmc_ring_queue_scale mpmc_ring_queue_scale_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_mpmc_ring_queue_scale ${TEST_LIBRARIES})
target_compile_options(test_mpmc_ring_queue_scale PRIVATE -Wdeclaration-after-statement)
add_test(
    NAME test_mpmc_ring_queue_scale
    COMMAND $<TARGET_FILE:test_mpmc_ring_queue_scale>
)
set_tests_properties(test_mpmc_ring_queue_scale PROPERTIES LABELS "mpmc_ring_queue_scale")

add_executable(test_queuedef queuedef_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_queuedef ${TEST_LIBRARIES})
add_test(
    NAME test_queuedef
    COMMAND $<TARGET_FILE:test_queuedef>
)
set_tests_properties(test_queuedef PROPERTIES LABELS "queuedef")

add_executable(test_spinlock spinlock_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_spinlock ${TEST_LIBRARIES})
add_test(
    NAME test_spinlock
    COMMAND $<TARGET_FILE:test_spinlock>
)
set_tests_properties(test_spinlock PROPERTIES LABELS "spinlock")

add_executable(test_spinlock_scale spinlock_scale_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_spinlock_scale ${TEST_LIBRARIES})
add_test(
    NAME test_spinlock_scale
    COMMAND $<TARGET_FILE:test_spinlock_scale>
)
set_tests_properties(test_spinlock_scale PROPERTIES LABELS "spinlock_scale")

add_executable(test_spinlock_long spinlock_long_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_spinlock_long ${TEST_LIBRARIES})
add_test(
    NAME test_spinlock_long
    COMMAND $<TARGET_FILE:test_spinlock_scale>
)
set_tests_properties(test_spinlock_long PROPERTIES LABELS "spinlock_long")

add_executable(test_barrier barrier_test.c ${REQUIRED_SOURCES})
target_link_libraries(test_barrier ${TEST_LIBRARIES})
add_test(
    NAME test_barrier
    COMMAND $<TARGET_FILE:test_barrier>
)
set_tests_properties(test_barrier PROPERTIES LABELS "barrier")

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
