if(BUILD_TESTING)
    add_executable(sort_correctness correctness.cpp)
    target_link_libraries(sort_correctness PRIVATE quicksort)

    foreach(threads IN ITEMS 1 4)
        add_test(NAME sort_correctness_${threads}_threads COMMAND sort_correctness)
        set_tests_properties(sort_correctness_${threads}_threads PROPERTIES
            ENVIRONMENT "PARLAY_NUM_THREADS=${threads}"
            PROCESSORS ${threads}
            TIMEOUT 60
        )
    endforeach()
endif()

if(BUILD_BENCHMARKS)
    add_executable(sort_benchmark benchmark.cpp)
    target_link_libraries(sort_benchmark PRIVATE quicksort)
endif()
