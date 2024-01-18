//
// Created by Hasan Aytekin on 15.07.2023.
//
//#include <iostream>
#include <fstream>
#include <iostream>
#include <algorithm>
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

    // Compares two Data structures by row and column values.
    template<typename T>
    bool compareCoordinateRows(const Data<T>& d1, const Data<T>& d2)
    {
        if (d1.Row < d2.Row) {
            // Row1 < Row2
            return true;
        }
        else if (d1.Row > d2.Row) {
            // Row1 > Row2
            return false;
        }
        else if (d1.Column < d2.Column) {
            // Row1 = Row2 and Column1 < Column2
            return true;
        }
        else {
            // Row1 = Row2 and Column1 >= Column2
            return false;
        }
    }

    class MatrixMarketData {

/*
        int GetDataTypeLengthForIndexing(unsigned long aValue) {
            if (aValue <= 65535) { // 2 bytes
                return sizeof(unsigned short);
            } else if (aValue <= 4294967295) { // 4 bytes
                return sizeof(unsigned int);
            } else if (aValue <= ((unsigned long)18446744073709551615)) { // 8 bytes
                return sizeof(unsigned long);
            }
        }
*/
//        const bool DisplayResult = true;
//        const bool DisplayProgress = true;
//        double defaultSegmentSize = 65535; // double -> ceil function requires double


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

/*
        int getDataTypeLengthForIndexing(unsigned long aValue) {
            if (aValue <= 65535) { // 2 bytes
                return sizeof(unsigned short);
            } else if (aValue <= 4294967295) { // 4 bytes
                return sizeof(unsigned int);
            } else if (aValue <= ((unsigned long)18446744073709551615)) { // 8 bytes
                return sizeof(unsigned long);
            }
        }
*/

        MatrixSize readMatrixSize(std::ifstream &f, FieldType fieldType, SymmetryType symmetryType) {
            MatrixSize matrixSize;
//            std::int64_t colIndexTotal, rowIndexTotal, dataTotal;
            long long int colIndexTotal, rowIndexTotal, dataTotal;

            f >> matrixSize.Rows >> matrixSize.Columns >> matrixSize.NumberOfEntries;

            long long int numOfEntries = (symmetryType == Symmetric) ? 2 * matrixSize.NumberOfEntries : matrixSize.NumberOfEntries;


//            if (symmetryType == Symmetric) matrixSize.NumberOfEntries *= 2;

////            colIndexTotal = matrixSize.NumberOfEntries * sizeof(long long int);
////            rowIndexTotal = matrixSize.NumberOfEntries * sizeof(long long int);
//            colIndexTotal = matrixSize.NumberOfEntries * getDataTypeLengthForIndexing(matrixSize.Columns);
//            rowIndexTotal = matrixSize.NumberOfEntries * getDataTypeLengthForIndexing(matrixSize.Rows);
//            colIndexTotal = numOfEntries * getDataTypeLengthForIndexing(matrixSize.Columns);
//            rowIndexTotal = numOfEntries * getDataTypeLengthForIndexing(matrixSize.Rows);
            matrixSize.ColumnIndexSizeInBytes = Utilities::GetDataTypeLengthForIndexing(matrixSize.Columns);
            matrixSize.RowIndexSizeInBytes = Utilities::GetDataTypeLengthForIndexing(matrixSize.Rows);
            colIndexTotal = numOfEntries * matrixSize.ColumnIndexSizeInBytes;
            rowIndexTotal = numOfEntries * matrixSize.RowIndexSizeInBytes;

            switch (fieldType) {
                case Real:
//                    dataTotal = matrixSize.NumberOfEntries * sizeof(float);
                    matrixSize.ValueSizeInBytes = sizeof(float);
//                    dataTotal = numOfEntries * matrixSize.ValueSizeInBytes;
                    break;
                case Double:
//                    dataTotal = matrixSize.NumberOfEntries * sizeof(double);
                    matrixSize.ValueSizeInBytes = sizeof(double);
//                    dataTotal = numOfEntries * matrixSize.ValueSizeInBytes;
                    break;
                case Complex:
                    // NIY
                    matrixSize.ValueSizeInBytes = -1;
//                    dataTotal = 0;
                    break;
                case Integer:
//                    dataTotal = matrixSize.NumberOfEntries * sizeof(int);
                    matrixSize.ValueSizeInBytes = sizeof(int);
//                    dataTotal = numOfEntries * matrixSize.ValueSizeInBytes;
                    break;
                case Pattern:
//                    dataTotal = matrixSize.NumberOfEntries * sizeof(short);
                    matrixSize.ValueSizeInBytes = sizeof(short);
//                    dataTotal = numOfEntries * matrixSize.ValueSizeInBytes;
                    break;
                case UnknownField:
                    matrixSize.ValueSizeInBytes = -1;
//                    dataTotal = 0;
                    break;
            }
            dataTotal = numOfEntries * matrixSize.ValueSizeInBytes;
            matrixSize.SizeInBytes = colIndexTotal + rowIndexTotal + dataTotal;

//            if (symmetryType == Symmetric) matrixSize.SizeInBytes *= 2;

            return matrixSize;
        }

        template<typename T>
        MatrixData<Data<T>> readMatrixData(std::ifstream &f, MatrixSize matrixSize, SymmetryType symmetryType) {
//            T matrixCellData;
            MatrixData<Data<T>> matrixData;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++) {
                Data <T> newCellData;
                f >> newCellData.Row >> newCellData.Column >> newCellData.Value;
                matrixData.Coordinate.push_back(newCellData);

//                std::cout << "R: " << newCellData.Row << " - C: " << newCellData.Column << " - D: " << newCellData.Value << std::endl;

                if ((symmetryType == Symmetric) || (symmetryType == SkewSymmetric)) {
                    if (newCellData.Row != newCellData.Column) {
                        Data<T> symmetricCellData;
                        symmetricCellData.Row = newCellData.Column;
                        symmetricCellData.Column = newCellData.Row;
                        symmetricCellData.Value = newCellData.Value;
                        matrixData.Coordinate.push_back(symmetricCellData);
//                    std::cout << "R: " << symmetricCellData.Row << " - C: " << symmetricCellData.Column << " - D: " << symmetricCellData.Value << std::endl;
                    }
                } else if (symmetryType == Hermitian) {
                    if (newCellData.Row != newCellData.Column) {
                        // Do not deal with complex value here (symmetricCellData.Value = std::conj(symmetricCellData.Value) (ax+b -> conj -> ax-b)
                        Data<T> symmetricCellData;
                        symmetricCellData.Row = newCellData.Column;
                        symmetricCellData.Column = newCellData.Row;
                        symmetricCellData.Value = newCellData.Value;
                        matrixData.Coordinate.push_back(symmetricCellData);
                    }

                }
            }

            // Sort the contents of the matrix for further processing (re-ordering the vector after symmetric etc. additions...)
            std::sort(matrixData.Coordinate.begin(), matrixData.Coordinate.end(), compareCoordinateRows<T>);

            return matrixData;
        }

        MatrixData<Data<short>> readMatrixData_Pattern(std::ifstream &f, MatrixSize matrixSize, SymmetryType symmetryType) {
            // For FieldType = Pattern, no value fields exist. All the coordinate values are assumed to be 1.
            MatrixData<Data<short>> matrixData;

            Data <short> newCellData;
            Data<short> symmetricCellData;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++)
            {
             /*   int row, column;
                f >> row >> column;

                std::cout << "R: " << row << " - C: " << column << " - D: " << matrixCellData << std::endl;*/
//                Data <short> newCellData;
                f >> newCellData.Row >> newCellData.Column;
                newCellData.Value = 1;
                matrixData.Coordinate.push_back(newCellData);

//                if (newCellData.Column == 1 && newCellData.Row == 82653){
//                    int xxxx;
//                    xxxx=0;
//                }
//                std::cout << "R: " << newCellData.Row << " - C: " << newCellData.Column << " - D: " << newCellData.Value << std::endl;
                if (symmetryType == Symmetric) {
//                    Data<short> symmetricCellData;
                    symmetricCellData.Row = newCellData.Column;
                    symmetricCellData.Column = newCellData.Row;
                    symmetricCellData.Value = newCellData.Value;
                    matrixData.Coordinate.push_back(symmetricCellData);
//                    std::cout << "R: " << symmetricCellData.Row << " - C: " << symmetricCellData.Column << " - D: " << symmetricCellData.Value << std::endl;
                }
            }

            // Done in MatrixCSR
