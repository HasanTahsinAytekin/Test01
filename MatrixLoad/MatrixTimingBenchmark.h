//
// Created by Hasan Aytekin on 24.12.2023.
//

#ifndef MATRIXLOAD_MATRIXTIMINGBENCHMARK_H
#define MATRIXLOAD_MATRIXTIMINGBENCHMARK_H

#include <chrono>
#include <iostream>
#include <cmath>
#include "GlobalDeclerations.h"

namespace Matrix {

    class MatrixTimingBenchmark {
    public:
        class MatrixCSR {
        private:
            MatrixCollection coordinateMatrixCollection;   // Raw data
            MatrixCompression_CSR_Collection matrixCompression_CSR_Collection;  // CSR

//            template <typename IndexType, typename ValueType>
//            ValueType retrieveValueFromCSR(long long row, long long col, MatrixCompression_CSR<ValueType>& csrCollectionData) {
            template <typename IndexType, typename ValueType>
            ValueType retrieveValueFromCSR(IndexType row, IndexType col, MatrixCompression_CSR<ValueType>& csrCollectionData) {
                // row and col values are 1 based

                // first element index in Values
                auto firstElementIndex = csrCollectionData.RowSeperators[row - 1]; // Value is 0 based
                // Number of values in the corresponding row
//                auto x0 = csrCollectionData.RowSeperators[row - 1];
//                auto x1 = csrCollectionData.RowSeperators[row];
                auto numValues = csrCollectionData.RowSeperators[row] - csrCollectionData.RowSeperators[row -1];
                // Retrieve the value by checking the column index

                ValueType value = 0;

                for (auto i = 0; i < numValues; i++) {
                    if ((csrCollectionData.ColumnIndexes[firstElementIndex + i]) == col) {
                        value = csrCollectionData.Values[firstElementIndex + i];
                        break;
                    }
                }

                return value;
            }

            template <typename IndexType, typename ValueType>
            long long sparseMatrixItemsRetrievalTiming(MatrixData<Data<ValueType>>& matrixData, MatrixCompression_CSR<ValueType>& matrixCompression_CSR){
                Timer *processTimer = new Timer();
                processTimer->Start();

                auto start = std::chrono::high_resolution_clock::now();

                // Perform the task ....
                for(Data<ValueType> data: matrixData.Coordinate) {
                    auto x = retrieveValueFromCSR<IndexType, ValueType>(data.Row, data.Column, matrixCompression_CSR);
                    // for debugging
                    if (x != data.Value) {
                        std::cout << " Ivalid data: Row=" << data.Row << " Col=" << data.Column << "Value=" << data.Value << " (!=" << x << ")" << std::endl;
//                        x = retrieveValueFromCSR<IndexType, ValueType>(data.Row, data.Column, matrixCompression_CSR);
                    }
                }

                processTimer->Stop();
//                std::cout << "Görevin işlem süresi: " << processTimer->Duration() << " mikrosaniye" << std::endl;
//                auto stop = std::chrono::high_resolution_clock::now();
//                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//                std::cout << "Görevin işlem süresi: " << duration.count() << " mikrosaniye" << std::endl;

                long long duration = processTimer->Duration();
                delete processTimer;

                return duration;
            }

        public:
            long long int CSR_Benchmark_Timing;

