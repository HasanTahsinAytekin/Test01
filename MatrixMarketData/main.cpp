#include <iostream>
#include <filesystem>
#include "MatrixMarketData.h"
#include "MatrixCSR.h"
#include "SegmentedMatrixCSR.h"
//#include "BitNumber.h"

void processFile(std::string filePath, std::string fileName, std::ofstream& outFile) {
    auto mmData = new Matrix::MatrixMarketData(filePath);
    Matrix::DataRange mmDataRange = mmData->GetMatrixDataCollection().matrixDataCollection.type_data_range;
    auto matrixCSR = new Matrix::MatrixCSR(mmData->GetMatrixDataCollection());
    auto segmentedMatrixCSR = new Matrix::SegmentedMatrixCSR(mmData->GetMatrixDataCollection());

    Matrix::MatrixSize matrixSize = mmData->GetMatrixSize();

    std::cout << fileName <<
              " Rows: " << matrixSize.Rows << " Columns: " << matrixSize.Columns << " Nonzero Entries: " << matrixSize.NumberOfEntries <<
              " MinValue: " << mmDataRange.Minimum <<
              " MaxValue: " << mmDataRange.Maximum <<
              " Coordinate: " << matrixSize.SizeInBytes <<
              " CSR: " << matrixCSR->GetMatrixSizeInBytes() <<
              " SCSR: " << segmentedMatrixCSR->GetMatrixSizeInBytes() <<
              " SCSR+: " << segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes() << std::endl;

    // Also log the results into the benchmark results file
    outFile << fileName <<
              ", " << matrixSize.Rows << ", " << matrixSize.Columns << ", " << matrixSize.NumberOfEntries <<
              ", " << mmDataRange.Minimum <<
              ", " << mmDataRange.Maximum <<
              ", " << matrixSize.SizeInBytes <<
              ", " << matrixCSR->GetMatrixSizeInBytes() <<
              ", " << segmentedMatrixCSR->GetMatrixSizeInBytes() <<
              ", " << segmentedMatrixCSR->GetMatrixSizeWithBitNumberInBytes() << std::endl;

    delete mmData;
    delete matrixCSR;
    delete segmentedMatrixCSR;

}

void processFiles() {
    std::string path_to_dir = "C:\\IntelliJProjects\\ssget\\Data";

    try {
        //open file for writing
        std::ofstream fw("C:\\CLionProjects\\Test\\MatrixMarketData\\SparseMatrixProposalBenchmark.csv", std::ofstream::out);
        //check if file was successfully opened for writing
        if (fw.is_open())
        {
            // Print the header info into the benchmark results file
            fw << "File, Rows, Columns, NonzeroEntries, MinValue, MaxValue, Coordinate_Bytes, CSR_Bytes, SCSR_Bytes, SCSR+_Bytes" << std::endl;

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
