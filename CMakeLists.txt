cmake_minimum_required(VERSION 3.10)
project(fhe_masked_membership)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(helib REQUIRED)

add_executable(fhe_masked_membership try.cpp)

target_link_libraries(fhe_masked_membership PRIVATE helib)
