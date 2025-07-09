cmake_minimum_required(VERSION 3.10)
project(fhe_masked_membership)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cari HElib dari instalasi yang sudah benar
find_package(helib REQUIRED)

# Tambahkan executable
add_executable(fhe_masked_membership try.cpp)

# Tautkan dengan helib biasa (bukan helib::helib)
target_link_libraries(fhe_masked_membership PRIVATE helib)
