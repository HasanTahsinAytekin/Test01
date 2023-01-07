#include <iostream>

using namespace std;

void dynamicMemoryMultiDimensionalArray() {
    int rows, cols;
    cout << "rows, cols:";
    cin >> rows >> cols;
    cout << "rows: " << rows << ", cols:" << cols << endl;

    int **table = new int * [rows];

    for(int i=0; i<rows; i++) {
        table[i] = new int[cols];
    }

    table[1][2] = 88;

    cout << "table[1][2]: " << table[1][2] << endl;

    // Garbage Collection
    for(int i=0; i<rows; i++) {
        delete[] table[i];
    }
    delete[] table;

    table = nullptr;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    dynamicMemoryMultiDimensionalArray();

    system("pause>0");

    return 0;
}
