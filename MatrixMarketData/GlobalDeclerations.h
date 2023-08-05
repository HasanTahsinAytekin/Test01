//
// Created by Hasan Aytekin on 21.07.2023.
//

#ifndef MATRIXMARKETDATA_GLOBALDECLERATIONS_H
#define MATRIXMARKETDATA_GLOBALDECLERATIONS_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

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
        std::int64_t Rows;
        std::int64_t Columns;
        std::int64_t NumberOfEntries;
        std::int64_t SizeInBytes;   // Array Size
    };

    template<typename T>
    struct Data {
        std::int64_t Row;
        std::int64_t Column;
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

    template<typename T>
    struct MatrixCompression_CSR {
        std::vector<T> Values;
        std::vector<int64_t> ColumnIndexes;
        std::vector<int64_t> RowSeperators;
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
        std::int64_t ValuesArraySizeInBytes;
        std::int64_t ColumnIndexesArraySizeInBytes;
        std::int64_t RowSeperatorsArraySizeInBytes;
        std::int64_t CSRSizeInBytes;
        std::int64_t SCSRSizeInBytes;
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
        std::int64_t ValuesArraySizeInBytes;
        std::int64_t ColumnIndexesArraySizeInBytes;
        std::int64_t RowSeperatorsArraySizeInBytes;
        std::int64_t SCSRSizeInBytes;
    };

    struct MatrixCompression_SCSR_Collection{
        MatrixCompression_SCSR_DataCollection SCSR_DataCollection;
        MatrixCompression_SCSR_Size CSSR_Structure_Size;
    };

    struct NumberAnalysis{
        int NumberOfBits;
        int NumberOfBytes;
        bool Recast;
    };


}
#endif //MATRIXMARKETDATA_GLOBALDECLERATIONS_H
