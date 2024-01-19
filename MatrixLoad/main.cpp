#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <chrono>
#include "GlobalDeclerations.h"
#include "MatrixMarketData.h"
#include "MatrixCSR.h"
#include "SegmentedMatrixCSR.h"
#include "MatrixCSC.h"
#include "SegmentedMatrixCSC.h"
#include "MatrixTimingBenchmark.h"



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
//const bool DisplayResult = true;
//const bool DisplayProgress = true;
//double defaultSegmentSize = 65535; // double -> ceil function requires double





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
    time_t now = time(0); // get current date and time
    tm* ltm = localtime(&now);

    auto start{std::chrono::steady_clock::now()};

    if (Constants::DisplayProgress) std::cout << fileName << " -> Loading...";
    auto mmData = new Matrix::MatrixMarketData(filePath);
    if (Constants::DisplayProgress) std::cout << "Done";

    Matrix::MatrixSize matrixSize = mmData->GetMatrixSize();
    auto& mmMatrixCollection = mmData->GetMatrixDataCollection();
//    free(mmData);

//    Matrix::ObjectType mmObjectType = mmMatrixCollection.matrix_header.Object;
//    Matrix::FormatType mmFormatType = mmMatrixCollection.matrix_header.Format;
//    Matrix::SymmetryType mmSymmetryType = mmMatrixCollection.matrix_header.Symmetry;
//    Matrix::FieldType mmFieldType = mmMatrixCollection.matrix_header.Field;
    Matrix::DataRange mmDataRange = mmMatrixCollection.matrixDataCollection.type_data_range;
    // CSR
    auto matrixCSR = new Matrix::MatrixCSR(mmMatrixCollection);
    auto matrixCSRSizeInBytes = matrixCSR->GetMatrixSizeInBytes();
    auto x = sizeof(matrixCSR);
    // Benchmark Timing
//    auto benchmarkTiming = new Matrix::MatrixTimingBenchmark();
//    Matrix::MatrixCompression_CSR_Collection& xxxxxxx = matrixCSR->GetMatrixCSRCollectionAddress();
    auto benchmarkTimingMatrixCSR = new Matrix::MatrixTimingBenchmark::MatrixCSR(mmMatrixCollection, matrixCSR->GetMatrixCSRCollectionAddress());
    long long int btCSR = benchmarkTimingMatrixCSR->CSR_Benchmark_Timing;
    delete benchmarkTimingMatrixCSR;
    delete matrixCSR;

    auto segmentedMatrixCSR = new Matrix::SegmentedMatrixCSR(mmMatrixCollection);
    auto segmentedMatrixCSRSizeInBytes = segmentedMatrixCSR->GetMatrixSizeInBytes();
    auto segmentedMatrixCSRSizeWithBitNumberInBytes = segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes();
    // Benchmark Timing
    auto benchmarkSegmentedMatrixCSR = new Matrix::MatrixTimingBenchmark::SegmentedMatrixCSR(mmMatrixCollection, segmentedMatrixCSR->GetSegmentMapAddress(), segmentedMatrixCSR->GetCSRSegmentsAddress());
    long long int btSCSR = benchmarkSegmentedMatrixCSR->SCSR_Benchmark_Timing;
    delete benchmarkSegmentedMatrixCSR;
    delete segmentedMatrixCSR;

    // CSC
    auto matrixCSC = new Matrix::MatrixCSC(mmMatrixCollection);
    auto matrixCSCSizeInBytes = matrixCSC->GetMatrixSizeInBytes();
    delete matrixCSC;

    auto segmentedMatrixCSC = new Matrix::SegmentedMatrixCSC(mmMatrixCollection);
    auto segmentedMatrixCSCSizeInBytes = segmentedMatrixCSC->GetMatrixSizeInBytes();
    auto segmentedMatrixCSCSizeWithBitNumberInBytes = segmentedMatrixCSC->GetMatrixSizeWithBitNumberInBytes();
    delete segmentedMatrixCSC;

    auto stop{std::chrono::steady_clock::now()};
    auto time_diff = stop - start;
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(time_diff);

//    Matrix::MatrixSize matrixSize = mmData->GetMatrixSize();

    if (Constants::DisplayResult)
        std::cout << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " (" << elapsed.count()  << ") | " <<
                  fileName <<
                  " Obj: " << objectTypeName(mmMatrixCollection.matrix_header.Object) <<
                  " Format: " << formatTypeName(mmMatrixCollection.matrix_header.Format) <<
                  " Symm: " << symmetryTypeName(mmMatrixCollection.matrix_header.Symmetry) <<
                  " Rows: " << matrixSize.Rows << " Cols: " << matrixSize.Columns << " NE: " << matrixSize.NumberOfEntries <<
                  " FType: " << fieldTypeName(mmMatrixCollection.matrix_header.Field) <<
                  " MinVal: " << mmDataRange.Minimum <<
                  " MaxVal: " << mmDataRange.Maximum <<
                  " Coor(RIB): " << matrixSize.RowIndexSizeInBytes <<
                  " Coor(CIB): " << matrixSize.ColumnIndexSizeInBytes <<
                  " Coor(VB): " << matrixSize.ValueSizeInBytes <<
                  " Coor: " << matrixSize.SizeInBytes <<
                  " CSR: " << matrixCSRSizeInBytes << // matrixCSR->GetMatrixSizeInBytes() <<
                  " CSR-T: " << btCSR <<
                  " SCSR: " << segmentedMatrixCSRSizeInBytes << // segmentedMatrixCSR->GetMatrixSizeInBytes() <<
                  " SCSR-T: " << btSCSR <<
                  " SCSR+: " << segmentedMatrixCSRSizeWithBitNumberInBytes << // segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes() <<
                  " CSC: " << matrixCSCSizeInBytes << // matrixCSC->GetMatrixSizeInBytes() <<
                  " SCSC: " << segmentedMatrixCSCSizeInBytes << // segmentedMatrixCSC->GetMatrixSizeInBytes() <<
                  " SCSC+: " << segmentedMatrixCSCSizeWithBitNumberInBytes << // segmentedMatrixCSC->GetMatrixSizeWithBitNumberInBytes() <<
                  std::endl;
