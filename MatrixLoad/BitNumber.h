//
// Created by Hasan Aytekin on 28.07.2023.
//

#ifndef BITNUMBER_BITNUMBER_H
#define BITNUMBER_BITNUMBER_H

#include <iostream>
#include <cmath>
#include <typeinfo>

// Row Col Value

// Short8 Sort8 Byte
// Short8 Sort16 Byte
// Short16 Sort8 Byte
// Short16 Sort16 Byte

// Short8 Sort8 Byte
// Short8 Sort16 Byte
// Short16 Sort8 Byte
// Short16 Sort16 Byte

inline unsigned int AsInt(float f) {
    return *(unsigned int *) &f;
}

struct NumberAnalysis{
    int NumberOfBits;
    int NumberOfBytes;
    bool Recast;
};

template <typename T>
class BitNumber {
    private:
    struct S16 {
        union {
            short sValue;
            struct {
                unsigned short num: 15;
                unsigned short sign: 1;
            };
        };

        S16(short s) {
            sValue = s;
        }
    };

    struct S12 {
        union {
            short sValue;
            struct {
                unsigned short num: 11;
                unsigned short sign: 1;
            };
        };

        S12(short s) {
            sValue = s;
        }
    };

    struct S8 {
        union {
            short sValue;
            struct {
                unsigned short num: 7;
                unsigned short sign: 1;
            };
        };

        S8(short s) {
            sValue = s;
        }
    };

    struct NUM_unsigned {
        long long num_of_bits;
        long long num_of_bytes;

        union {
            long long llValue;
            struct {
                unsigned short num: 9;
//                unsigned short sign: 1;
            };
        };

        NUM_unsigned(long long anyNum) {
            llValue = anyNum;
//            num_of_bits = numberOfBitsRequired(anyNum);
//            num_of_bytes = numberOfBytesRequired(num_of_bits);
        }
    };

    struct FLOAT_NUMBER_PACKED_INTEGER {
        union {
            unsigned int Packed;
            struct {
                unsigned short integer;
                unsigned short fraction;
            };
        };
        FLOAT_NUMBER_PACKED_INTEGER(double f) {
            fraction = (short) modf(f, reinterpret_cast<double *>(&integer));
            float f1, f2;
            f1 = modff(f, &f2);
            int x1, x2, x3;
            x1 = (int) f2;
            x2 = (int) (f1 * 100);
            x3 = (f - (float) x1) * 100;
            int x = 0;
        }
    };
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
            return (val>0) ? ceil(log2(val)) : (val == 0) ? 0 : ceil(log2(std::abs(val)));
        }

        int numberOfBytesRequired(int numberOfBits) {
            return ceil(numberOfBits / 8.0);
        }

    void displayFloatInfo(FLOAT f) {
        std::cout << "FLOAT" << std::endl;
        std::cout << f.fValue << " (" << sizeof(f.fValue) << " bytes)" << std::endl;
        std::cout << f.sign << std::endl;
        std::cout << f.exponent << " (" << numberOfBytesRequired(numberOfBitsRequired(f.exponent)) << " bytes) [" << numberOfBitsRequired(f.exponent) << "]" << std::endl;
        std::cout << f.mantissa << " (" << numberOfBytesRequired(numberOfBitsRequired(f.mantissa)) << " bytes) [" << numberOfBitsRequired(f.mantissa) << "]" << std::endl;
    }
    void displayDoubleInfo(DOUBLE d) {
        std::cout << "DOUBLE" << std::endl;
        std::cout << d.dValue << " (" << sizeof(d.dValue) << " bytes)" << std::endl;
        std::cout << d.sign << std::endl;
        std::cout << d.exponent << " (" << numberOfBytesRequired(numberOfBitsRequired(d.exponent)) << " bytes) [" << numberOfBitsRequired(d.exponent) << "]" << std::endl;
        std::cout << d.mantissa << " (" << numberOfBytesRequired(numberOfBitsRequired(d.mantissa)) << " bytes) [" << numberOfBitsRequired(d.mantissa) << "]" << std::endl;
    }
    public:
        NumberAnalysis numberAnalysisResult;

        BitNumber(T number) : numberToAnalyze(number) {
            numberAnalysisResult = AnalyzeNumber();
        }

        NumberAnalysis AnalyzeNumber() {
            NumberAnalysis analysis;

            if (typeid(this->numberToAnalyze) == typeid(short)) {
                analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                analysis.Recast = analysis.NumberOfBytes < sizeof(short);
            } else {
                if (typeid(this->numberToAnalyze) == typeid(unsigned short)) {
                    analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                    analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                    analysis.Recast = analysis.NumberOfBytes < sizeof(unsigned short);
                } else {
                    if (typeid(this->numberToAnalyze) == typeid(int)) {
                        analysis.NumberOfBits = numberOfBitsRequired(this->numberToAnalyze);
                        analysis.NumberOfBytes = numberOfBytesRequired(analysis.NumberOfBits);
                        analysis.Recast = analysis.NumberOfBytes < sizeof(int);
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


#endif //BITNUMBER_BITNUMBER_H
