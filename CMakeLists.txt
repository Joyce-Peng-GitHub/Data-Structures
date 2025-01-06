cmake_minimum_required(VERSION 3.8)

PROJECT(Data_Structure)

set(CMAKE_CXX_STANDARD 20)

include_directories(ds)

set(SOURCE_FILES
        test/test_fenwicktree.cpp)

add_executable(Data_Structure ${SOURCE_FILES})