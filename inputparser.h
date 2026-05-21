#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include "bigfraction.h"
#include <string>

class InputParser {
public:
    static BigFraction parse(const std::string& input, long long base);
private:
    static BigInteger digitsToInteger(const std::vector<long long>& digits, long long base);
    static std::vector<long long> tokenize(const std::string& s, long long base);
};

#endif // INPUTPARSER_H