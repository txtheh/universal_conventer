#ifndef BIGINTEGER_H
#define BIGINTEGER_H
#include <vector>
#include <string>

class BigInteger {
public:
    static const int BASE = 1000000000;
    static const int BASE_DIGITS = 9;

    std::vector<int> digits;

    BigInteger();
    BigInteger(long long n);
    BigInteger(const std::string& s);

    std::string toString() const;
    bool isZero() const;

    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(long long n) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(long long n) const;
    BigInteger operator/(const BigInteger& other) const;
    BigInteger operator%(const BigInteger& other) const;
    long long   operator%(long long n) const;

    static BigInteger gcd(BigInteger a, BigInteger b);

private:
    void removeLeadingZeros();
    BigInteger multiplySimple(const BigInteger& other) const;
    BigInteger multiplyKaratsuba(const BigInteger& other) const;
};

#endif // BIGINTEGER_H