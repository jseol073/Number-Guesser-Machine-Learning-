cmake_minimum_required(VERSION 3.5)
project(naivebayes)

set(CMAKE_CXX_STANDARD 14)

add_executable(naivebayes src/main.cpp src/BinaryImage.cpp)
add_executable(unit-tests test/test.cpp test/catch.hpp src/main.cpp)
