//
// Created by Hasan Aytekin on 15.07.2023.
//
#include <iostream>
#include <fstream>

#ifndef MATRIXMARKETDATA_MATRIXMARKETDATA_H
#define MATRIXMARKETDATA_MATRIXMARKETDATA_H

namespace Matrix {

    enum ObjectType {
        Matrix,
        Vector,
        UnknownObject
    };
    enum FormatType {
        Coordinate,
        Array,
        UnknownFormat
    };
    enum FieldType {
        Real,
        Double,
        Complex,
        Integer,
        Pattern,
        UnknownField
    };
    enum SymmetryType {
        General,
        Symmetric,
        SkewSymmetric,
        Hermitian,
        UnknownSymmetry
    };

    class MatrixMarketData {
        std::string fileName;
        //
        // Matrix Market Characteristics
        //
        struct Header {
            std::string Identifier;
            ObjectType Object;
            FormatType Format;
            FieldType Field;
            SymmetryType Symmetry;
        };
        struct Comment {
            std::string Text;
        };
        struct MatrixSize {
            std::int64_t Rows;
            std::int64_t Columns;
            std::int64_t NumberOfEntries;
        };
        template<typename T>
        struct Data {
            std::int64_t Row;
            std::int64_t Column;
            T Value;
        };

        friend std::ostream& operator << (std::ostream& os, const Header& header)
        {
            //return os << header.Identifier  << header.Object << header.Field <<   header.Format << header.Symmetry << std::endl;

            os << "Identifier: " << header.Identifier << std::endl;

            os << "Object: ";
            switch (header.Object) {
                case Matrix:
                    os << "Matrix" << std::endl;
                    break;
                case Vector:
                    os << "Vector" << std::endl;
                    break;
                case UnknownObject:
                    os << "UnknownObject" << std::endl;
                    break;
            }

            os << "Format: ";
            switch (header.Format) {
                case Coordinate:
                    os << "Coordinate" << std::endl;
                    break;
                case Array:
                    os << "Array" << std::endl;
                    break;
                case UnknownFormat:
                    os << "UnknownFormat" << std::endl;
                    break;
            }

            os << "Field: ";
            switch (header.Field) {
                case Real:
                    os << "Real" << std::endl;
                    break;
                case Double:
                    os << "Double" << std::endl;
                    break;
                case Complex:
                    os << "Complex" << std::endl;
                    break;
                case Integer:
                    os << "Integer" << std::endl;
                    break;
                case Pattern:
                    os << "Pattern" << std::endl;
                    break;
                case UnknownField:
                    os << "UnknownField" << std::endl;
                    break;
            }

            os << "Symmetry: ";
            switch (header.Symmetry) {
                case General:
                    os << "General" << std::endl;
                    break;
                case Symmetric:
                    os << "Symmetric" << std::endl;
                    break;
                case SkewSymmetric:
                    os << "SkewSymmetric" << std::endl;
                    break;
                case Hermitian:
                    os << "Hermitian" << std::endl;
                    break;
                case UnknownSymmetry:
                    os << "UnknownSymmetry" << std::endl;
                    break;
            }
            //os << header << std::endl;

            return os;
        }
        friend std::ostream& operator << (std::ostream& os, const Comment& comment)
        {
            //return os << header.Identifier  << header.Object << header.Field <<   header.Format << header.Symmetry << std::endl;

            os << comment.Text << std::endl;

            return os;
        }
        friend std::ostream& operator << (std::ostream& os, const MatrixSize& matrixSize)
        {
            os << "Matrix Size: \n   Rows: " << matrixSize.Rows << std::endl
                    << "   Columns: " << matrixSize.Columns << std::endl
                    << "   Num. of non-zero entries: " << matrixSize.NumberOfEntries << std::endl;

            return os;
        }

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

        MatrixSize readMatrixSize(std::ifstream &f) {
            MatrixSize matrixSize;

            f >> matrixSize.Rows >> matrixSize.Columns >> matrixSize.NumberOfEntries;

            return matrixSize;
        }

        template<typename T>
        void readMatrixData(std::ifstream &f, MatrixSize matrixSize) {
            T matrixCellData;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++)
            {
                int row, column;
                f >> row >> column >> matrixCellData;

                std::cout << "R: " << row << " - C: " << column << " - D: " << matrixCellData << std::endl;
            }
        }

        void readMatrixData_Pattern(std::ifstream &f, MatrixSize matrixSize) {
            // For FieldType = Pattern, no value fields exist. All the coordinate values are assumed to be 1.
            auto matrixCellData = 1;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++)
            {
                int row, column;
                f >> row >> column;

                std::cout << "R: " << row << " - C: " << column << " - D: " << matrixCellData << std::endl;
            }
        }

        void readMatrixData(std::ifstream &f, MatrixSize matrixSize, FieldType fieldType) {

            switch (fieldType) {
                case Real:
                    readMatrixData <float> (f, matrixSize);
                    break;
                case Double:
                    readMatrixData <double> (f, matrixSize);
                    break;
                case Complex:
                    break;
                case Integer:
                    readMatrixData <int> (f, matrixSize);
                    break;
                case Pattern:
                    readMatrixData_Pattern(f, matrixSize);
                    break;
                case UnknownField:
                    break;
            }
        }

        void readMatrix(std::string file_name) {
            std::string aLine;
            std::ifstream f(file_name);

            // Read the header
            Header matrixHeader = readHeader(f);
            Comment matrixComment = readComment(f);
            MatrixSize matrixSize = readMatrixSize(f);

            std::cout << matrixHeader << std::endl;
            std::cout << matrixComment << std::endl;
            std::cout << matrixSize << std::endl;

            readMatrixData(f, matrixSize, matrixHeader.Field);
        }

    public:
        MatrixMarketData(std::string FileName) {
            fileName = FileName;
            readMatrix(FileName);
        }

        ~MatrixMarketData(){
            // Destructor of the class
            std::cout << "MatrixMarketData destructor ...\n";
        }
    };

} // Matrix

#endif //MATRIXMARKETDATA_MATRIXMARKETDATA_H
