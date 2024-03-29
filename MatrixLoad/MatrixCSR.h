//
// Created by Hasan Aytekin on 22.07.2023.
//

#ifndef MATRIXMARKETDATA_MATRIXCSR_H
#define MATRIXMARKETDATA_MATRIXCSR_H

#include <algorithm>
#include <chrono>
#include "GlobalDeclerations.h"
#include "BitNumber.h"

namespace Matrix {

/*
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
*/

    class MatrixCSR {

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

    private:
        MatrixCompression_CSR_Collection matrixCompression_CSR_Collection;

        template<typename T>
        MatrixCompression_CSR<T> compressMatrixToCSR(MatrixData<Data<T>>& matrixData) {
            MatrixCompression_CSR<T> matrix_csr;
            long long int current_row = 0;
            long long int num_of_values_for_current_row = 0;

            // Initial row index and number of values for the current row are both 0
            matrix_csr.RowSeperators.push_back(num_of_values_for_current_row);

            for(Data<T> data: matrixData.Coordinate) {
                // Check if we are dealing with a new row
                if ((data.Row-1) != current_row) { // Convert row index to base 0
                    // Store the current num_of_values_for_current_row into the RowSeperators vector
                    matrix_csr.RowSeperators.push_back(num_of_values_for_current_row);
                    // Check if the next row is the row in sequence?
                    if ((data.Row-1) != (current_row+1)) {
                        // We have virtual row() with no column data at all !!!
                        // Insert dummy row separators for each dummy row.
//                        if ((data.Row == 1) && (data.Column == 65535)) {
//                            int q;
//                            q=0;
//                        }
                        for (int dummyRows = 0; dummyRows < ((data.Row-1) - (current_row+1)); ++dummyRows) {
                            // Store the current num_of_values_for_current_row into the RowSeperators
                            matrix_csr.RowSeperators.push_back(num_of_values_for_current_row);
                        }
                    }
                    // Set the new row to <this> row, DO NOT set the num_of_values_for_current_row to 0
                    current_row = data.Row - 1;
                }
                matrix_csr.Values.push_back(data.Value);

                matrix_csr.ColumnIndexes.push_back(data.Column);
                // OK. We inserted a value.
                // Increment the count of values by 1;
                num_of_values_for_current_row++;
//
//                if ((data.Row == 1) && (data.Column == 65535)) {
//                    int q;
//                    q=0;
//                }
            }
            // Insert the last row value count into the RowSeperators vector, if exists !
            if (num_of_values_for_current_row !=0) {
                matrix_csr.RowSeperators.push_back(num_of_values_for_current_row);
            }

            return matrix_csr;
        }

        template <typename IndexType, typename ValueType>
        ValueType retrieveValueFromCSR(IndexType row, IndexType col, MatrixCompression_CSR<ValueType>& csrCollection) {
            // row and col values are 1 based

            // first element index in Values
            auto firstElementIndex = csrCollection.RowSeperators[row - 1]; // Value is 0 based
            // Number of values in the corresponding row
            auto numValues = csrCollection.RowSeperators[row - 1] - csrCollection.RowSeperators[row];
            // Retrieve the value by checking the column index

            ValueType value = 0;

            for (auto i = 0; i < numValues; i++) {
                if ((csrCollection.ColumnIndexes[firstElementIndex + i] + 1) == col) {
                    value = csrCollection.Values[firstElementIndex + i];
                    break;
                }
            }

            return value;
        }

        template <typename IndexType, typename ValueType>
        auto sparseMatrixItemsRetrievalTiming(MatrixData<Data<ValueType>>& matrixData, int rowIndexSizeInBytes, int colIndexSizeInBytes){
            auto start = std::chrono::high_resolution_clock::now();

            // Perform the task ....
            for(Data<ValueType> data: matrixData.Coordinate) {
                //auto x = retrieveValueFromCSR<IndexType, ValueType>(data.Row, data.Column, ???);
            }

            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "Görevin işlem süresi: " << duration.count() << " mikrosaniye" << std::endl;

            return 0;
        }

