//
// Created by Hasan Aytekin on 8.08.2023.
//

#ifndef MATRIXLOAD_MATRIXCSC_H
#define MATRIXLOAD_MATRIXCSC_H

#include <algorithm>
#include "GlobalDeclerations.h"
#include "BitNumber.h"

namespace Matrix {

/*
    // Compares two Data structures by row and column values.
    template<typename T>
    bool compareCoordinateColumns(const Data<T>& d1, const Data<T>& d2)
    {
        if (d1.Column < d2.Column) {
            // Column1 < Column2
            return true;
        }
        else if (d1.Column > d2.Column) {
            // Column1 > Column2
            return false;
        }
        else if (d1.Row < d2.Row) {
            // Column1 = Column2 and Row1 < Row2
            return true;
        }
        else {
            // Column1 = Column2 and Row1 >= Row2
            return false;
        }
    }
*/

    class MatrixCSC {

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
        MatrixCompression_CSC_Collection matrixCompression_CSC_Collection;

        template<typename T>
        MatrixCompression_CSC<T> compressMatrixToCSC(MatrixData<Data<T>>& matrixData) {
            MatrixCompression_CSC<T> matrix_csc;
            long long int current_col = 0;
            long long int num_of_values_for_current_col = 0;

            // Initial row index and number of values for the current row are both 0
            matrix_csc.ColumnSeparators.push_back(num_of_values_for_current_col);

            for(Data<T> data: matrixData.Coordinate) {
                // Check if we are dealing with a new column
                if ((data.Column-1) != current_col) { // Convert col index to base 0
                    // Store the current num_of_values_for_current_row into the ColSeparators vector
                    matrix_csc.ColumnSeparators.push_back(num_of_values_for_current_col);
                    // Set the new row to <this> row, DO NOT set the num_of_values_for_current_row to 0
                    current_col = data.Column - 1;
                }
                matrix_csc.Values.push_back(data.Value);

                matrix_csc.RowIndexes.push_back(data.Column);
                // OK. We inserted a value.
                // Increment the count of values by 1;
                num_of_values_for_current_col++;
            }
            // Insert the last row value count into the RowSeperators vector, if exists !
            if (num_of_values_for_current_col !=0) {
                matrix_csc.ColumnSeparators.push_back(num_of_values_for_current_col);
            }

            return matrix_csc;
        }

