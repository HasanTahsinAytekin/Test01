//
// Created by Hasan Aytekin on 15.07.2023.
//
//#include <iostream>
#include <fstream>
//#include <vector>
#include "GlobalDeclerations.h"
//#include "BitNumber.h"
//#include "MatrixData.h"

#ifndef MATRIXMARKETDATA_MATRIXMARKETDATA_H
#define MATRIXMARKETDATA_MATRIXMARKETDATA_H

namespace Matrix {

    std::ostream& operator << (std::ostream& os, const Header& header);
    std::ostream& operator << (std::ostream& os, const Comment& comment);
    std::ostream& operator << (std::ostream& os, const MatrixSize& matrixSize);
    std::ostream& operator << (std::ostream& os, const MatrixDataCollection& matrixDataCollection);
    std::ostream& operator << (std::ostream& os, const DataRange& dataRange);

    class MatrixMarketData {
        std::string fileName;
        MatrixCollection matrixCollection;

        Header readHeader(std::ifstream &f) {
            Header matrixHeader;

            std::string identifier;
            std::string object;
            std::string format;
            std::string field;
            std::string symmetry;

            if (f >> identifier >> object >> format >> field >> symmetry) {
                matrixHeader.Identifier = identifier;
                matrixHeader.Object = (object == "matrix") ? Matrix :
                                      (object == "vector") ? Vector : UnknownObject;
                matrixHeader.Format = (format == "coordinate") ? Coordinate :
                                      (format == "array") ? Array : UnknownFormat;
                matrixHeader.Field = (field == "real") ? Real :
                                     (field == "double") ? Double :
                                     (field == "complex") ? Complex :
                                     (field == "integer") ? Integer :
                                     (field == "pattern") ? Pattern : UnknownField;
                matrixHeader.Symmetry = (symmetry == "general") ? General :
                                        (symmetry == "symmetric") ? Symmetric :
                                        (symmetry == "skewSymmetric") ? SkewSymmetric :
                                        (symmetry == "hermitian") ? Hermitian : UnknownSymmetry;
                //printHeader(identifier, object, format, field, symmetry);
                //printHeader(matrixHeader);
            }

            return matrixHeader;
        }

        Comment readComment(std::ifstream &f) {
            Comment matrixComment;
            std::string aLine;

            // The current position of the file pointer is on the last line end.
            // Need to advance the current position by one character
            //f.get();
            f.ignore();
            // Resume processing of the comment lines
            while (f.peek() == '%') {
                // Read and Log the entire line
                if (std::getline(f,aLine)) {
                    matrixComment.Text += aLine;
                    matrixComment.Text.push_back('\n');
                }
            }

            return matrixComment;
        }

        MatrixSize readMatrixSize(std::ifstream &f, FieldType fieldType) {
            MatrixSize matrixSize;
            std::int64_t colIndexTotal, rowIndexTotal, dataTotal;

            f >> matrixSize.Rows >> matrixSize.Columns >> matrixSize.NumberOfEntries;

            colIndexTotal = matrixSize.NumberOfEntries * sizeof(std::int64_t);
            rowIndexTotal = matrixSize.NumberOfEntries * sizeof(std::int64_t);

            switch (fieldType) {
                case Real:
                    dataTotal = matrixSize.NumberOfEntries * sizeof(float);
                    break;
                case Double:
                    dataTotal = matrixSize.NumberOfEntries * sizeof(double);
                    break;
                case Complex:
                    dataTotal = 0;
                    break;
                case Integer:
                    dataTotal = matrixSize.NumberOfEntries * sizeof(int);
                    break;
                case Pattern:
                    dataTotal = matrixSize.NumberOfEntries * sizeof(short);
                    break;
                case UnknownField:
                    dataTotal = 0;
                    break;
            }
            matrixSize.SizeInBytes = colIndexTotal + rowIndexTotal + dataTotal;

            return matrixSize;
        }

        template<typename T>
        MatrixData<Data<T>> readMatrixData(std::ifstream &f, MatrixSize matrixSize) {
//            T matrixCellData;
            MatrixData<Data<T>> matrixData;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++) {
                Data <T> newCellData;
                f >> newCellData.Row >> newCellData.Column >> newCellData.Value;
                matrixData.Coordinate.push_back(newCellData);

//                std::cout << "R: " << newCellData.Row << " - C: " << newCellData.Column << " - D: " << newCellData.Value << std::endl;
            }

