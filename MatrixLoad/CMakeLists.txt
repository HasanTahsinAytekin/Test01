cmake_minimum_required(VERSION 3.26)
project(MatrixLoad)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(MatrixLoad main.cpp
        GlobalDeclerations.h
        MatrixCSR.cpp
        MatrixMarketData.cpp
        BitNumber.cpp
        SegmentedMatrixCSR.cpp
        MatrixCSC.cpp
        SegmentedMatrixCSC.cpp
        MatrixTimingBenchmark.cpp
        MatrixTimingBenchmark.h
        MatrixUtilities.cpp
        MatrixUtilities.h)
