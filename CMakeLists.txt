# Require a modern version of CMake
cmake_minimum_required(VERSION 3.15)

# Define the project name and the language
project(CppBenchmarkLab LANGUAGES CXX)

# Strictly enforce the C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define your executable target and point it to your source file
add_executable(benchmark_runner src/benchmark/main.cpp)

# Tell the executable where to find header files
target_include_directories(benchmark_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)