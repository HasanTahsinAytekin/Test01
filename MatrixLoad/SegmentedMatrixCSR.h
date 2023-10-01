//
// Created by Hasan Aytekin on 23.07.2023.
//

#ifndef MATRIXMARKETDATA_SEGMENTEDMATRIXCSR_H
#define MATRIXMARKETDATA_SEGMENTEDMATRIXCSR_H

#include "MatrixCSR.h"

namespace Matrix {

    class SegmentedMatrixCSR {
        std::vector<Segment> segments;

    private:
        MatrixCompression_SCSR_Collection scsrCollection;
        unsigned long totalCSRSizeOfSegmentsInBytes;
        unsigned long totalCSRSizeOfSegmentsWithBitNumberInBytes;

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

//        std::vector<Segment> constructSegmentedMatrices(MatrixCollection& matrixCollection) {
        void constructSegmentedMatrices(MatrixCollection& matrixCollection) {
//            std::vector<Segment> segments;
            unsigned int numberOfRowSegments;
            unsigned int numberOfColSegments;
            unsigned int totalNumberOfEntries = 0;

            numberOfRowSegments = ceil(matrixCollection.matrix_size.Rows / Constants::DefaultSegmentSize);
            numberOfColSegments = ceil(matrixCollection.matrix_size.Columns / Constants::DefaultSegmentSize);

            for(unsigned int segmentRow = 0; segmentRow < numberOfRowSegments; segmentRow++) {
                int numberOfRowsToProcess;
                SegmentRange segmentRowRange;

                segmentRowRange.Start = segmentRow * Constants::DefaultSegmentSize + 1; // 1 Based

                numberOfRowsToProcess = std::min((int)matrixCollection.matrix_size.Rows - segmentRowRange.Start, (int) Constants::DefaultSegmentSize - 1); // -1 is the correction of 1 based representation
                segmentRowRange.End = segmentRowRange.Start + numberOfRowsToProcess;

                for(unsigned int segmentCol = 0; segmentCol < numberOfColSegments; segmentCol++) {
                    int numberOfColsToProcess;
                    SegmentRange segmentColRange;
                    MatrixDataCollection dataCollection;

//                    if (Constants::DisplayProgress) std::cout << " -> segment(" << segmentRow << ", " << segmentCol << ")";

                    segmentColRange.Start = segmentCol * Constants::DefaultSegmentSize + 1; // 1 Based

                    numberOfColsToProcess = std::min((int)matrixCollection.matrix_size.Columns - segmentColRange.Start, (int) Constants::DefaultSegmentSize - 1);
                    segmentColRange.End = segmentColRange.Start + numberOfColsToProcess;

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
//            return segments;
        }

    public:
        SegmentedMatrixCSR(MatrixCollection& coordinateMatrixCollection) {

            if (Constants::DisplayProgress) std::cout << " -> Compressing Segmented RowWise...";

            constructSegmentedMatrices(coordinateMatrixCollection);

            // Obtain the CSR representation of each segment
            unsigned long int totalCSRSizeOfSegmentsInBytes = 0;
            unsigned long int totalCSRSizeOfSegmentsWithBitNumberInBytes = 0;

            for (Segment segment: segments) {
                auto matrixCSR = new MatrixCSR(segment);
                totalCSRSizeOfSegmentsInBytes += matrixCSR->GetMatrixSizeInBytes();
                totalCSRSizeOfSegmentsWithBitNumberInBytes += matrixCSR->GetSegmentedMatrixSizeWithBitNumberInBytes();
                delete matrixCSR;
//                free(matrixCSR);
            }

            this->totalCSRSizeOfSegmentsInBytes = totalCSRSizeOfSegmentsInBytes;
            this->totalCSRSizeOfSegmentsWithBitNumberInBytes = totalCSRSizeOfSegmentsWithBitNumberInBytes;

            if (Constants::DisplayProgress) std::cout << "Done";
        }

        unsigned long GetMatrixSizeInBytes() {
            return this->totalCSRSizeOfSegmentsInBytes;
        }

        unsigned long GetMatrixSizeWithBitNumberInBytes() {
            return this->totalCSRSizeOfSegmentsWithBitNumberInBytes;
        }

        ~SegmentedMatrixCSR() {
            // Destructor of the class
            this->segments.clear();
            //
            this->scsrCollection.SCSR_DataCollection.type_pattern.Values.clear();
            this->scsrCollection.SCSR_DataCollection.type_pattern.ColumnIndexes.clear();
            this->scsrCollection.SCSR_DataCollection.type_pattern.RowSeperators.clear();
            //
            this->scsrCollection.SCSR_DataCollection.type_double.Values.clear();
            this->scsrCollection.SCSR_DataCollection.type_double.ColumnIndexes.clear();
            this->scsrCollection.SCSR_DataCollection.type_double.RowSeperators.clear();
            //
            this->scsrCollection.SCSR_DataCollection.type_float.Values.clear();
            this->scsrCollection.SCSR_DataCollection.type_float.ColumnIndexes.clear();
            this->scsrCollection.SCSR_DataCollection.type_float.RowSeperators.clear();
            //
            this->scsrCollection.SCSR_DataCollection.type_int.Values.clear();
            this->scsrCollection.SCSR_DataCollection.type_int.ColumnIndexes.clear();
            this->scsrCollection.SCSR_DataCollection.type_int.RowSeperators.clear();

//            std::cout << "SegmentedMatrixCSR destructor ...\n";
        }

    };

} // Matrix

#endif //MATRIXMARKETDATA_SEGMENTEDMATRIXCSR_H
