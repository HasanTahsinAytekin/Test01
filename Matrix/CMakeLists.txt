cmake_minimum_required(VERSION 3.16)
project(Matrix)

set(CMAKE_CXX_STANDARD 17)

add_executable(Matrix main.cpp Matrix.cpp Matrix.h MatrixMarketData.cpp MatrixMarketData.h)
