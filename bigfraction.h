#ifndef BIGFRACTION_H
#define BIGFRACTION_H
#include "biginteger.h"

    class BigFraction {
public:
    BigInteger numerator;
    BigInteger denominator;
    BigFraction();
    BigFraction(BigInteger num, BigInteger den);
    void reduce();
    BigFraction operator+(const BigFraction& other) const;
    BigFraction operator-(const BigFraction& other) const;
    BigFraction operator*(const BigFraction& other) const;
    BigFraction operator/(const BigFraction& other) const;
    BigInteger integerPart() const;
    BigFraction fractionalRemainder() const;
    bool isZero() const;
};

#endif // BIGFRACTION_H