        MatrixCompression_CSC_Collection processMatrixData(MatrixCollection& coordinateMatrixCollection) {
            MatrixCompression_CSC_Collection matrix_csc_collection;

            int sizeOfIndexFieldRow = Utilities::GetDataTypeLengthForIndexing(coordinateMatrixCollection.matrix_size.Rows);
            int sizeOfIndexFieldColSeparator = Utilities::GetDataTypeLengthForIndexing(coordinateMatrixCollection.matrix_size.NumberOfEntries);
            int sizeOfValueField = Utilities::GetDataTypeLengthForIndexing((long)coordinateMatrixCollection.matrixDataCollection.type_data_range.Maximum);
            long SCSCSizeInBytes;

            switch (coordinateMatrixCollection.matrix_header.Field) {
                case Real:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_float.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_float.Coordinate.end(),
                              compareCoordinateColumns<float>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_float = compressMatrixToCSC<float>(coordinateMatrixCollection.matrixDataCollection.type_float);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(float) * matrix_csc_collection.CSC_DataCollection.type_float.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeOfIndexFieldRow * matrix_csc_collection.CSC_DataCollection.type_float.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeOfIndexFieldColSeparator * matrix_csc_collection.CSC_DataCollection.type_float.ColumnSeparators.size();
                    SCSCSizeInBytes = sizeOfValueField * matrix_csc_collection.CSC_DataCollection.type_float.Values.size();
                    break;
                case Double:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_double.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_double.Coordinate.end(),
                              compareCoordinateColumns<double>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_double = compressMatrixToCSC<double>(coordinateMatrixCollection.matrixDataCollection.type_double);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(double) * matrix_csc_collection.CSC_DataCollection.type_double.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeOfIndexFieldRow * matrix_csc_collection.CSC_DataCollection.type_double.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeOfIndexFieldColSeparator * matrix_csc_collection.CSC_DataCollection.type_double.ColumnSeparators.size();
                    SCSCSizeInBytes = sizeOfValueField * matrix_csc_collection.CSC_DataCollection.type_double.Values.size();
                    break;
                case Complex:
                    SCSCSizeInBytes = 0;;
                    break;
                case Integer:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_int.Coordinate.end(),
                              compareCoordinateColumns<int>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_int = compressMatrixToCSC<int>(coordinateMatrixCollection.matrixDataCollection.type_int);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(int) * matrix_csc_collection.CSC_DataCollection.type_int.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeOfIndexFieldRow * matrix_csc_collection.CSC_DataCollection.type_int.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeOfIndexFieldColSeparator * matrix_csc_collection.CSC_DataCollection.type_int.ColumnSeparators.size();
                    SCSCSizeInBytes = sizeOfValueField * matrix_csc_collection.CSC_DataCollection.type_int.Values.size();
                    break;
                case Pattern:
/*
                    std::sort(coordinateMatrixCollection.matrixDataCollection.type_pattern.Coordinate.begin(),
                              coordinateMatrixCollection.matrixDataCollection.type_pattern.Coordinate.end(),
                              compareCoordinateColumns<short>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_pattern = compressMatrixToCSC<short>(coordinateMatrixCollection.matrixDataCollection.type_pattern);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_pattern.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeOfIndexFieldRow * matrix_csc_collection.CSC_DataCollection.type_pattern.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeOfIndexFieldColSeparator * matrix_csc_collection.CSC_DataCollection.type_pattern.ColumnSeparators.size();
                    SCSCSizeInBytes = sizeOfValueField * matrix_csc_collection.CSC_DataCollection.type_pattern.Values.size();
                    break;
                case UnknownField:
                    SCSCSizeInBytes = 0;;
                    break;
            }

            matrix_csc_collection.CSC_Structure_Size.CSCSizeInBytes = matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes + matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes + matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes;
            matrix_csc_collection.CSC_Structure_Size.SCSCSizeInBytes = SCSCSizeInBytes + matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes + matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes;

            return matrix_csc_collection;
        }

        MatrixCompression_CSC_Collection processMatrixData(Segment& segmentedCoordinateMatrixCollection) {

            MatrixCompression_CSC_Collection matrix_csc_collection;

            auto valueField = new BitNumber<long long>((long long) segmentedCoordinateMatrixCollection.type_data_range.Maximum);
            int sizeOfValueField = valueField->numberAnalysisResult.NumberOfBytes;
            delete valueField;

            long SCSCSizeInBytes;

            switch (segmentedCoordinateMatrixCollection.segmentDataType) {
                case Real:
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_float.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_float.Coordinate.end(),
                              compareCoordinateColumns<float>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_float = compressMatrixToCSC<float>(segmentedCoordinateMatrixCollection.segmentCollection.type_float);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(float) * matrix_csc_collection.CSC_DataCollection.type_float.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_float.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_float.ColumnSeparators.size();
                    SCSCSizeInBytes = std::min(sizeOfValueField, (int)sizeof(float)) * matrix_csc_collection.CSC_DataCollection.type_float.Values.size();
                    break;
                case Double:
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_double.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_double.Coordinate.end(),
                              compareCoordinateColumns<double>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_double = compressMatrixToCSC<double>(segmentedCoordinateMatrixCollection.segmentCollection.type_double);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(double) * matrix_csc_collection.CSC_DataCollection.type_double.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_double.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_double.ColumnSeparators.size();
                    SCSCSizeInBytes = std::min(sizeOfValueField, (int)sizeof(double)) * matrix_csc_collection.CSC_DataCollection.type_double.Values.size();
                    break;
                case Complex:
                    SCSCSizeInBytes = 0;
                    break;
                case Integer:
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_int.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_int.Coordinate.end(),
                              compareCoordinateColumns<int>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_int = compressMatrixToCSC<int>(segmentedCoordinateMatrixCollection.segmentCollection.type_int);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(int) * matrix_csc_collection.CSC_DataCollection.type_int.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_int.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_int.ColumnSeparators.size();
                    SCSCSizeInBytes = std::min(sizeOfValueField, (int)sizeof(int)) * matrix_csc_collection.CSC_DataCollection.type_int.Values.size();
                    break;
                case Pattern:
/*
                    std::sort(segmentedCoordinateMatrixCollection.segmentCollection.type_pattern.Coordinate.begin(),
                              segmentedCoordinateMatrixCollection.segmentCollection.type_pattern.Coordinate.end(),
                              compareCoordinateColumns<short>);
*/
                    matrix_csc_collection.CSC_DataCollection.type_pattern = compressMatrixToCSC<short>(segmentedCoordinateMatrixCollection.segmentCollection.type_pattern);
                    matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_pattern.Values.size();
                    matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_pattern.RowIndexes.size();
                    matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes = sizeof(short) * matrix_csc_collection.CSC_DataCollection.type_pattern.ColumnSeparators.size();
                    SCSCSizeInBytes = std::min(sizeOfValueField, (int)sizeof(short)) * matrix_csc_collection.CSC_DataCollection.type_pattern.Values.size();
                    break;
                case UnknownField:
                    SCSCSizeInBytes = 0;
                    break;
            }

            matrix_csc_collection.CSC_Structure_Size.CSCSizeInBytes = matrix_csc_collection.CSC_Structure_Size.ValuesArraySizeInBytes + matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes + matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes;
            matrix_csc_collection.CSC_Structure_Size.SCSCSizeInBytes = SCSCSizeInBytes +  matrix_csc_collection.CSC_Structure_Size.RowIndexesArraySizeInBytes + matrix_csc_collection.CSC_Structure_Size.ColumnSeparatorsArraySizeInBytes;

            return matrix_csc_collection;
        }