            MatrixCSR(MatrixCollection& coordinateCollection, MatrixCompression_CSR_Collection& CSR_Collection) {
                if (Constants::DisplayProgress) std::cout << " -> CSR Benchmark...";

                matrixCompression_CSR_Collection = CSR_Collection;
                coordinateMatrixCollection = coordinateCollection;

//                int sizeOfIndexFieldRow = Utilities::GetDataTypeLengthForIndexing(coordinateMatrixCollection.matrix_size.RowIndexSizeInBytes);
//                int sizeOfIndexFieldColumn = Utilities::GetDataTypeLengthForIndexing(coordinateMatrixCollection.matrix_size.ColumnIndexSizeInBytes);
//                int sizeOfIndexField = std::max(sizeOfIndexFieldRow, sizeOfIndexFieldColumn);
                int sizeOfIndexField = std::max(coordinateMatrixCollection.matrix_size.RowIndexSizeInBytes, coordinateMatrixCollection.matrix_size.ColumnIndexSizeInBytes);
//                long long benchmarkTiming;

                switch (coordinateMatrixCollection.matrix_header.Field) {
                    case Real:
                        if (sizeOfIndexField <= 2) { // 2 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned short, float>(coordinateMatrixCollection.matrixDataCollection.type_float, matrixCompression_CSR_Collection.CSR_DataCollection.type_float);
                        } else if (sizeOfIndexField <= 4) { // 4 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned int, float>(coordinateMatrixCollection.matrixDataCollection.type_float, matrixCompression_CSR_Collection.CSR_DataCollection.type_float);
                        } else if (sizeOfIndexField <= 8) { // 8 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned long, float>(coordinateMatrixCollection.matrixDataCollection.type_float, matrixCompression_CSR_Collection.CSR_DataCollection.type_float);
                        }
                        break;
                    case Double:
                        if (sizeOfIndexField <= 2) { // 2 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned short, double>(coordinateMatrixCollection.matrixDataCollection.type_double, matrixCompression_CSR_Collection.CSR_DataCollection.type_double);
                        } else if (sizeOfIndexField <= 4) { // 4 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned int, double>(coordinateMatrixCollection.matrixDataCollection.type_double, matrixCompression_CSR_Collection.CSR_DataCollection.type_double);
                        } else if (sizeOfIndexField <= 8) { // 8 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned long, double>(coordinateMatrixCollection.matrixDataCollection.type_double, matrixCompression_CSR_Collection.CSR_DataCollection.type_double);
                        }
                        break;
                    case Complex:
                        // NIY. For testing purposes ....
                        CSR_Benchmark_Timing = 0;
                        break;
                    case Integer:
                        if (sizeOfIndexField <= 2) { // 2 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned short, int>(coordinateMatrixCollection.matrixDataCollection.type_int, matrixCompression_CSR_Collection.CSR_DataCollection.type_int);
                        } else if (sizeOfIndexField <= 4) { // 4 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned int, int>(coordinateMatrixCollection.matrixDataCollection.type_int, matrixCompression_CSR_Collection.CSR_DataCollection.type_int);
                        } else if (sizeOfIndexField <= 8) { // 8 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned long, int>(coordinateMatrixCollection.matrixDataCollection.type_int, matrixCompression_CSR_Collection.CSR_DataCollection.type_int);
                        }
                        break;
                    case Pattern:
                        if (sizeOfIndexField <= 2) { // 2 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned short, short>(coordinateMatrixCollection.matrixDataCollection.type_pattern, matrixCompression_CSR_Collection.CSR_DataCollection.type_pattern);
                        } else if (sizeOfIndexField <= 4) { // 4 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned int, short>(coordinateMatrixCollection.matrixDataCollection.type_pattern, matrixCompression_CSR_Collection.CSR_DataCollection.type_pattern);
                        } else if (sizeOfIndexField <= 8) { // 8 bytes
                            CSR_Benchmark_Timing = sparseMatrixItemsRetrievalTiming<unsigned long, short>(coordinateMatrixCollection.matrixDataCollection.type_pattern, matrixCompression_CSR_Collection.CSR_DataCollection.type_pattern);
                        }
                        break;
                    case UnknownField:
                        CSR_Benchmark_Timing = 0;
                        break;
                }

//                CSR_Collection.CSR_Benchmark_Timing = xx;

                if (Constants::DisplayProgress) std::cout << "Done";
            }

        };

        class SegmentedMatrixCSR {
        private:
            MatrixCollection coordinateMatrixCollection;  // Raw data
            std::vector<Segment> segmentedCSRCollection;  // SCSR

            int segment_number(int num) {
                return num / Constants::DefaultSegmentSize;
            }

