//
// Created by Hasan Aytekin on 23.07.2023.
//

#ifndef MATRIXMARKETDATA_SEGMENTEDMATRIXCSR_H
#define MATRIXMARKETDATA_SEGMENTEDMATRIXCSR_H

#include <cmath>
#include <iostream>
//#include "GlobalDeclerations.h"
#include "MatrixCSR.h"

namespace Matrix {

/*
    template<typename T>
    struct MatrixCompression_SCSR {
        std::vector<T> Values;
        std::vector<short> ColumnIndexes;
        std::vector<short> RowSeperators;
    };

    struct MatrixCompression_SCSR_DataCollection{
        MatrixCompression_SCSR<short> type_pattern;
        MatrixCompression_SCSR<float> type_float;
        MatrixCompression_SCSR<double> type_double;
        MatrixCompression_SCSR<int> type_int;
    };

    struct MatrixCompression_SCSR_Size {
        std::int64_t ValuesArraySizeInBytes;
        std::int64_t ColumnIndexesArraySizeInBytes;
        std::int64_t RowSeperatorsArraySizeInBytes;
        std::int64_t SCSRSizeInBytes;
    };

    struct MatrixCompression_SCSR_Collection{
        MatrixCompression_SCSR_DataCollection SCSR_DataCollection;
        MatrixCompression_SCSR_Size CSSR_Structure_Size;
    };
*/

//****************************************************************************************************************

//****************************************************************************************************************

    template<typename T>
    struct SegmentedData {
        short Row;
        short Column;
        T Value;
    };

/*
    struct Segment {
        FieldType segmentDataType;
        MatrixSize segmentSize;
        SegmentRange segmentRangeRow;
        SegmentRange segmentRangeColumn;
        MatrixDataCollection segmentCollection;
    };
*/

    struct MatrixSegments {
        std::vector <Segment> segments;
        int totalSegmentSize;
    };

    class SegmentedMatrixCSR {
    private:
        double defaultSegmentSize = 65535; // double -> ceil function requires double
//        double defaultSegmentSize = 100; // double -> ceil function requires double
        MatrixCompression_SCSR_Collection scsrCollection;
        unsigned long totalCSRSizeOfSegmentsInBytes;
        unsigned long totalCSRSizeOfSegmentsWithBitNumberInBytes;
/*
        DataTypesUnsigned getDataTypeForIndexing(long aValue) {
            if (aValue <= 65535) { // 2 bytes
                return UnsignedShortInt;
            } else if (aValue <= 4294967295) { // 4 bytes
                return UnsignedInt;
            } else if (aValue <= 18446744073709551615) { // 8 bytes
                return UnsignedLong;
            }
        }
*/
/*
        template<typename T>
        MatrixData<Data<T>> readMatrixData(std::ifstream &f, MatrixSize matrixSize) {
//            T matrixCellData;
            MatrixData<Data<T>> matrixData;

            // Read the data
            for (int line = 0; line < matrixSize.NumberOfEntries; line++) {
                Data<T> newCellData;
                f >> newCellData.Row >> newCellData.Column >> newCellData.Value;
                matrixData.Coordinate.push_back(newCellData);

//                std::cout << "R: " << newCellData.Row << " - C: " << newCellData.Column << " - D: " << newCellData.Value << std::endl;
            }

            return matrixData;
        }
*/

        template <typename T>
        MatrixData<Data<T>> constructSegmentMatrix(MatrixData<Data<T>>& matrixDataC, SegmentRange rowRange, SegmentRange colRange) {
            MatrixData<Data<T>> matrixData;

            // Construct the Segment
            for (Data<T> cellData: matrixDataC.Coordinate) {
                if ((cellData.Row >= rowRange.Start) && (cellData.Row <= rowRange.End)) {
                    if ((cellData.Column >= colRange.Start) && (cellData.Column <= colRange.End)) {
                        Data<T> segmentedData;
                        segmentedData.Row = (short) (cellData.Row - rowRange.Start) + 1; // 1 based
                        segmentedData.Column = (short) (cellData.Column - colRange.Start) + 1; // 1 based
                        segmentedData.Value = cellData.Value;
                        matrixData.Coordinate.push_back(segmentedData);
                    }
                }
            }

            return matrixData;
        }

