//
// Created by Hasan Aytekin on 20.07.2023.
//
#include <iostream>
#include <fstream>
//#include <vector>
#include <cstdint>
#include "GlobalDeclerations.h"

#ifndef MATRIXMARKETDATA_MATRIXDATA_H
#define MATRIXMARKETDATA_MATRIXDATA_H

namespace Matrix {

    template <typename MatrixDataType>
    class MatrixCoordinateData {
    private:
        std::ifstream& file;

        struct Data {
            std::int64_t Row;
            std::int64_t Column;
            MatrixDataType Value;
        };

        struct MatrixData {
            std::vector<Data> Coordinate;
        };

        MatrixData readMatrixData(std::ifstream &f, MatrixSize matrixSize, SymmetryType symmetryType) {
//            MatrixDataType matrixCellData;
            MatrixData matrixData;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++) {
                Data newCellData;
                f >> newCellData.Row >> newCellData.Column >> newCellData.Value;
                matrixData.Coordinate.push_back(newCellData);
                std::cout << "R: " << newCellData.Row << " - C: " << newCellData.Column << " - D: " << newCellData.Value << std::endl;

                if (symmetryType == Symmetric) {
                    Data symmetricCellData;
                    symmetricCellData.Row = newCellData.Column;
                    symmetricCellData.Column = newCellData.Row;
                    symmetricCellData.Value = newCellData.Value;
                    matrixData.Coordinate.push_back(symmetricCellData);
                    std::cout << "R: " << symmetricCellData.Row << " - C: " << symmetricCellData.Column << " - D: " << symmetricCellData.Value << std::endl;
                }
            }

            return matrixData;
        }

        MatrixData readMatrixData_Pattern(std::ifstream &f, MatrixSize matrixSize) {
            // For FieldType = Pattern, no value fields exist. All the coordinate values are assumed to be 1.
            MatrixData matrixData;
            auto matrixCellData = 1;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++)
            {
                Data newCellData;
                f >> newCellData.Row >> newCellData.Column;
                newCellData.Value = 1;
                matrixData.Coordinate.push_back(newCellData);

                std::cout << "R: " << newCellData.Row << " - C: " << newCellData.Column << " - D: " << newCellData.Value << std::endl;
            }

            return matrixData;
        }

        void readMatrixData(std::ifstream &f, MatrixSize matrixSize, FieldType fieldType, SymmetryType symmetryType) {
            MatrixData m_data;

            switch (fieldType) {
                case Real:
                case Double:
                case Integer:
                    m_data = readMatrixData(f, matrixSize);
                    break;
                case Complex:
                    break;
                case Pattern:
                    m_data = readMatrixData_Pattern(f, matrixSize);
                    break;
                case UnknownField:
                    break;
            }
        }

    public:
        MatrixCoordinateData(std::ifstream &f) : file(f) {
            int x1,x2,x3;
            file>>x1>>x2>>x3;

            std::cout << "MatrixCoordinateData" << std::endl;
        }

        MatrixCoordinateData(std::ifstream &f, MatrixSize matrixSize, FieldType fieldType) : file(f) {
            readMatrixData(file, matrixSize, fieldType);
        }

        ~MatrixCoordinateData(){
            // Destructor of the class
            std::cout << "MatrixCoordinateData destructor ...\n";
        }

    };

} // Matrix

#endif //MATRIXMARKETDATA_MATRIXDATA_H
