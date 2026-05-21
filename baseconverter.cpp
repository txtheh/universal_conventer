#include "baseconverter.h"
#include "inputparser.h"
#include <stdexcept>

std::string BaseConverter::digitToStr(long long d) {
    if (d < 10) return std::string(1, '0' + d);
    if (d < 36) return std::string(1, 'A' + (d - 10));
    return "[" + std::to_string(d) + "]";
}

std::string BaseConverter::integerToBase(BigInteger n, long long q) {
    if (n.isZero()) return "0";
    std::string result;
    BigInteger base(q);
    while (!n.isZero()) {
        long long rem = n % q;
        result = digitToStr(rem) + result;
        n = n / q;
    }
    return result;
}


BaseConverter::Result BaseConverter::fractionToBase(
    BigInteger remainder, BigInteger denom, long long q, size_t alreadyUsed)
{
    Result res;
    std::string fracDigits;
    std::map<BigInteger, size_t> seen;
    size_t pos = 0;
    size_t periodStart = std::string::npos;

    BigInteger base(q);

    while (!remainder.isZero()) {
        if (seen.count(remainder)) {
            periodStart = seen[remainder];
            break;
        }
        seen[remainder] = pos;

        remainder = remainder * base;
        BigInteger digitBig = remainder / denom;
        long long digit = digitBig % q;
        remainder = remainder % denom;
        fracDigits += digitToStr(digit);
        remainder = remainder % denom;
        pos++;

        if (alreadyUsed + fracDigits.size() >= MAX_OUTPUT_LENGTH) {
            res.truncated = true;
            res.output = fracDigits;
            return res;
        }
    }

    if (periodStart == std::string::npos) {
        res.output = fracDigits;
    } else {
        std::string nonPeriodic = fracDigits.substr(0, periodStart);
        std::string period      = fracDigits.substr(periodStart);
        res.output = nonPeriodic + "(" + period + ")";
    }
    return res;
}

BaseConverter::Result BaseConverter::convert(
    const std::string& input, long long p, long long q)
{
    BigFraction frac = InputParser::parse(input, p);

    BigInteger intPart = frac.integerPart();
    BigFraction fracRem = frac.fractionalRemainder();

    std::string intStr = integerToBase(intPart, q);

    if (fracRem.isZero()) {
        return {intStr, false};
    }

    size_t used = intStr.size() + 1;
    Result fracResult = fractionToBase(
        fracRem.numerator, fracRem.denominator, q, used);

    Result res;
    res.truncated = fracResult.truncated;
    res.output = intStr + "." + fracResult.output;
    if (fracResult.truncated)
        res.output += "...";
    return res;
}