//            if (symmetryType == Symmetric) {
//                // Sort the vector for symmetric repetitions!
//            }

            // Sort the contents of the matrix for further processing (re-ordering the vector after symmetric etc. additions...)
            std::sort(matrixData.Coordinate.begin(), matrixData.Coordinate.end(), compareCoordinateRows<short>);

            return matrixData;
        }

        template <typename T>
        DataRange getDataRange(MatrixData<Data<T>>& matrixData) {
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

        MatrixDataCollection readMatrixData(std::ifstream &f, MatrixSize matrixSize, FieldType fieldType, SymmetryType symmetryType) {
            MatrixDataCollection matrixDataCollection;

            switch (fieldType) {
                case Real:
                    matrixDataCollection.type_float = readMatrixData <float> (f, matrixSize, symmetryType);
                    matrixDataCollection.type_data_range = getDataRange<float>(matrixDataCollection.type_float);
                    break;
                case Double:
                    matrixDataCollection.type_double = readMatrixData <double> (f, matrixSize, symmetryType);
                    matrixDataCollection.type_data_range = getDataRange<double>(matrixDataCollection.type_double);
                    break;
                case Complex:
                    break;
                case Integer:
                    matrixDataCollection.type_int = readMatrixData <int> (f, matrixSize, symmetryType);
                    matrixDataCollection.type_data_range = getDataRange<int>(matrixDataCollection.type_int);
                    break;
                case Pattern:
                    matrixDataCollection.type_pattern = readMatrixData_Pattern(f, matrixSize, symmetryType);
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
            matrixCollection.matrix_size = readMatrixSize(f, matrixCollection.matrix_header.Field, matrixCollection.matrix_header.Symmetry);
            matrixCollection.matrixDataCollection = readMatrixData(f, matrixCollection.matrix_size, matrixCollection.matrix_header.Field, matrixCollection.matrix_header.Symmetry);

//            std::cout << matrixCollection.matrix_header << std::endl;
//            std::cout <<  matrixCollection.matrix_comment << std::endl;
//            std::cout << matrixCollection.matrix_size << std::endl;
//            std::cout <<  matrixCollection.matrixDataCollection.type_data_range << std::endl;
//            std::cout <<  matrixCollection.matrixDataCollection << std::endl;
        }

    public:
        explicit MatrixMarketData(std::string FileName) {

//            if (Constants::DisplayProgress) std::cout << FileName << " -> Loading...";

            fileName = FileName;
            readMatrix(FileName);

//            if (Constants::DisplayProgress) std::cout << "Done";
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
//            if (Constants::DisplayProgress) std::cout << fileName << " Destroyed" << '\r';
        }
    };

} // Matrix

#endif //MATRIXMARKETDATA_MATRIXMARKETDATA_H
