//
// Created by Hasan Aytekin on 22.07.2023.
//

#ifndef MATRIXMARKETDATA_MATRIXCSR_H
#define MATRIXMARKETDATA_MATRIXCSR_H
//#include <vector>
//#include <iostream>
//#include<algorithm>
#include "GlobalDeclerations.h"
#include "BitNumber.h"

namespace Matrix {

    // Compares two Data structures by row and column values.
    template<typename T>
    bool compareCoordinate(const Data<T>& d1, const Data<T>& d2)
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

    class MatrixCSR {
    private:
        MatrixCompression_CSR_Collection matrixCompression_CSR_Collection;

        template<typename T>
        MatrixCompression_CSR<T> compressMatrixToCSR(MatrixData<Data<T>> matrixData) {
            MatrixCompression_CSR<T> matrix_csr;
            std::int64_t current_row = 0;
            std::int64_t num_of_values_for_current_row = 0;

            // Initial row index and number of values for the current row are both 0
            matrix_csr.RowSeperators.push_back(num_of_values_for_current_row);

            for(Data<T> data: matrixData.Coordinate) {
                // Check if we are dealing with a new row

                if ((data.Row-1) != current_row) { // Convert row index to base 0
                    // Store the current num_of_values_for_current_row into the RowSeperators vector
                    matrix_csr.RowSeperators.push_back(num_of_values_for_current_row);
                    // Set the new row to <this> row, DO NOT set the num_of_values_for_current_row to 0
                    current_row = data.Row - 1;
                    // num_of_values_for_current_row = 0;
                }
                matrix_csr.Values.push_back(data.Value);
//                if (data.Column == 0) {
//                    int yy = 0;
//                }
//                matrix_csr.ColumnIndexes.push_back(data.Column - 1); // Convert column index into base 0
                matrix_csr.ColumnIndexes.push_back(data.Column);
                // OK. We inserted a value.
                // Increment the count of values by 1;
                num_of_values_for_current_row++;
            }
            // Insert the last row value count into the RowSeperators vector, if exists !
            if (num_of_values_for_current_row !=0) {
                matrix_csr.RowSeperators.push_back(num_of_values_for_current_row);
            }

            return matrix_csr;
        }

        int getDataTypeLengthForIndexing(unsigned long aValue) {
            if (aValue <= 65535) { // 2 bytes
                return sizeof(unsigned short);
            } else if (aValue <= 4294967295) { // 4 bytes
                return sizeof(unsigned int);
            } else if (aValue <= (unsigned long)18446744073709551615) { // 8 bytes
                return sizeof(unsigned long);
            }
        }

        MatrixCompression_CSR_Collection processMatrixData(MatrixCollection& coordinateMatrixCollection) {
////            MatrixCompression_CSR<T> matrix_csr;
//            MatrixCompression_CSR_DataCollection matrix_csr_collection;
//            MatrixCompression_CSR_Size matrix_csr_size;
            MatrixCompression_CSR_Collection matrix_csr_collection;

            int sizeOfIndexField = getDataTypeLengthForIndexing(coordinateMatrixCollection.matrix_size.NumberOfEntries);
            int sizeOfValueField = getDataTypeLengthForIndexing((long)coordinateMatrixCollection.matrixDataCollection.type_data_range.Maximum);
            long SCSRSizeInBytes;

            switch (coordinateMatrixCollection.matrix_header.Field) {
                case Real:
//                    BitNumber rowIndexSizeInfo = new BitNumber<int>(matrix_csr_collection.CSR_DataCollection.type_float.ColumnIndexes.size());
//                    BitNumber columnIndexSizeInfo = new BitNumber<int>(matrix_csr_collection.CSR_DataCollection.type_float.RowSeperators.size());

                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_float.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_float.Coordinate.end(),
                              compareCoordinate<float>);
                    matrix_csr_collection.CSR_DataCollection.type_float = compressMatrixToCSR<float>(coordinateMatrixCollection.matrixDataCollection.type_float);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(float) * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();
//                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_float.ColumnIndexes.size();
//                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_float.RowSeperators.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_float.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_float.RowSeperators.size();
                    SCSRSizeInBytes = sizeOfValueField * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();
                    break;
                case Double:
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_double.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_double.Coordinate.end(),
                              compareCoordinate<double>);
                    matrix_csr_collection.CSR_DataCollection.type_double = compressMatrixToCSR<double>(coordinateMatrixCollection.matrixDataCollection.type_double);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(double) * matrix_csr_collection.CSR_DataCollection.type_double.Values.size();
//                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_double.ColumnIndexes.size();
//                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_double.RowSeperators.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_double.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_double.RowSeperators.size();
                    SCSRSizeInBytes = sizeOfValueField * matrix_csr_collection.CSR_DataCollection.type_double.Values.size();
                    break;
                case Complex:
                    SCSRSizeInBytes = 0;;
                    break;
                case Integer:
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.end(),
                              compareCoordinate<int>);
                    matrix_csr_collection.CSR_DataCollection.type_int = compressMatrixToCSR<int>(coordinateMatrixCollection.matrixDataCollection.type_int);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
//                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_int.ColumnIndexes.size();
//                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_int.RowSeperators.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_int.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_int.RowSeperators.size();
                    SCSRSizeInBytes = sizeOfValueField * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
                    break;
                case Pattern:
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_pattern.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_pattern.Coordinate.end(),
                              compareCoordinate<short>);
                    matrix_csr_collection.CSR_DataCollection.type_pattern = compressMatrixToCSR<short>(coordinateMatrixCollection.matrixDataCollection.type_pattern);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_pattern.Values.size();
