set -e

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
PARLAY_NUM_THREADS=4 ./build/test/sort_benchmark 100000000 5