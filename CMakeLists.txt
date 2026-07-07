# Require a modern version of CMake
cmake_minimum_required(VERSION 3.15)

# Define the project name and the language
project(CppBenchmarkLab LANGUAGES CXX)

# Strictly enforce the C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define your executable target and point it to your source file
add_executable(benchmark_runner src/benchmark/main.cpp)

#defining libraries
add_library(utils STATIC src/utils/Timer.cpp "src/algorithms/VectorMath_baseline.cpp" "src/algorithms/VectorMath_reserving.cpp" "src/utils/BenchMarkReporter.cpp" "src/algorithms/VectorMath_prealloc.cpp" "src/algorithms/MatrixMath_baseline.cpp" "src/algorithms/MatrixMath_loopReordered.cpp")

# Tell the executable where to find header files
target_include_directories(benchmark_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#linking libraries to the executable
target_link_libraries(benchmark_runner PRIVATE utils)



