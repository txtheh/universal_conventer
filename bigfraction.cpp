#include "bigfraction.h"


BigFraction::BigFraction() {
    numerator = BigInteger(0);
    denominator = BigInteger(1);
}

BigFraction::BigFraction(BigInteger num, BigInteger den) {
    numerator = num;
    denominator = den;
    reduce();
}


void BigFraction::reduce() {
    BigInteger g = BigInteger::gcd(numerator, denominator);
    numerator = numerator / g;
    denominator = denominator / g;
}

bool BigFraction::isZero() const {
    return numerator.isZero();
}

BigInteger BigFraction::integerPart() const {
    return numerator / denominator;
}


BigFraction BigFraction::fractionalRemainder() const {
    return BigFraction(numerator % denominator, denominator);
}


BigFraction BigFraction::operator+(const BigFraction& other) const {
    BigInteger num = numerator * other.denominator + other.numerator * denominator;
    BigInteger den = denominator * other.denominator;
    return BigFraction(num, den);
}


BigFraction BigFraction::operator-(const BigFraction& other) const {
    BigInteger num = numerator * other.denominator - other.numerator * denominator;
    BigInteger den = denominator * other.denominator;
    return BigFraction(num, den);
}

BigFraction BigFraction::operator*(const BigFraction& other) const {
    return BigFraction(numerator * other.numerator, denominator * other.denominator);
}

BigFraction BigFraction::operator/(const BigFraction& other) const {
    return BigFraction(numerator * other.denominator, denominator * other.numerator);
}