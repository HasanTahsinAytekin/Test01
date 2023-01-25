#include <iostream>
#include <cstring>
#include <unistd.h>
//#include <winsock.h>
#include "Matrix.h"
#include "MatrixMarketData.h"

#define GetCurrentDir getcwd
int print_current_working_directory_linux(){
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return errno;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    printf ("The current working directory is %s", cCurrentPath);

    return 0;
}

struct F32 {
    union {
        float fValue;

        struct {
            unsigned int mantissa: 23;
            unsigned int exponent: 8;
            unsigned int sign: 1;
        };
    };

    F32(float f) {
        fValue = f;
    };
};

int main() {
    char hostname[255];
//    char *hostname;
//    memset(hostname, 0, 256);

    std::cout << "Hello, World!" << std::endl;

    F32 f(25.0f);

    std::cout << f.fValue << std::endl;
    f.sign = !f.sign;
    std::cout << f.fValue << std::endl;
    f.sign = !f.sign; //revert to original
    f.exponent++;
    std::cout << f.fValue << std::endl;
    std::cout << "Mantissa: " << f.mantissa << std::endl;
    std::cout << "Exponent: " << f.exponent << std::endl;
    std::cout << "Sign: " << f.sign << std::endl;

    gethostname(hostname, sizeof(hostname));

    std::cout << "Hello, " << hostname << std::endl;

    Matrix::Matrix *x = new Matrix::Matrix(1, 1);

    delete x;

//    MatrixMarketData::MatrixMarketData *mmData = new MatrixMarketData::MatrixMarketData("C:\\CLionProjects\\Test\\Matrix\\ash219.mtx");
    MatrixMarketData::MatrixMarketData *mmData = new MatrixMarketData::MatrixMarketData(
            "/mnt/c/CLionProjects/Test/Matrix/cmake-build-debug-wsl/ash219.mtx");
    delete mmData;

    print_current_working_directory_linux();

    return 0;
}
