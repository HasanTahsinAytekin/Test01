#include <iostream>
#include "MatrixMarketData.h"

int main() {
/*    auto mmData = new Matrix::MatrixMarketData(
            "/mnt/c/CLionProjects/Test/MatrixMarketData/ash219.mtx");*/
    //auto mmData = new Matrix::MatrixMarketData("C:\\CLionProjects\\Test\\MatrixMarketData\\ash219.mtx");
    //auto mmData = new Matrix::MatrixMarketData("C:\\CLionProjects\\Test\\MatrixMarketData\\130bit.mtx");
    auto mmData = new Matrix::MatrixMarketData("C:\\CLionProjects\\Test\\MatrixMarketData\\lpi_cplex2.mtx");
    delete mmData;

    return 0;
}