        std::vector<Segment> constructSegmentedMatrices(MatrixCollection& matrixCollection) {
//            MatrixData<Data<T>> matrixData;
//            std::vector<MatrixData<Data<T>>> segmentedMatrices;
            std::vector<Segment> segments;
//            MatrixDataCollection dataCollection;
            unsigned int numberOfRowSegments;
            unsigned int numberOfColSegments;
            unsigned int totalNumberOfEntries = 0;

            numberOfRowSegments = ceil(matrixCollection.matrix_size.Rows / this->defaultSegmentSize);
            numberOfColSegments = ceil(matrixCollection.matrix_size.Columns / this->defaultSegmentSize);

//            std::cout << "NumberOfRowSegments: " << numberOfRowSegments << std::endl;
//            std::cout << "NumberOfColSegments: " << numberOfColSegments << std::endl;

            for(unsigned int segmentRow = 0; segmentRow < numberOfRowSegments; segmentRow++) {
                int numberOfRowsToProcess;
                SegmentRange segmentRowRange;

                segmentRowRange.Start = segmentRow * this->defaultSegmentSize + 1; // 1 Based

                numberOfRowsToProcess = std::min((int)matrixCollection.matrix_size.Rows - segmentRowRange.Start, (int) this->defaultSegmentSize - 1); // -1 is the correction of 1 based representation
                segmentRowRange.End = segmentRowRange.Start + numberOfRowsToProcess;

                for(unsigned int segmentCol = 0; segmentCol < numberOfColSegments; segmentCol++) {
                    int numberOfColsToProcess;
                    SegmentRange segmentColRange;
                    MatrixDataCollection dataCollection;

//                    segmentColRange.Start = segmentCol * this->defaultSegmentSize + 1; // 1 Based
                    segmentColRange.Start = segmentCol * this->defaultSegmentSize + 1; // 1 Based

                    numberOfColsToProcess = std::min((int)matrixCollection.matrix_size.Columns - segmentColRange.Start, (int) this->defaultSegmentSize - 1);
                    segmentColRange.End = segmentColRange.Start + numberOfColsToProcess;

//                    std::cout << "Segment: ---> R = " << segmentRow << " (" << segmentRowRange.Start << ", " << segmentRowRange.End << ")"
//                              << " --- C = " << segmentCol << " (" << segmentColRange.Start << ", " << segmentColRange.End << ")" << std::endl;

                    MatrixSize sSize;
                    switch (matrixCollection.matrix_header.Field) {
                        case Real:
                            dataCollection.type_float = constructSegmentMatrix<float>(matrixCollection.matrixDataCollection.type_float, segmentRowRange, segmentColRange);
                            sSize.NumberOfEntries = dataCollection.type_float.Coordinate.size();
                            break;
                        case Double:
                            dataCollection.type_double = constructSegmentMatrix<double>(matrixCollection.matrixDataCollection.type_double, segmentRowRange, segmentColRange);
                            sSize.NumberOfEntries = dataCollection.type_double.Coordinate.size();
                            break;
                        case Complex:
                            break;
                        case Integer:
                            dataCollection.type_int = constructSegmentMatrix<int>(matrixCollection.matrixDataCollection.type_int, segmentRowRange, segmentColRange);
                            sSize.NumberOfEntries = dataCollection.type_int.Coordinate.size();
                            break;
                        case Pattern:
                            dataCollection.type_pattern = constructSegmentMatrix<short>(matrixCollection.matrixDataCollection.type_pattern, segmentRowRange, segmentColRange);
                            sSize.NumberOfEntries = dataCollection.type_pattern.Coordinate.size();
                            break;
                        case UnknownField:
                            break;
                    }

                    // Construct the Segment structure
                    Segment aSegment;
                    aSegment.segmentDataType = matrixCollection.matrix_header.Field;
                    aSegment.segmentRangeRow = segmentRowRange;
                    aSegment.segmentRangeColumn = segmentColRange;
                    aSegment.segmentCollection = dataCollection;
                    aSegment.type_data_range = matrixCollection.matrixDataCollection.type_data_range;

                    sSize.Rows = numberOfRowsToProcess + 1; // 1 based
                    sSize.Columns = numberOfColsToProcess + 1; // 1 based
                    sSize.SizeInBytes = 0;
                    aSegment.segmentSize = sSize;
                    totalNumberOfEntries += sSize.NumberOfEntries;

                    // Add this segment into the Segments collection
                    segments.push_back(aSegment);
                }
            }
            return segments;
        }

    public:
        SegmentedMatrixCSR(MatrixCollection& coordinateMatrixCollection) {

            std::vector<Segment> segments = constructSegmentedMatrices(coordinateMatrixCollection);

            // Obtain the CSR representation of each segment
            unsigned long int totalCSRSizeOfSegmentsInBytes = 0;
            unsigned long int totalCSRSizeOfSegmentsWithBitNumberInBytes = 0;
            for (Segment segment: segments) {
                auto matrixCSR = new MatrixCSR(segment);
                totalCSRSizeOfSegmentsInBytes += matrixCSR->GetMatrixSizeInBytes();
                totalCSRSizeOfSegmentsWithBitNumberInBytes += matrixCSR->GetSegmentedMatrixSizeWithBitNumberInBytes();
            }

            this->totalCSRSizeOfSegmentsInBytes = totalCSRSizeOfSegmentsInBytes;
            this->totalCSRSizeOfSegmentsWithBitNumberInBytes = totalCSRSizeOfSegmentsWithBitNumberInBytes;
        }

        unsigned long GetMatrixSizeInBytes() {
            return this->totalCSRSizeOfSegmentsInBytes;
        }

        unsigned long GetMatrixSizeWithBitNumberInBytes() {
            return this->totalCSRSizeOfSegmentsWithBitNumberInBytes;
        }

        ~SegmentedMatrixCSR() {
            // Destructor of the class
//            std::cout << "SegmentedMatrixCSR destructor ...\n";
        }

    };

} // Matrix

#endif //MATRIXMARKETDATA_SEGMENTEDMATRIXCSR_H