//                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_pattern.ColumnIndexes.size();
//                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_pattern.RowSeperators.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_pattern.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexField * matrix_csr_collection.CSR_DataCollection.type_pattern.RowSeperators.size();
                    SCSRSizeInBytes = sizeOfValueField * matrix_csr_collection.CSR_DataCollection.type_pattern.Values.size();
                    break;
                case UnknownField:
                    SCSRSizeInBytes = 0;;
                    break;
            }

            matrix_csr_collection.CSR_Structure_Size.CSRSizeInBytes = matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes + matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes + matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes;
            matrix_csr_collection.CSR_Structure_Size.SCSRSizeInBytes = SCSRSizeInBytes + matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes + matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes;

            return matrix_csr_collection;
        }

        MatrixCompression_CSR_Collection processMatrixData(Segment& segmentedCoordinateMatrixCollection) {

            MatrixCompression_CSR_Collection matrix_csr_collection;

            auto valueField = new BitNumber<long long>((long long) segmentedCoordinateMatrixCollection.type_data_range.Maximum);
            int sizeOfValueField = valueField->numberAnalysisResult.NumberOfBytes;
//            int sizeOfValueField = getDataTypeLengthForIndexing((long) segmentedCoordinateMatrixCollection.type_data_range.Maximum);
//            sizeOfValueField = valueField->numberAnalysisResult.NumberOfBytes;

            long SCSRSizeInBytes;

            switch (segmentedCoordinateMatrixCollection.segmentDataType) {
                case Real:
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_float.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_float.Coordinate.end(),
                              compareCoordinate<float>);
                    matrix_csr_collection.CSR_DataCollection.type_float = compressMatrixToCSR<float>(segmentedCoordinateMatrixCollection.segmentCollection.type_float);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(float) * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_float.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_float.RowSeperators.size();
                    SCSRSizeInBytes = std::min(sizeOfValueField, (int)sizeof(float)) * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();
                    break;
                case Double:
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_double.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_double.Coordinate.end(),
                              compareCoordinate<double>);
                    matrix_csr_collection.CSR_DataCollection.type_double = compressMatrixToCSR<double>(segmentedCoordinateMatrixCollection.segmentCollection.type_double);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(double) * matrix_csr_collection.CSR_DataCollection.type_double.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_double.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_double.RowSeperators.size();
                    SCSRSizeInBytes = std::min(sizeOfValueField, (int)sizeof(double)) * matrix_csr_collection.CSR_DataCollection.type_double.Values.size();
                    break;
                case Complex:
                    SCSRSizeInBytes = 0;
                    break;
                case Integer:
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_int.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_int.Coordinate.end(),
                              compareCoordinate<int>);
                    matrix_csr_collection.CSR_DataCollection.type_int = compressMatrixToCSR<int>(segmentedCoordinateMatrixCollection.segmentCollection.type_int);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_int.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_int.RowSeperators.size();
                    SCSRSizeInBytes = std::min(sizeOfValueField, (int)sizeof(int)) * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
                    break;
                case Pattern:
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_pattern.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_pattern.Coordinate.end(),
                              compareCoordinate<short>);
                    matrix_csr_collection.CSR_DataCollection.type_pattern = compressMatrixToCSR<short>(segmentedCoordinateMatrixCollection.segmentCollection.type_pattern);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_pattern.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_pattern.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_pattern.RowSeperators.size();
                    SCSRSizeInBytes = std::min(sizeOfValueField, (int)sizeof(short)) * matrix_csr_collection.CSR_DataCollection.type_pattern.Values.size();
                    break;
                case UnknownField:
                    SCSRSizeInBytes = 0;
                    break;
            }

            matrix_csr_collection.CSR_Structure_Size.CSRSizeInBytes = matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes + matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes + matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes;
            matrix_csr_collection.CSR_Structure_Size.SCSRSizeInBytes = SCSRSizeInBytes +  matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes + matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes;

            return matrix_csr_collection;
        }
    public:
/*
        MatrixCSR(MatrixData<Data<T>> coordinateMatrix) {
            Data<int> x = {0, 1, 2};
            coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.push_back(x);
            processMatrixData(coordinateMatrix);
        }
*/

        MatrixCSR(MatrixCollection& coordinateMatrixCollection) {
//            Data<int> x = {0, 1, 2};
//            coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.push_back(x);
            matrixCompression_CSR_Collection = processMatrixData(coordinateMatrixCollection);
        }

        MatrixCSR(Segment& coordinateMatrixCollection) {
            matrixCompression_CSR_Collection = processMatrixData(coordinateMatrixCollection);
        }

        MatrixCompression_CSR_Collection GetMatrixCSRCollection() {
            return this->matrixCompression_CSR_Collection;
        }

        unsigned long GetMatrixSizeInBytes() {
            return this->matrixCompression_CSR_Collection.CSR_Structure_Size.CSRSizeInBytes;
        }

        unsigned long GetSegmentedMatrixSizeWithBitNumberInBytes() {
            return this->matrixCompression_CSR_Collection.CSR_Structure_Size.SCSRSizeInBytes;
        }

        virtual ~MatrixCSR() {
            // Destructor of the class
//            std::cout << "MatrixCSR destructor ...\n";
        }
    };
}

#endif //MATRIXMARKETDATA_MATRIXCSR_H
