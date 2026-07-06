# 1. Require a modern version of CMake
cmake_minimum_required(VERSION 3.15)

# 2. Define the project name and the language (CXX means C++)
project(CppBenchmarkLab LANGUAGES CXX)

# 3. Strictly enforce the C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. Define your executable target and point it to your source file
add_executable(benchmark_runner src/benchmark/main.cpp)

# 5. Tell the executable where to find header files (when we create them later)
target_include_directories(benchmark_runner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)