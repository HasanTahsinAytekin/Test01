//
// Created by Hasan Aytekin on 9.08.2023.
//

#ifndef MATRIXLOAD_SEGMENTEDMATRIXCSC_H
#define MATRIXLOAD_SEGMENTEDMATRIXCSC_H

#include "MatrixCSC.h"

namespace Matrix {

    class SegmentedMatrixCSC {
        std::vector<Segment> segments;

    private:
        MatrixCompression_SCSC_Collection scscCollection;
        unsigned long totalCSCSizeOfSegmentsInBytes;
        unsigned long totalCSCSizeOfSegmentsWithBitNumberInBytes;

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

        void constructSegmentedMatrices(MatrixCollection& matrixCollection) {
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
        }

    public:
        SegmentedMatrixCSC(MatrixCollection& coordinateMatrixCollection) {

            if (Constants::DisplayProgress) std::cout << " -> Compressing Segmented ColumnWise...";

            constructSegmentedMatrices(coordinateMatrixCollection);

            // Obtain the CSC representation of each segment
            unsigned long int totalCSCSizeOfSegmentsInBytes = 0;
            unsigned long int totalCSCSizeOfSegmentsWithBitNumberInBytes = 0;

            for (Segment segment: segments) {
                auto matrixCSC = new MatrixCSC(segment);
                totalCSCSizeOfSegmentsInBytes += matrixCSC->GetMatrixSizeInBytes();
                totalCSCSizeOfSegmentsWithBitNumberInBytes += matrixCSC->GetSegmentedMatrixSizeWithBitNumberInBytes();
                delete matrixCSC;
            }

            this->totalCSCSizeOfSegmentsInBytes = totalCSCSizeOfSegmentsInBytes;
            this->totalCSCSizeOfSegmentsWithBitNumberInBytes = totalCSCSizeOfSegmentsWithBitNumberInBytes;

            if (Constants::DisplayProgress) std::cout << "Done" << '\r';
        }

        unsigned long GetMatrixSizeInBytes() {
            return this->totalCSCSizeOfSegmentsInBytes;
        }

        unsigned long GetMatrixSizeWithBitNumberInBytes() {
            return this->totalCSCSizeOfSegmentsWithBitNumberInBytes;
        }

        ~SegmentedMatrixCSC() {
            // Destructor of the class
            this->segments.clear();
            //
            this->scscCollection.SCSC_DataCollection.type_pattern.Values.clear();
            this->scscCollection.SCSC_DataCollection.type_pattern.RowIndexes.clear();
            this->scscCollection.SCSC_DataCollection.type_pattern.ColumnSeperators.clear();
            //
            this->scscCollection.SCSC_DataCollection.type_double.Values.clear();
            this->scscCollection.SCSC_DataCollection.type_double.RowIndexes.clear();
            this->scscCollection.SCSC_DataCollection.type_double.ColumnSeperators.clear();
            //
            this->scscCollection.SCSC_DataCollection.type_float.Values.clear();
            this->scscCollection.SCSC_DataCollection.type_float.RowIndexes.clear();
            this->scscCollection.SCSC_DataCollection.type_float.ColumnSeperators.clear();
            //
            this->scscCollection.SCSC_DataCollection.type_int.Values.clear();
            this->scscCollection.SCSC_DataCollection.type_int.RowIndexes.clear();
            this->scscCollection.SCSC_DataCollection.type_int.ColumnSeperators.clear();

//            std::cout << "SegmentedMatrixCSR destructor ...\n";
        }
    };

} // Matrix

#endif //MATRIXLOAD_SEGMENTEDMATRIXCSC_H
