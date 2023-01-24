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

        void printHeader(std::string identifier, std::string object, std::string format, std::string field, std::string symmetry) {
            std::cout << identifier << std::endl;
            std::cout << object << std::endl;
            std::cout << format << std::endl;
            std::cout << field << std::endl;
            std::cout << symmetry << std::endl;
        }

        void printHeader(Header matrixHeader) {

            std::cout << "Identifier: " << matrixHeader.Identifier << std::endl;

            std::cout << "Object: ";
            switch (matrixHeader.Object) {
                case Matrix:
                    std::cout << "Matrix" << std::endl;
                    break;
                case Vector:
                    std::cout << "Vector" << std::endl;
                    break;
                case UnknownObject:
                    std::cout << "UnknownObject" << std::endl;
                    break;
            }

            std::cout << "Format: ";
            switch (matrixHeader.Format) {
                case Coordinate:
                    std::cout << "Coordinate" << std::endl;
                    break;
                case Array:
                    std::cout << "Array" << std::endl;
                    break;
                case UnknownFormat:
                    std::cout << "UnknownFormat" << std::endl;
                    break;
            }

            std::cout << "Field: ";
            switch (matrixHeader.Field) {
                case Real:
                    std::cout << "Real" << std::endl;
                    break;
                case Double:
                    std::cout << "Double" << std::endl;
                    break;
                case Complex:
                    std::cout << "Complex" << std::endl;
                    break;
                case Integer:
                    std::cout << "Integer" << std::endl;
                    break;
                case Pattern:
                    std::cout << "Pattern" << std::endl;
                    break;
                case UnknownField:
                    std::cout << "UnknownField" << std::endl;
                    break;
            }

            std::cout << "Symmetry: ";
            switch (matrixHeader.Symmetry) {
                case General:
                    std::cout << "General" << std::endl;
                    break;
                case Symmetric:
                    std::cout << "Symmetric" << std::endl;
                    break;
                case SkewSymmetric:
                    std::cout << "SkewSymmetric" << std::endl;
                    break;
                case Hermitian:
                    std::cout << "Hermitian" << std::endl;
                    break;
                case UnknownSymmetry:
                    std::cout << "UnknownSymmetry" << std::endl;
                    break;
            }
            //std::cout << matrixHeader << std::endl;
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
                printHeader(identifier, object, format, field, symmetry);
                //printHeader(matrixHeader);
            }

            return matrixHeader;
        }


        void readMatrixData(std::string file_name) {
            std::string aLine;
            std::ifstream f(file_name);

            // Read the header
            Header matrixHeader;
            matrixHeader = readHeader(f);
            std::cout << matrixHeader << std::endl;

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
