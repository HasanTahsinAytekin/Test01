#include <iostream>
#include <fstream>
#include <filesystem>
#include "GlobalDeclerations.h"
#include "MatrixMarketData.h"
#include "MatrixCSR.h"
#include "SegmentedMatrixCSR.h"

std::string fieldTypeName(Matrix::FieldType fType) {
    switch (fType) {

        case Matrix::Real:
            return "Real";
            break;
        case Matrix::Double:
            return "Double";
            break;
        case Matrix::Complex:
            return "Complex";
            break;
        case Matrix::Integer:
            return "Integer";
            break;
        case Matrix::Pattern:
            return "Pattern";
            break;
        case Matrix::UnknownField:
            return "Unknown";
            break;
    }
    return "Unknown";
}

std::string objectTypeName(Matrix::ObjectType oType) {
    switch (oType) {
        case Matrix::Matrix:
            return "Matrix";
            break;
        case Matrix::Vector:
            return "Vector";
            break;
        case Matrix::UnknownObject:
            return "Unknown";
            break;
    }
    return "Unknown";
}
std::string formatTypeName(Matrix::FormatType fType) {
    switch (fType) {
        case Matrix::Coordinate:
            return "Coordinate";
            break;
        case Matrix::Array:
            return "Array";
            break;
        case Matrix::UnknownFormat:
            return "Unknown";
            break;
    }
    return "Unknown";
}
std::string symmetryTypeName(Matrix::SymmetryType sType) {
    switch (sType) {
        case Matrix::General:
            return "General";
            break;
        case Matrix::Symmetric:
            return "Symmetric";
            break;
        case Matrix::SkewSymmetric:
            return "SkewSymmetric";
            break;
        case Matrix::Hermitian:
            return "Hermitian";
            break;
        case Matrix::UnknownSymmetry:
            return "UnknownSymmetry";
            break;
    }
    return "Unknown";
}

void processFile(std::string filePath, std::string fileName, std::ofstream& outFile) {
    auto mmData = new Matrix::MatrixMarketData(filePath);
    auto mmMatrixCollection = mmData->GetMatrixDataCollection();
//    Matrix::ObjectType mmObjectType = mmMatrixCollection.matrix_header.Object;
//    Matrix::FormatType mmFormatType = mmMatrixCollection.matrix_header.Format;
//    Matrix::SymmetryType mmSymmetryType = mmMatrixCollection.matrix_header.Symmetry;
//    Matrix::FieldType mmFieldType = mmMatrixCollection.matrix_header.Field;
    Matrix::DataRange mmDataRange = mmMatrixCollection.matrixDataCollection.type_data_range;
    auto matrixCSR = new Matrix::MatrixCSR(mmMatrixCollection);
    auto segmentedMatrixCSR = new Matrix::SegmentedMatrixCSR(mmMatrixCollection);

    Matrix::MatrixSize matrixSize = mmData->GetMatrixSize();

    std::cout << fileName <<
              " Object: " << objectTypeName(mmMatrixCollection.matrix_header.Object) <<
              " Format: " << formatTypeName(mmMatrixCollection.matrix_header.Format) <<
              " Symmetry: " << symmetryTypeName(mmMatrixCollection.matrix_header.Symmetry) <<
              " Rows: " << matrixSize.Rows << " Columns: " << matrixSize.Columns << " Nonzero Entries: " << matrixSize.NumberOfEntries <<
              " FieldType: " << fieldTypeName(mmMatrixCollection.matrix_header.Field) <<
              " MinValue: " << mmDataRange.Minimum <<
              " MaxValue: " << mmDataRange.Maximum <<
              " Coordinate (RIB): " << matrixSize.RowIndexSizeInBytes <<
              " Coordinate (CIB): " << matrixSize.ColumnIndexSizeInBytes <<
              " Coordinate (VB): " << matrixSize.ValueSizeInBytes <<
              " Coordinate: " << matrixSize.SizeInBytes <<
              " CSR: " << matrixCSR->GetMatrixSizeInBytes() <<
              " SCSR: " << segmentedMatrixCSR->GetMatrixSizeInBytes() <<
              " SCSR+: " << segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes() << std::endl;

    // Also log the results into the benchmark results file
    outFile << fileName <<
              "; " << objectTypeName(mmMatrixCollection.matrix_header.Object) <<
              "; " << formatTypeName(mmMatrixCollection.matrix_header.Format) <<
              "; " << symmetryTypeName(mmMatrixCollection.matrix_header.Symmetry) <<
              "; " << matrixSize.Rows << "; " << matrixSize.Columns << "; " << matrixSize.NumberOfEntries <<
              "; " << fieldTypeName(mmMatrixCollection.matrix_header.Field) <<
              "; " << mmDataRange.Minimum <<
              "; " << mmDataRange.Maximum <<
              "; " << matrixSize.RowIndexSizeInBytes <<
              "; " << matrixSize.ColumnIndexSizeInBytes <<
              "; " << matrixSize.ValueSizeInBytes <<
              "; " << matrixSize.SizeInBytes <<
              "; " << matrixCSR->GetMatrixSizeInBytes() <<
              "; " << segmentedMatrixCSR->GetMatrixSizeInBytes() <<
              "; " << segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes() << std::endl;

    delete mmData;
    delete matrixCSR;
    delete segmentedMatrixCSR;

}