            return matrixData;
        }

        MatrixData<Data<short>> readMatrixData_Pattern(std::ifstream &f, MatrixSize matrixSize) {
            // For FieldType = Pattern, no value fields exist. All the coordinate values are assumed to be 1.
            MatrixData<Data<short>> matrixData;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++)
            {
             /*   int row, column;
                f >> row >> column;

                std::cout << "R: " << row << " - C: " << column << " - D: " << matrixCellData << std::endl;*/
                Data <short> newCellData;
                f >> newCellData.Row >> newCellData.Column;
                newCellData.Value = 1;
                matrixData.Coordinate.push_back(newCellData);

//                std::cout << "R: " << newCellData.Row << " - C: " << newCellData.Column << " - D: " << newCellData.Value << std::endl;
            }

            return matrixData;
        }

        template <typename T>
        DataRange getDataRange(MatrixData<Data<T>> matrixData) {
            DataRange myRange;

            myRange.Maximum = matrixData.Coordinate[0].Value;
            myRange.Minimum = matrixData.Coordinate[0].Value;

            for(Data<T> data: matrixData.Coordinate) {
                if (data.Value > myRange.Maximum) {
                    myRange.Maximum = data.Value;
                }
                else {
                    if (data.Value < myRange.Minimum) {
                        myRange.Minimum = data.Value;
                    }
                }
            }

            return myRange;
        }

        MatrixDataCollection readMatrixData(std::ifstream &f, MatrixSize matrixSize, FieldType fieldType) {
            MatrixDataCollection matrixDataCollection;

            switch (fieldType) {
                case Real:
                    matrixDataCollection.type_float = readMatrixData <float> (f, matrixSize);
                    matrixDataCollection.type_data_range = getDataRange<float>(matrixDataCollection.type_float);
                    break;
                case Double:
                    matrixDataCollection.type_double = readMatrixData <double> (f, matrixSize);
                    matrixDataCollection.type_data_range = getDataRange<double>(matrixDataCollection.type_double);
                    break;
                case Complex:
                    break;
                case Integer:
                    matrixDataCollection.type_int = readMatrixData <int> (f, matrixSize);
                    matrixDataCollection.type_data_range = getDataRange<int>(matrixDataCollection.type_int);
                    break;
                case Pattern:
                    matrixDataCollection.type_pattern = readMatrixData_Pattern(f, matrixSize);
                    matrixDataCollection.type_data_range = getDataRange<short>(matrixDataCollection.type_pattern);
                    break;
                case UnknownField:
                    break;
            }

            return matrixDataCollection;
        }

        void readMatrix(std::string file_name) {
            std::string aLine;
            std::ifstream f(file_name);

            // Read the header
/*
            Header matrixHeader = readHeader(f);
            Comment matrixComment = readComment(f);
            MatrixSize matrixSize = readMatrixSize(f, matrixHeader.Field);
*/
            matrixCollection.matrix_header = readHeader(f);
            matrixCollection.matrix_comment = readComment(f);
            matrixCollection.matrix_size = readMatrixSize(f, matrixCollection.matrix_header.Field);
            matrixCollection.matrixDataCollection = readMatrixData(f, matrixCollection.matrix_size, matrixCollection.matrix_header.Field);

//            std::cout << matrixCollection.matrix_header << std::endl;
//            std::cout <<  matrixCollection.matrix_comment << std::endl;
//            std::cout << matrixCollection.matrix_size << std::endl;
//            std::cout <<  matrixCollection.matrixDataCollection.type_data_range << std::endl;
//            std::cout <<  matrixCollection.matrixDataCollection << std::endl;
        }

    public:
        explicit MatrixMarketData(std::string FileName) {
            fileName = FileName;
            readMatrix(FileName);
        }

        MatrixCollection& GetMatrixDataCollection() {
            return this->matrixCollection;
        }

        unsigned long GetMatrixSizeInBytes() {
            return this->matrixCollection.matrix_size.SizeInBytes;
        }

        MatrixSize GetMatrixSize() {
            return this->matrixCollection.matrix_size;
        }

        ~MatrixMarketData(){
            // Destructor of the class
//            std::cout << "MatrixMarketData destructor ...\n";
        }
    };

} // Matrix

#endif //MATRIXMARKETDATA_MATRIXMARKETDATA_H
