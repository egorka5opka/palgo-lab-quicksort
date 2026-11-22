add_library(quicksort STATIC sort.cpp)
target_compile_features(quicksort PUBLIC cxx_std_17)
target_include_directories(quicksort PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(quicksort PUBLIC parlay)