            void x(long long row, long long col) {
                unsigned int numberOfRowSegments;
                unsigned int numberOfColSegments;
                unsigned int totalNumberOfEntries = 0;

                numberOfRowSegments = ceil(row / Constants::DefaultSegmentSize);
                numberOfColSegments = ceil(col / Constants::DefaultSegmentSize);
            }

            template <typename ValueType>
            ValueType retrieveValueFromSCSR(unsigned short row, unsigned short col, MatrixCompression_CSR<ValueType>& csrCollectionData) {
                // row and col values are 1 based

                // first element index in Values
//                auto firstElementIndex = csrCollectionData.RowSeperators[row - 1]; // Value is 0 based
                auto firstElementIndex = csrCollectionData.RowSeperators[row]; // Value is 0 based
                // Number of values in the corresponding row
//                auto numValues = csrCollectionData.RowSeperators[row] - csrCollectionData.RowSeperators[row -1];
                auto numValues = csrCollectionData.RowSeperators[row+1] - csrCollectionData.RowSeperators[row];
                // Retrieve the value by checking the column index

                ValueType value = 0;

                for (auto i = 0; i < numValues; i++) {
                    if ((csrCollectionData.ColumnIndexes[firstElementIndex + i]) == (col+1)) { // col values in CulumnIndexes are 1 based !
                        value = csrCollectionData.Values[firstElementIndex + i];
                        break;
                    }
                }

                return value;
            }

            template <typename ValueType>
            auto segmentedSparseMatrixItemsRetrievalTiming(MatrixData<Data<ValueType>>& matrixData, MatrixCompression_SCSR_Map& scsrMap, std::vector<Segment>& segmentedCSR){
                Timer *processTimer = new Timer();
                processTimer->Start();
//                auto start = std::chrono::high_resolution_clock::now();

                // Perform the task ....
                // For each original coordinate matrix value, retrieve the 'corresponding value' from the 'corresponding segment' stored in the segmented CSR vector
                for(Data<ValueType> data: matrixData.Coordinate) {
                    // Row and column values are 1 based !!!
//                    auto rowSegment = ceil((data.Row - 1) / Constants::DefaultSegmentSizeTest);
//                    auto colSegment = ceil((data.Column - 1) / Constants::DefaultSegmentSizeTest);
                    // Calculate row and column segments for 0 based representation ....
                    auto rowSegment = ceil((data.Row - 1) / Constants::DefaultSegmentSizeTest);
                    auto colSegment = ceil((data.Column - 1) / Constants::DefaultSegmentSizeTest);
                    // ... and proceed
                    unsigned short rowInSegment = (data.Row - 1) % Constants::DefaultSegmentSizeTest;
                    unsigned short colInSegment = (data.Column - 1) % Constants::DefaultSegmentSizeTest;

                    ValueType aValue;
                    auto anIndex = rowSegment * scsrMap.NumberOfColSegments + colSegment;

//                    Segment& correspondingSegment = segmentedCSR[(rowSegment-1) * scsrMap.NumberOfColSegments + colSegment]; // -1 is for 0 based vector array
//                    auto x1 = correspondingSegment.segmented_CSR_Collection.CSR_DataCollection;

                    switch (scsrMap.SCSRFieldType) {
                        case Real:
                            aValue = retrieveValueFromSCSR<float>(rowInSegment, colInSegment, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_float);
                            break;
                        case Double:
                            aValue = retrieveValueFromSCSR<double>(rowInSegment, colInSegment, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_double);
                            break;
                        case Complex:
                            // NIY !!!
                            break;
                        case Integer:
                            aValue = retrieveValueFromSCSR<int>(rowInSegment, colInSegment, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_int);
                            break;
                        case Pattern:
                            aValue = retrieveValueFromSCSR<short>(rowInSegment, colInSegment, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_pattern);
                            //aValue = retrieveValueFromSCSR<short>(data.Row, data.Column, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_pattern);
                            break;
                        case UnknownField:
                            // NIY !!!
                            break;
                    }

                    // for debugging
                    if (aValue != data.Value) {
                        std::cout << " Invalid data: Row=" << data.Row << " [" << rowInSegment << "] Col=" << data.Column << "[" << colInSegment << "] Value=" << data.Value << " (!=" << aValue << ")" << std::endl;
//                        aValue = retrieveValueFromSCSR<short>(rowInSegment, colInSegment, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_pattern);
                        aValue = retrieveValueFromSCSR<int>(rowInSegment, colInSegment, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_int);
//                        aValue = retrieveValueFromSCSR<short>(data.Row, data.Column, segmentedCSR[anIndex].segmented_CSR_Collection.CSR_DataCollection.type_pattern);
                    }
                }

                processTimer->Stop();
//                std::cout << "Görevin işlem süresi: " << processTimer->Duration() << " mikrosaniye" << std::endl;
//                auto stop = std::chrono::high_resolution_clock::now();
//                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//                std::cout << "Görevin işlem süresi: " << duration.count() << " mikrosaniye" << std::endl;
                long long duration = processTimer->Duration();
                delete processTimer;

                return duration;
            }

