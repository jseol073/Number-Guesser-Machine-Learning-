cmake_minimum_required(VERSION 3.5)
project(naivebayes)

set(CMAKE_CXX_STANDARD 14)

add_executable(naivebayes src/main.cpp src/BinaryImage.cpp src/Training.cpp src/Training.h)
add_executable(unit-tests test/test.cpp test/catch.hpp src/Training.cpp src/BinaryImage.cpp)