/*
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
              " SCSR+: " << segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes() <<
              " CSC: " << matrixCSC->GetMatrixSizeInBytes() <<
              " SCSR: " << segmentedMatrixCSC->GetMatrixSizeInBytes() <<
              " SCSR+: " << segmentedMatrixCSC->GetMatrixSizeWithBitNumberInBytes() << std::endl;
*/


    // Also log the results into the benchmark results file
    outFile << elapsed.count() <<
              "; " << fileName <<
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
              "; " << matrixCSRSizeInBytes << // matrixCSR->GetMatrixSizeInBytes() <<
              "; " << btCSR <<
              "; " << segmentedMatrixCSRSizeInBytes << // segmentedMatrixCSR->GetMatrixSizeInBytes() <<
              "; " << btSCSR <<
              "; " << segmentedMatrixCSRSizeWithBitNumberInBytes << // segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes() <<
              "; " << matrixCSCSizeInBytes << // matrixCSC->GetMatrixSizeInBytes() <<
              "; " << segmentedMatrixCSCSizeInBytes << // segmentedMatrixCSC->GetMatrixSizeInBytes() <<
              "; " << segmentedMatrixCSCSizeWithBitNumberInBytes << // segmentedMatrixCSC->GetMatrixSizeWithBitNumberInBytes() <<
              std::endl;

    delete mmData;
/*
    delete mmData;
    delete matrixCSR;
    delete segmentedMatrixCSR;
    delete matrixCSC;
    delete segmentedMatrixCSC;
*/

}

std::string appendTimeStampToFileName(std::string fileName){
    char filename_extension[100];
    time_t now = time(0);
    struct tm tstruct;

    localtime_s(&tstruct, &now);
//    strftime(filename_extension, sizeof(filename_extension), "_%Y-%m-%d_%H-%M-%S.csv", &tstruct);
    strftime(filename_extension, sizeof(filename_extension), "_%Y%m%d-%H%M%S.csv", &tstruct);

    fileName.append(filename_extension);

    return fileName;
}

void processFiles() {
    std::string path_to_dir = "C:\\IntelliJProjects\\ssget\\Data";

    try {
//        std::string result_file_name = "C:\\CLionProjects\\Test\\MatrixLoad\\SparseMatrixProposalBenchmark";
//        std::string result_file_name = appendTimeStampToFileName("C:\\CLionProjects\\Test\\MatrixLoad\\SparseMatrixProposalBenchmark");

        //open file for writing
//        std::ofstream fw("C:\\CLionProjects\\Test\\MatrixLoad\\SparseMatrixProposalBenchmark.csv", std::ofstream::out);
        std::ofstream fw(appendTimeStampToFileName("C:\\CLionProjects\\Test\\MatrixLoad\\SparseMatrixProposalBenchmark"), std::ofstream::out);
        //check if file was successfully opened for writing
        if (fw.is_open())
        {
            // Print the header info into the benchmark results file
            fw << "ElapsedS; File; Object; Format; Symmetry; Rows; Columns; NonzeroEntries; FieldType; MinValue; MaxValue; Coordinate_RIB; Coordinate_CIB; Coordinate_VB; Coordinate_Bytes; CSR_Bytes; CSR_Timing; SCSR_Bytes; SCSR_Timing; SCSR+_Bytes; CSC_Bytes; SCSC_Bytes; SCSC+_Bytes" << std::endl;

/*
            if (Constants::DisplayProgress) std::cout << "bcsstk01.mtx";
            processFile("C:\\IntelliJProjects\\ssget\\Data\\bcsstk01.mtx", "bcsstk01.mtx", fw);
*/
//            processFile("C:\\IntelliJProjects\\ssget\\Data\\JP.mtx", "JP.mtx", fw);
//            processFile("C:\CLionProjects\Test\MatrixMarketData\CSR_Test.mtx", "CSR_Test.mtx", fw);
/*
            if (Constants::DisplayProgress) std::cout << "mycielskian17.mtx";
            processFile("C:\\IntelliJProjects\\ssget\\Data\\mycielskian17.mtx", "mycielskian17.mtx", fw);
*/

//            processFile("C:\\IntelliJProjects\\ssget\\Data\\CSR_Test.mtx", "CSR_Test.mtx", fw);
//            processFile("C:\\IntelliJProjects\\ssget\\Data\\144_Dbg.mtx", "144_Dbg.mtx", fw);

//            processFile("C:\\IntelliJProjects\\ssget\\Data\\lp_ken_18.mtx", "lp_ken_18.mtx", fw);

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

    double x1 = Constants::DefaultSegmentSizeTest;
    double x2 = (double) Constants::DefaultSegmentSizeTest;
    int x3 = (int) Constants::DefaultSegmentSize;
    long x4 = (long) Constants::DefaultSegmentSize;
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
