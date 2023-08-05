//
// Created by Hasan Aytekin on 15.07.2023.
//

#include "MatrixMarketData.h"

namespace Matrix {
    std::ostream &operator<<(std::ostream &os, const MatrixDataCollection &matrixDataCollection) {
        double min, max;

        os << "Matrix Data Range:" << std::endl
           << "   Minimum: " << matrixDataCollection.type_data_range.Minimum << std::endl
           << "   Maximum: " << matrixDataCollection.type_data_range.Maximum << "\n" << std::endl;

        if (!matrixDataCollection.type_pattern.Coordinate.empty()) {
            for (Data<short> data: matrixDataCollection.type_pattern.Coordinate) {
                os << "R: " << data.Row << " - C: " << data.Column << " - D: " << data.Value << std::endl;
            }
        }
        else {
            if (!matrixDataCollection.type_int.Coordinate.empty()) {
                for (Data<int> data: matrixDataCollection.type_int.Coordinate) {
                    os << "R: " << data.Row << " - C: " << data.Column << " - D: " << data.Value << std::endl;
                }
            }
            else {
                if (!matrixDataCollection.type_float.Coordinate.empty()) {
                    for (Data<float> data: matrixDataCollection.type_float.Coordinate) {
                        os << "R: " << data.Row << " - C: " << data.Column << " - D: " << data.Value << std::endl;
                    }
                }
                else {
                    if (!matrixDataCollection.type_double.Coordinate.empty()) {
                        for (Data<double> data: matrixDataCollection.type_double.Coordinate) {
                            os << "R: " << data.Row << " - C: " << data.Column << " - D: " << data.Value << std::endl;
                        }
                    }
                }
            }
        }

        return os;
    }

    std::ostream &operator<<(std::ostream &os, const MatrixSize &matrixSize) {
        os << "Matrix Size: \n"
           << "   Rows: " << matrixSize.Rows << std::endl
           << "   Columns: " << matrixSize.Columns << std::endl
           << "   Num. of non-zero entries: " << matrixSize.NumberOfEntries << std::endl
           << "   Size (Bytes): " << matrixSize.SizeInBytes << std::endl;

        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Comment &comment) {
        //return os << header.Identifier  << header.Object << header.Field <<   header.Format << header.Symmetry << std::endl;

        os << comment.Text << std::endl;

        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Header &header) {
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

    std::ostream &operator<<(std::ostream &os, const DataRange &dataRange) {
        double min, max;

        os << "Matrix Data Range:" << std::endl
           << "   Minimum: " << dataRange.Minimum << std::endl
           << "   Maximum: " << dataRange.Maximum << "\n" << std::endl;

        return os;
    }
} // Matrix