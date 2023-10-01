//
// Created by Hasan Aytekin on 21.07.2023.
//

#ifndef MATRIXMARKETDATA_GLOBALDECLERATIONS_H
#define MATRIXMARKETDATA_GLOBALDECLERATIONS_H

//#include <iostream>
#include <vector>
#include <string>
//#include <cstdint>
//#include <algorithm>

namespace Matrix {
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
    enum DataTypesUnsigned {
        UnsignedShortInt,
        UnsignedInt,
        UnsignedLong
    };
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

    struct MatrixSize {
//        std::int64_t Rows;
//        std::int64_t Columns;
//        std::int64_t NumberOfEntries;
//        std::int64_t SizeInBytes;   // Array Size
        long long int Rows;
        long long int Columns;
        long long int NumberOfEntries;
        long long int SizeInBytes;   // Array Size
        short RowIndexSizeInBytes;
        short ColumnIndexSizeInBytes;
        short ValueSizeInBytes;
    };

    template<typename T>
    struct Data {
//        std::int64_t Row;
//        std::int64_t Column;
        long long int Row;
        long long int Column;
        T Value;
    };

    template<class C>
    struct MatrixData {
        std::vector<C> Coordinate;
    };

    struct DataRange {
        double Minimum;
        double Maximum;
    };

    struct SegmentRange {
        int Start;
        int End;
    };

    struct MatrixDataCollection {
        MatrixData<Data<short>> type_pattern;
        MatrixData<Data<float>> type_float;
        MatrixData<Data<double>> type_double;
        MatrixData<Data<int>> type_int;
        DataRange type_data_range;
    };

    struct MatrixCollection {
        MatrixDataCollection matrixDataCollection;
        Header matrix_header;
        Comment matrix_comment;
        MatrixSize matrix_size;
    };

    // For Compressed Row
    template<typename T>
    struct MatrixCompression_CSR {
        std::vector<T> Values;
//        std::vector<int64_t> ColumnIndexes;
//        std::vector<int64_t> RowSeperators;
        std::vector<long long int> ColumnIndexes;
        std::vector<long long int> RowSeperators;
    };
/*
    template<typename ValueT, typename IndexT>
    struct MatrixCompression_CSR {
        std::vector<ValueT> Values;
        std::vector<IndexT> ColumnIndexes;
        std::vector<IndexT> RowSeperators;
    };
*/

    struct MatrixCompression_CSR_DataCollection{
        MatrixCompression_CSR<short> type_pattern;
        MatrixCompression_CSR<float> type_float;
        MatrixCompression_CSR<double> type_double;
        MatrixCompression_CSR<int> type_int;
    };

    struct MatrixCompression_CSR_Size {
/*
        std::int64_t ValuesArraySizeInBytes;
        std::int64_t ColumnIndexesArraySizeInBytes;
        std::int64_t RowSeperatorsArraySizeInBytes;
        std::int64_t CSRSizeInBytes;
        std::int64_t SCSRSizeInBytes;
*/
        long long int ValuesArraySizeInBytes;
        long long int ColumnIndexesArraySizeInBytes;
        long long int RowSeperatorsArraySizeInBytes;
        long long int CSRSizeInBytes;
        long long int SCSRSizeInBytes;
    };

    struct MatrixCompression_CSR_Collection{
        MatrixCompression_CSR_DataCollection CSR_DataCollection;
        MatrixCompression_CSR_Size CSR_Structure_Size;
    };

    struct Segment {
        FieldType segmentDataType;
        MatrixSize segmentSize;
        SegmentRange segmentRangeRow;
        SegmentRange segmentRangeColumn;
        DataRange type_data_range;
        MatrixDataCollection segmentCollection;
    };

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
/*
        std::int64_t ValuesArraySizeInBytes;
        std::int64_t ColumnIndexesArraySizeInBytes;
        std::int64_t RowSeperatorsArraySizeInBytes;
        std::int64_t SCSRSizeInBytes;
*/
        long long int ValuesArraySizeInBytes;
        long long int ColumnIndexesArraySizeInBytes;
        long long int RowSeperatorsArraySizeInBytes;
        long long int SCSRSizeInBytes;
    };

    struct MatrixCompression_SCSR_Collection{
        MatrixCompression_SCSR_DataCollection SCSR_DataCollection;
        MatrixCompression_SCSR_Size CSSR_Structure_Size;
    };

    // For Compressed Column
    template<typename T>
    struct MatrixCompression_CSC {
        std::vector<T> Values;
        std::vector<long long int> RowIndexes;
        std::vector<long long int> ColumnSeparators;
    };

    struct MatrixCompression_CSC_DataCollection{
        MatrixCompression_CSC<short> type_pattern;
        MatrixCompression_CSC<float> type_float;
        MatrixCompression_CSC<double> type_double;
        MatrixCompression_CSC<int> type_int;
    };

    struct MatrixCompression_CSC_Size {
        long long int ValuesArraySizeInBytes;
        long long int RowIndexesArraySizeInBytes;
        long long int ColumnSeparatorsArraySizeInBytes;
        long long int CSCSizeInBytes;
        long long int SCSCSizeInBytes;
    };

    struct MatrixCompression_CSC_Collection{
        MatrixCompression_CSC_DataCollection CSC_DataCollection;
        MatrixCompression_CSC_Size CSC_Structure_Size;
    };

    struct NumberAnalysis{
        int NumberOfBits;
        int NumberOfBytes;
        bool Recast;
    };

    template<typename T>
    struct MatrixCompression_SCSC {
        std::vector<T> Values;
        std::vector<short> RowIndexes;
        std::vector<short> ColumnSeperators;
    };

    struct MatrixCompression_SCSC_DataCollection{
        MatrixCompression_SCSC<short> type_pattern;
        MatrixCompression_SCSC<float> type_float;
        MatrixCompression_SCSC<double> type_double;
        MatrixCompression_SCSC<int> type_int;
    };

    struct MatrixCompression_SCSC_Size {
        long long int ValuesArraySizeInBytes;
        long long int RowIndexesArraySizeInBytes;
        long long int ColumnSeperatorsArraySizeInBytes;
        long long int SCSCSizeInBytes;
    };

    struct MatrixCompression_SCSC_Collection{
        MatrixCompression_SCSC_DataCollection SCSC_DataCollection;
        MatrixCompression_SCSC_Size CSSC_Structure_Size;
    };
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
    const bool DisplayResult = true;
    const bool DisplayProgress = true;
    double defaultSegmentSize = 65535; // double -> ceil function requires double
*/
}

namespace Constants {
    const bool DisplayResult = true;
    const bool DisplayProgress = true;
    const double DefaultSegmentSize = 65535; // double -> ceil function requires double
//    const unsigned long MaxUnsignedNumber8Bytes = 18446744073709551615ul;
}

namespace Utilities {
    static int GetDataTypeLengthForIndexing(unsigned long aValue) {
        if (aValue <= 65535) { // 2 bytes
            return sizeof(unsigned short);
        } else if (aValue <= 4294967295) { // 4 bytes
            return sizeof(unsigned int);
        } else if (aValue <= ((unsigned long)18446744073709551615)) { // 8 bytes
            return sizeof(unsigned long);
        }
    }
}

#endif //MATRIXMARKETDATA_GLOBALDECLERATIONS_H