void processFiles() {
    std::string path_to_dir = "C:\\IntelliJProjects\\ssget\\Data";

    try {
        //open file for writing
        std::ofstream fw("C:\\CLionProjects\\Test\\MatrixLoad\\SparseMatrixProposalBenchmark.csv", std::ofstream::out);
        //check if file was successfully opened for writing
        if (fw.is_open())
        {
            // Print the header info into the benchmark results file
            fw << "File; Object; Format; Symmetry; Rows; Columns; NonzeroEntries; FieldType; MinValue; MaxValue; Coordinate_RIB; Coordinate_CIB; Coordinate_VB; Coordinate_Bytes; CSR_Bytes; SCSR_Bytes; SCSR+_Bytes" << std::endl;

//            processFile("C:\\IntelliJProjects\\ssget\\Data\\bcsstk01.mtx", "bcsstk01.mtx", fw);

            for( const auto & entry : std::filesystem::directory_iterator( path_to_dir ) ) {
                processFile(entry.path().string(), entry.path().filename().string(), fw);
            }
            fw.close();
        }
        else std::cout << "Problem with opening file";
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
    }


//    for( const auto & entry : std::filesystem::directory_iterator( path_to_dir ) ){
//        processFile(entry.path().string(), entry.path().filename().string());
////        std::cout << entry.path().filename() << std::endl;
//    }
}

int main() {
//    std::string fileName = "C:\\CLionProjects\\Test\\MatrixMarketData\\lpi_cplex2.mtx";
/*    auto mmData = new Matrix::MatrixMarketData(
            "/mnt/c/CLionProjects/Test/MatrixMarketData/ash219.mtx");*/
//    auto mmData = new Matrix::MatrixMarketData("C:\\CLionProjects\\Test\\MatrixMarketData\\ash219.mtx");
//    auto mmData = new Matrix::MatrixMarketData("C:\\CLionProjects\\Test\\MatrixMarketData\\130bit.mtx");
    //auto mmData = new Matrix::MatrixMarketData(fileName);
//    auto mmData = new Matrix::MatrixMarketData("C:\\CLionProjects\\Test\\MatrixMarketData\\lpi_cplex2.mtx");
//    auto mmData = new Matrix::MatrixMarketData("C:\\CLionProjects\\Test\\MatrixMarketData\\CSR_Test.mtx");
    //auto matrixCSR = new Matrix::MatrixCSR(mmData->GetMatrixDataCollection());
    //auto segmentedMatrixCSR = new Matrix::SegmentedMatrixCSR(mmData->GetMatrixDataCollection());

/*
    std::string path_to_dir = "C:\\IntelliJProjects\\ssget\\Data";
    for( const auto & entry : std::filesystem::directory_iterator( path_to_dir ) ){
        std::cout << entry.path().filename() << std::endl;
    }

    std::cout << fileName << "Rows: " << "Columns: " << "Nonzero Entries: " <<
                             " Coordinate: " << mmData->GetMatrixSizeInBytes() <<
                             " CSR: " << matrixCSR->GetMatrixSizeInBytes() <<
                             " SCSR: " << segmentedMatrixCSR->GetMatrixSizeInBytes() << std::endl;

    delete mmData;
    delete matrixCSR;
    delete segmentedMatrixCSR;
*/

    processFiles();

/*
    auto x1 = new Matrix::BitNumber<int>(48);
    auto x2 = new Matrix::BitNumber<float>(-48.2f);
    auto x3 = new Matrix::BitNumber<short>(65);
    auto x4 = new Matrix::BitNumber<short>(257);
    auto x5 = new Matrix::BitNumber<unsigned short>(64250);
    auto x6 = new Matrix::BitNumber<int>(64250);

    delete x1;
    delete x2;
    delete x3;
    delete x4;
    delete x5;
    delete x6;
*/

    return 0;
}
