#include <iostream>
#include <cstring>
#include <unistd.h>
//#include <winsock.h>
#include "Matrix.h"
#include "MatrixMarketData.h"

int main() {
    char hostname[255];
//    char *hostname;
//    memset(hostname, 0, 256);

    std::cout << "Hello, World!" << std::endl;

    gethostname(hostname, sizeof(hostname));
//    gethostname(hostname, 256);


    std::cout << "Hello, " << hostname << std::endl;

    Matrix::Matrix *x = new Matrix::Matrix(1, 1);

    delete x;

    MatrixMarketData::MatrixMarketData *mmData = new MatrixMarketData::MatrixMarketData("ash219.mtx");
    delete mmData;

    return 0;


}