        MatrixCompression_CSR_Collection processMatrixData(MatrixCollection& coordinateMatrixCollection) {
            MatrixCompression_CSR_Collection matrix_csr_collection;

            int sizeOfIndexFieldColumn = Utilities::GetDataTypeLengthForIndexing(coordinateMatrixCollection.matrix_size.Columns);
            int sizeOfIndexFieldRowSeparator = Utilities::GetDataTypeLengthForIndexing(coordinateMatrixCollection.matrix_size.NumberOfEntries);
            int sizeOfValueField = Utilities::GetDataTypeLengthForIndexing((long)coordinateMatrixCollection.matrixDataCollection.type_data_range.Maximum);
            long SCSRSizeInBytes;

            switch (coordinateMatrixCollection.matrix_header.Field) {
                case Real:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_float.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_float.Coordinate.end(),
                              compareCoordinateRows<float>);
*/
                    matrix_csr_collection.CSR_DataCollection.type_float = compressMatrixToCSR<float>(coordinateMatrixCollection.matrixDataCollection.type_float);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(float) * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexFieldColumn * matrix_csr_collection.CSR_DataCollection.type_float.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexFieldRowSeparator * matrix_csr_collection.CSR_DataCollection.type_float.RowSeperators.size();
                    SCSRSizeInBytes = sizeOfValueField * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();

//                    auto x = sparceMatrixItemsRetrievalTiming<float, float>(coordinateMatrixCollection.matrixDataCollection.type_float, coordinateMatrixCollection.matrix_size.RowIndexSizeInBytes, coordinateMatrixCollection.matrix_size.ColumnIndexSizeInBytes)

                    break;
                case Double:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_double.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_double.Coordinate.end(),
                              compareCoordinateRows<double>);
*/
                    matrix_csr_collection.CSR_DataCollection.type_double = compressMatrixToCSR<double>(coordinateMatrixCollection.matrixDataCollection.type_double);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(double) * matrix_csr_collection.CSR_DataCollection.type_double.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexFieldColumn * matrix_csr_collection.CSR_DataCollection.type_double.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexFieldRowSeparator * matrix_csr_collection.CSR_DataCollection.type_double.RowSeperators.size();
                    SCSRSizeInBytes = sizeOfValueField * matrix_csr_collection.CSR_DataCollection.type_double.Values.size();
                    break;
                case Complex:
                    SCSRSizeInBytes = 0;;
                    break;
                case Integer:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.end(),
                              compareCoordinateRows<int>);
*/
                    matrix_csr_collection.CSR_DataCollection.type_int = compressMatrixToCSR<int>(coordinateMatrixCollection.matrixDataCollection.type_int);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexFieldColumn * matrix_csr_collection.CSR_DataCollection.type_int.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexFieldRowSeparator * matrix_csr_collection.CSR_DataCollection.type_int.RowSeperators.size();
                    SCSRSizeInBytes = sizeOfValueField * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
                    break;
                case Pattern:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_pattern.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_pattern.Coordinate.end(),
                              compareCoordinateRows<short>);
*/
                    matrix_csr_collection.CSR_DataCollection.type_pattern = compressMatrixToCSR<short>(coordinateMatrixCollection.matrixDataCollection.type_pattern);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_pattern.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeOfIndexFieldColumn * matrix_csr_collection.CSR_DataCollection.type_pattern.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeOfIndexFieldRowSeparator * matrix_csr_collection.CSR_DataCollection.type_pattern.RowSeperators.size();
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
            delete valueField;

            long SCSRSizeInBytes;

            switch (segmentedCoordinateMatrixCollection.segmentDataType) {
                case Real:
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_float.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_float.Coordinate.end(),
                              compareCoordinateRows<float>);
*/
                    matrix_csr_collection.CSR_DataCollection.type_float = compressMatrixToCSR<float>(segmentedCoordinateMatrixCollection.segmentCollection.type_float);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(float) * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_float.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_float.RowSeperators.size();
                    SCSRSizeInBytes = std::min(sizeOfValueField, (int)sizeof(float)) * matrix_csr_collection.CSR_DataCollection.type_float.Values.size();
                    break;
                case Double:
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_double.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_double.Coordinate.end(),
                              compareCoordinateRows<double>);
*/
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
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_int.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_int.Coordinate.end(),
                              compareCoordinateRows<int>);
*/
                    matrix_csr_collection.CSR_DataCollection.type_int = compressMatrixToCSR<int>(segmentedCoordinateMatrixCollection.segmentCollection.type_int);
                    matrix_csr_collection.CSR_Structure_Size.ValuesArraySizeInBytes = sizeof(int) * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
                    matrix_csr_collection.CSR_Structure_Size.ColumnIndexesArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_int.ColumnIndexes.size();
                    matrix_csr_collection.CSR_Structure_Size.RowSeperatorsArraySizeInBytes = sizeof(short) * matrix_csr_collection.CSR_DataCollection.type_int.RowSeperators.size();
                    SCSRSizeInBytes = std::min(sizeOfValueField, (int)sizeof(int)) * matrix_csr_collection.CSR_DataCollection.type_int.Values.size();
                    break;
                case Pattern:
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_pattern.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_pattern.Coordinate.end(),
                              compareCoordinateRows<short>);
*/
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
        MatrixCSR(MatrixCollection& coordinateMatrixCollection) {
            if (Constants::DisplayProgress) std::cout << " -> Compressing RowWise...";

            matrixCompression_CSR_Collection = processMatrixData(coordinateMatrixCollection);

            // Get the benchmark retrieval timing of all items in the matrix

            if (Constants::DisplayProgress) std::cout << "Done";
        }

        MatrixCSR(Segment& coordinateMatrixCollectionOfASegment) {
            matrixCompression_CSR_Collection = processMatrixData(coordinateMatrixCollectionOfASegment);
        }

        MatrixCompression_CSR_Collection GetMatrixCSRCollection() {
            return this->matrixCompression_CSR_Collection;
        }

        MatrixCompression_CSR_Collection& GetMatrixCSRCollectionAddress() {
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
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_int.Values.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_int.RowSeperators.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_int.ColumnIndexes.clear();
            //
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_float.Values.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_float.RowSeperators.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_float.ColumnIndexes.clear();
            //
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_double.Values.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_double.RowSeperators.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_double.ColumnIndexes.clear();
            //
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_pattern.Values.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_pattern.RowSeperators.clear();
            this->matrixCompression_CSR_Collection.CSR_DataCollection.type_pattern.ColumnIndexes.clear();

//            std::cout << "MatrixCSR destructor ...\n";
        }
    };
}

#endif //MATRIXMARKETDATA_MATRIXCSR_H