        public:
            long long int SCSR_Benchmark_Timing;

            SegmentedMatrixCSR(MatrixCollection& coordinateCollection, MatrixCompression_SCSR_Map& scsrMap,std::vector<Segment>& vectorSegmentedCSR){
                if (Constants::DisplayProgress) std::cout << " -> SCSR Benchmark...";

                segmentedCSRCollection = vectorSegmentedCSR;
                coordinateMatrixCollection = coordinateCollection;


                // Fixed size :)
                int sizeOfIndexField = 2; // unsigned short integer (2 bytes)

                switch (coordinateMatrixCollection.matrix_header.Field) {
                    case Real:
                        SCSR_Benchmark_Timing = segmentedSparseMatrixItemsRetrievalTiming<float>(coordinateMatrixCollection.matrixDataCollection.type_float, scsrMap, vectorSegmentedCSR);
                        break;
                    case Double:
                        SCSR_Benchmark_Timing = segmentedSparseMatrixItemsRetrievalTiming<double>(coordinateMatrixCollection.matrixDataCollection.type_double, scsrMap, vectorSegmentedCSR);
                        break;
                    case Complex:
                        // NIY. For testing purposes ....
                        SCSR_Benchmark_Timing = 0;
                        break;
                    case Integer:
                        SCSR_Benchmark_Timing = segmentedSparseMatrixItemsRetrievalTiming<int>(coordinateMatrixCollection.matrixDataCollection.type_int, scsrMap, vectorSegmentedCSR);
                        break;
                    case Pattern:
                        SCSR_Benchmark_Timing = segmentedSparseMatrixItemsRetrievalTiming<short>(coordinateMatrixCollection.matrixDataCollection.type_pattern, scsrMap, vectorSegmentedCSR);
                        break;
                    case UnknownField:
                        SCSR_Benchmark_Timing = 0;
                        break;
                }
                if (Constants::DisplayProgress) std::cout << "Done";
            }
        };

    private:
        class Timer {
        private:
            bool timerActive;
            std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
            std::chrono::time_point<std::chrono::high_resolution_clock> stopTime;
        public:
            Timer() {
                // Nothing for the time being
                timerActive = false;
            }

            void Start() {
                timerActive = true;
                startTime = std::chrono::high_resolution_clock::now();
            }

            void Stop() {
                stopTime = std::chrono::high_resolution_clock::now();
                timerActive = false;
            }

            long long int Duration() { // in microseconds

                if (timerActive) {
                    // Timer not stopped. Nothing to return.
                    return 0;
                }
                else {
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
                    return duration.count();
                }
            }

            long long int CurrentProcessingTime() { // in microseconds
                auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime);
                return elapsedTime.count();
            }
        };

    };

} // Matrix

#endif //MATRIXLOAD_MATRIXTIMINGBENCHMARK_H
