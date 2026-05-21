#ifndef BASECONVERTER_H
#define BASECONVERTER_H

#include "bigfraction.h"
#include <string>
#include <map>

const size_t MAX_OUTPUT_LENGTH = 100000;

class BaseConverter {
public:
    struct Result {
        std::string output;
        bool truncated = false;
    };
    static Result convert(const std::string& input, long long p, long long q);
private:
    static std::string integerToBase(BigInteger n, long long q);
    static Result fractionToBase(BigInteger remainder, BigInteger denom, long long q, size_t alreadyUsed);
    static std::string digitToStr(long long d);
};

#endif // BASECONVERTER_H