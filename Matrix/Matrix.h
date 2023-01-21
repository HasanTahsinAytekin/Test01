//
// Created by Hasan Aytekin on 14.01.2023.
//

#include <iostream>

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

namespace Matrix {
    class Matrix {
    private:
        int rows;
        int columns;
    public:
        template <typename T>
        struct Cell {
            int row;
            int col;
            T val;
        };

        Matrix(int rows, int columns) : rows(rows), columns(columns) {
            Cell<int> matrixCell;
            matrixCell.col = 1;
            matrixCell.row = 5;
            matrixCell.val = 7;

            std::cout << "Matrix with " << matrixCell.row << " rows and " << matrixCell.col << " columns ....\n";
        }

        ~Matrix() {
            std::cout << "Matrix destructor ...\n";
        }
};  // Matrix

#endif //MATRIX_MATRIX_H
}