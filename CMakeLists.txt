cmake_minimum_required(VERSION 3.2)

project(duplicate_search)
set(MAIN_SRC main.cpp)
set(MAIN_EXE duplicate_search)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
