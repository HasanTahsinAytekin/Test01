//
// Created by Hasan Aytekin on 25.07.2023.
//

#ifndef MATRIXMARKETDATA_BITNUMBER_H
#define MATRIXMARKETDATA_BITNUMBER_H

#include <iostream>
#include <cmath>
#include <typeinfo>
#include "GlobalDeclerations.h"

namespace Matrix {

    template <typename T>
    class BitNumber {
    private:
        struct FLOAT {
            union {
                float fValue;

                struct {
                    unsigned int mantissa: 23;
                    unsigned int exponent: 8;
                    unsigned int sign: 1;
                };
            };

            FLOAT(float f) {
                fValue = f;
            };
        };

        struct DOUBLE {
            union {
                double dValue;

                struct {
                    unsigned long long mantissa: 52;
                    unsigned int exponent: 11;
                    unsigned int sign: 1;
                };
            };

            DOUBLE(double d) {
                dValue = d;
            };
        };

        T numberToAnalyze;

        int numberOfBitsRequired(long long val) {
            return ceil(log2(val));
        }

        int numberOfBytesRequired(int numberOfBits) {
            return ceil(numberOfBits / 8.0);
        }
    public:
        NumberAnalysis numberAnalysisResult;

        BitNumber(T number) : numberToAnalyze(number) {
            numberAnalysisResult = AnalyzeNumber();
/*
            FLOAT f(number);

            std::cout << f.fValue << std::endl;
            std::cout << f.mantissa << std::endl;
            std::cout << f.exponent << std::endl;
            std::cout << f.sign << std::endl;
*/
        }

        NumberAnalysis AnalyzeNumber() {
            NumberAnalysis analysis;

            if (typeid(this->numberToAnalyze) == typeid(short)) {
//                analysis.NumberOfBits = numberOfBitsRequired(static_cast<long>(this->numberToAnalyze));
                analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                analysis.Recast = analysis.NumberOfBytes < sizeof(short);
//                std::cout << "Type is <short> and " << analysis.NumberOfBits << " bits are required to store " << this->numberToAnalyze << " into " << analysis.NumberOfBytes << " bytes." << (analysis.Recast ? " Recast.": " No need to Recast.") << std::endl;
            } else {
                if (typeid(this->numberToAnalyze) == typeid(unsigned short)) {
                    analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                    analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                    analysis.Recast = analysis.NumberOfBytes < sizeof(unsigned short);
//                    std::cout << "Type is <unsigned short> and " << analysis.NumberOfBits << " bits are required to store " << this->numberToAnalyze << " into " << analysis.NumberOfBytes << " bytes." << (analysis.Recast ? " Recast.": " No need to Recast.") << std::endl;
                } else {
                    if (typeid(this->numberToAnalyze) == typeid(int)) {
                        analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                        analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                        analysis.Recast = analysis.NumberOfBytes < sizeof(int);
//                        std::cout << "Type is <int> and " << analysis.NumberOfBits << " bits are required to store " << this->numberToAnalyze << " into " << analysis.NumberOfBytes << " bytes." << (analysis.Recast ? " Recast.": " No need to Recast.") << std::endl;
                    } else {
                        if (typeid(this->numberToAnalyze) == typeid(unsigned int)) {
                            analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                            analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                            analysis.Recast = analysis.NumberOfBytes < sizeof(unsigned int);
//                            std::cout << "Type is <unsigned int> and " << analysis.NumberOfBits
//                                      << " bits are required to store " << this->numberToAnalyze << " into " << analysis.NumberOfBytes
//                                      << " bytes." << (analysis.Recast ? " Recast." : " No need to Recast.")
//                                      << std::endl;
                        } else {
                            if (typeid(this->numberToAnalyze) == typeid(long)) {
                                analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                                analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                                analysis.Recast = analysis.NumberOfBytes < sizeof(unsigned long);
//                                std::cout << "Type is <long> and " << analysis.NumberOfBits
//                                          << " bits are required to store " << this->numberToAnalyze << " into "
//                                          << analysis.NumberOfBytes << " bytes."
//                                          << (analysis.Recast ? " Recast." : " No need to Recast.") << std::endl;
                            } else {
                                if (typeid(this->numberToAnalyze) == typeid(unsigned long)) {
                                    analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                                    analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                                    analysis.Recast = analysis.NumberOfBytes < sizeof(unsigned long);
//                                    std::cout << "Type is <unsigned long> and " << analysis.NumberOfBits
//                                              << " bits are required to store " << this->numberToAnalyze << " into "
//                                              << analysis.NumberOfBytes << " bytes."
//                                              << (analysis.Recast ? " Recast." : " No need to Recast.") << std::endl;
                                } else {
                                    if (typeid(this->numberToAnalyze) == typeid(long long)) {
                                        analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                                        analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                                        analysis.Recast = analysis.NumberOfBytes < sizeof(long long);
//                                    std::cout << "Type is <unsigned long> and " << analysis.NumberOfBits
//                                              << " bits are required to store " << this->numberToAnalyze << " into "
//                                              << analysis.NumberOfBytes << " bytes."
//                                              << (analysis.Recast ? " Recast." : " No need to Recast.") << std::endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            return analysis;
        }
    };

} // Matrix

#endif //MATRIXMARKETDATA_BITNUMBER_H
