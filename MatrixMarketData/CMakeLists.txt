cmake_minimum_required(VERSION 3.25)
project(MatrixMarketData)

set(CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(MatrixMarketData main.cpp MatrixMarketData.cpp MatrixMarketData.h Matrix.cpp Matrix.h GlobalDeclerations.h MatrixCSR.cpp MatrixCSR.h SegmentedMatrixCSR.cpp SegmentedMatrixCSR.h BitNumber.cpp BitNumber.h)
