cmake_minimum_required(VERSION 3.10)
project(fhe_i)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(helib REQUIRED)

add_executable(fhe_i try.cpp)

target_link_libraries(fhe_i PRIVATE helib)
