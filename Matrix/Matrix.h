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
        int total;
        int dimension = 1000;
        float* data;
    public:
        template <typename T>
        struct Cell {
            int row;
            int col;
            T val;
        };

        Matrix(int rows, int columns) : rows(rows), columns(columns), total(rows*columns){
            Cell<int> matrixCell;
            matrixCell.col = 1;
            matrixCell.row = 5;
            matrixCell.val = 7;

            std::cout << "Matrix with " << matrixCell.row << " rows and " << matrixCell.col << " columns ....\n";

            //Aligned malloc for cache line size ??????????????????????
            data = (float *) aligned_alloc(1024, 1024 * sizeof data);
        }

        ~Matrix() {
            std::cout << "Matrix destructor ...\n";

            free(data);
        }

        void Set(int col, int row, double v) {
            dataCol[col + row * dimension] = v;
            dataRow[row + col + dimension] = v;
        }
        float GetCol(int col, int row) {
            return dataCol[col + row * dimension];
        }

        float GetRow(int col, int row) {
            return dataRow[row + col * dimension];
        }
        // Override the () operator for the Matrix class
        float& operator()(int row, int col) {
            return data[row + col * rows];
        }

        void MOV(Matrix& dest, Matrix& src) {
            for(int i = 0; i < dest.dimension * dest.dimension; i++) {
                dest.dataRow[i] = src.dataRow[i];
                dest.dataCol[i] = src.dataCol[i];
            }
        }

        void MUL3(Matrix& dest, Matrix& src1, Matrix& src2) {   // Cashed
            int dim = dest.dimension;
            Matrix* tmp = new Matrix(dest.dimension);

            int blockSize = 100; // Related to dimension !!!!!

            // Compute the matrix product
            for(int row = 0; row < dim; row+=blockSize) {
                for (int col = 0; col < dim; col+=blockSize) {

                    for(int blockRow = row; blockRow < row + blockSize; blockRow++) {
                        for(int blockCol = col; blockCol < col + blockSize; blockCol++) {
                            double result = 0.0;

                            // Compute the dor product of row src1 and col src2
                            for (int dot = 0; dot < dim; dot++) {
                                result += src1.GetCol(dot, blockRow) * src2.GetRow(blockCol, dot);
                            }

                            tmp->Set(blockCol, blockRow, result);
                        }
                    }
                }
            }
        }

        void MUL2(Matrix& dest, Matrix& src1, Matrix& src2) {
            int dim = dest.dimension;
            Matrix* tmp = new Matrix(dest.dimension);

            // Compute the matrix product
            for(int row = 0; row < dim; row++) {
                for (int col = 0; col < dim; col++) {
                    double result = 0.0;

                    // Compute the dor product of row src1 and col src2
                    for (int dot = 0; dot < dim; dot++) {
                        result += src1.GetCol(dot, row) * src2.GetRow(col, dot);
                    }

                    tmp->Set(col, row, result);
                }
            }
        }

        void MUL1(Matrix& a, Matrix& b) {
            if (a.columns != b.rows)
                throw "MULTIPLICATION: Columns of matrix A must be equal to the rows of matrix B";

            if (this == &a || this == &b)
                throw "MULTIPLICATION: Destination matrix must be distinct from matrices A and B";

            int sharedDim = a.columns;

            for (int r = 0; r < a.rows; r++) {
                for (int c = 0; c < b.columns; c++) {
                    float total = 0.0f;
                    for (int d = 0; d < sharedDim; d++) {
                        total += a(r, d) * b(d, c);
                    }

                    this->operator()(r, c) = total;
                }
            }
        }

        void Hadamard(Matrix &a) {
            if (this->rows != a.rows || this->columns != a.columns)
                throw "HADAMARD: Matrices must be of the same dimensions to Hadamard";

            for (int i = 0; i < this->total; i++) {
                data[i] *= a.data[i];
            }
        }

        void ADD(Matrix &a) {
            if (this->rows != a.rows || this->columns != a.columns)
                throw "ADD: Matrices must be of the same dimensions to ADD";

            for (int i = 0; i < this->total; i++) {
                data[i] += a.data[i];
            }
        }

        void SUB(Matrix &a) {
            if (this->rows != a.rows || this->columns != a.columns)
                throw "SUB: Matrices must be of the same dimensions to SUB";

            for (int i = 0; i < this->total; i++) {
                data[i] -= a.data[i];
            }
        }

    };  // Matrix

#endif //MATRIX_MATRIX_H
}