    public:
        MatrixCSC(MatrixCollection& coordinateMatrixCollection) {
            if (Constants::DisplayProgress) std::cout << " -> Compressing ColumnWise...";

            matrixCompression_CSC_Collection = processMatrixData(coordinateMatrixCollection);

            if (Constants::DisplayProgress) std::cout << "Done";
        }

        MatrixCSC(Segment& coordinateMatrixCollection) {
            matrixCompression_CSC_Collection = processMatrixData(coordinateMatrixCollection);
        }

        MatrixCompression_CSC_Collection GetMatrixCSCCollection() {
            return this->matrixCompression_CSC_Collection;
        }

        unsigned long GetMatrixSizeInBytes() {
            return this->matrixCompression_CSC_Collection.CSC_Structure_Size.CSCSizeInBytes;
        }

        unsigned long GetSegmentedMatrixSizeWithBitNumberInBytes() {
            return this->matrixCompression_CSC_Collection.CSC_Structure_Size.SCSCSizeInBytes;
        }

        virtual ~MatrixCSC() {
            // Destructor of the class
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_int.Values.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_int.ColumnSeparators.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_int.RowIndexes.clear();
            //
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_float.Values.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_float.ColumnSeparators.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_float.RowIndexes.clear();
            //
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_double.Values.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_double.ColumnSeparators.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_double.RowIndexes.clear();
            //
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_pattern.Values.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_pattern.ColumnSeparators.clear();
            this->matrixCompression_CSC_Collection.CSC_DataCollection.type_pattern.RowIndexes.clear();

//            std::cout << "MatrixCSR destructor ...\n";
        }
    };

} // Matrix

#endif //MATRIXLOAD_MATRIXCSC_H
