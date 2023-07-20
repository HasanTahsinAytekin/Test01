cmake_minimum_required(VERSION 3.25)
project(MatrixMarketData)

set(CMAKE_CXX_STANDARD 23)

add_executable(MatrixMarketData main.cpp MatrixMarketData.cpp MatrixMarketData.h)
