//
// Created by Hasan Aytekin on 21.01.2023.
//

#include <iostream>
#include <fstream>

#ifndef MATRIX_MATRIXMARKETDATA_H
#define MATRIX_MATRIXMARKETDATA_H

namespace MatrixMarketData {

    enum ObjectType {
        Matrix,
        Vector
    };
    enum FormatType {
        Coordinate,
        Array
    };
    enum FieldType {
        Real,
        Double,
        Complex,
        Integer,
        Pattern
    };
    enum SymmetryType {
        General,
        Symmetric,
        SkewSymmetric,
        Hermitian
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
        struct Size {
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

        void parseHeader(std::string header_string) {
            std::string delimiter = " ";
            std::size_t pos = 0;
            std::string token;
            while ((pos = header_string.find(delimiter)) != std::string::npos) {
                token = header_string.substr(0, pos);
                std::cout << token << std::endl;
                header_string.erase(0, pos + delimiter.length());
            }
            std::cout << header_string << std::endl;
        }

        void readHeader(std::string identifier, std::string object, std::string format, std::string field, std::string symmetry) {
            std::cout << identifier << std::endl;
            std::cout << object << std::endl;
            std::cout << format << std::endl;
            std::cout << field << std::endl;
            std::cout << symmetry << std::endl;
        }

        void readMatrixData(std::string file_name) {
            std::string aLine;
            std::ifstream f(file_name);

            // Read the header
            Header matrixHeader;
            //readHeader(f);
            std::string identifier;
            std::string object;
            std::string format;
            std::string field;
            std::string symmetry;
            if (f >> identifier >> object >> format >> field >> symmetry) {
                readHeader(identifier, object, format, field, symmetry);
            }

        }
    public:
        MatrixMarketData(std::string FileName) {
            fileName = FileName;
            readMatrixData(FileName);
        }

        ~MatrixMarketData(){
            // Destructor of the class
            std::cout << "MatrixMarketData destructor ...\n";
        }
    };

} // MatrixMarketData

#endif //MATRIX_MATRIXMARKETDATA_H
