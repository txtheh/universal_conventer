#include "biginteger.h"
#include <algorithm>

BigInteger::BigInteger() {
    digits.push_back(0);
}

BigInteger::BigInteger(long long n) {
    if (n == 0) {
        digits.push_back(0);
        return;
    }
    while (n > 0) {
        digits.push_back(n % BASE);
        n /= BASE;
    }
}

BigInteger::BigInteger(const std::string& s) {
    for (int i = s.size(); i > 0; i -= BASE_DIGITS) {
        int from = std::max(0, i - BASE_DIGITS);
        digits.push_back(std::stoi(s.substr(from, i - from)));
    }
    removeLeadingZeros();
}

void BigInteger::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
}

bool BigInteger::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}

std::string BigInteger::toString() const {
    std::string res = std::to_string(digits.back());
    for (int i = digits.size() - 2; i >= 0; i--) {
        std::string p = std::to_string(digits[i]);
        while (p.size() < BASE_DIGITS)
            p = "0" + p;
        res += p;
    }
    return res;
}

bool BigInteger::operator==(const BigInteger& b) const {
    return digits == b.digits;
}
bool BigInteger::operator!=(const BigInteger& b) const {
    return !(*this == b);
}
bool BigInteger::operator<(const BigInteger& b) const {
    if (digits.size() != b.digits.size())
        return digits.size() < b.digits.size();
    for (int i = digits.size() - 1; i >= 0; i--)
        if (digits[i] != b.digits[i])
            return digits[i] < b.digits[i];
    return false;
}
bool BigInteger::operator<=(const BigInteger& b) const { return !(b < *this); }
bool BigInteger::operator>(const BigInteger& b) const  { return b < *this; }
bool BigInteger::operator>=(const BigInteger& b) const { return !(*this < b); }

BigInteger BigInteger::operator+(const BigInteger& b) const {
    BigInteger res;
    res.digits.clear();
    int c = 0;
    int n = std::max(digits.size(), b.digits.size());
    for (int i = 0; i < n || c; i++) {
        long long s = c;
        if (i < digits.size())   s += digits[i];
        if (i < b.digits.size()) s += b.digits[i];
        res.digits.push_back(s % BASE);
        c = s / BASE;
    }
    return res;
}

BigInteger BigInteger::operator-(const BigInteger& b) const {
    BigInteger res;
    res.digits.clear();
    int bor = 0;
    for (int i = 0; i < digits.size(); i++) {
        long long d = digits[i] - bor;
        if (i < b.digits.size()) d -= b.digits[i];
        if (d < 0) {
            d += BASE;
            bor = 1;
        } else {
            bor = 0;
        }
        res.digits.push_back(d);
    }
    res.removeLeadingZeros();
    return res;
}

BigInteger BigInteger::operator*(long long n) const {
    BigInteger res;
    res.digits.clear();
    long long c = 0;
    for (int i = 0; i < digits.size() || c; i++) {
        long long cur = c;
        if (i < digits.size()) cur += digits[i] * n;
        res.digits.push_back(cur % BASE);
        c = cur / BASE;
    }
    res.removeLeadingZeros();
    return res;
}

BigInteger BigInteger::multiplySimple(const BigInteger& b) const {
    BigInteger res;
    res.digits.assign(digits.size() + b.digits.size(), 0);
    for (int i = 0; i < (int)digits.size(); i++) {
        long long c = 0;
        for (int j = 0; j < (int)b.digits.size() || c; j++) {
            long long cur = res.digits[i + j] + c;
            if (j < (int)b.digits.size())
                cur += (long long)digits[i] * b.digits[j];
            res.digits[i + j] = cur % BASE;
            c = cur / BASE;
        }
    }
    res.removeLeadingZeros();
    return res;
}

BigInteger BigInteger::multiplyKaratsuba(const BigInteger& b) const {
    if (isZero() || b.isZero())
        return BigInteger();

    if ((int)digits.size() <= 32 || (int)b.digits.size() <= 32)
        return multiplySimple(b);

    int m = std::max(digits.size(), b.digits.size()) / 2;

    BigInteger a0, a1, b0, b1;

    for (int i = 0; i < std::min(m, (int)digits.size()); ++i)
        a0.digits.push_back(digits[i]);
    for (int i = m; i < (int)digits.size(); ++i)
        a1.digits.push_back(digits[i]);

    for (int i = 0; i < std::min(m, (int)b.digits.size()); ++i)
        b0.digits.push_back(b.digits[i]);
    for (int i = m; i < (int)b.digits.size(); ++i)
        b1.digits.push_back(b.digits[i]);

    a0.removeLeadingZeros(); a1.removeLeadingZeros();
    b0.removeLeadingZeros(); b1.removeLeadingZeros();

    BigInteger z0 = a0.multiplyKaratsuba(b0);
    BigInteger z2 = a1.multiplyKaratsuba(b1);
    BigInteger z1 = (a0 + a1).multiplyKaratsuba(b0 + b1) - z0 - z2;

    z2.digits.insert(z2.digits.begin(), 2 * m, 0);
    z1.digits.insert(z1.digits.begin(), m, 0);

    BigInteger res = z2 + z1 + z0;
    res.removeLeadingZeros();
    return res;
}

BigInteger BigInteger::operator*(const BigInteger& b) const {
    return multiplyKaratsuba(b);
}

BigInteger BigInteger::operator/(long long n) const {
    BigInteger res;
    res.digits.resize(digits.size());
    long long c = 0;
    for (int i = digits.size() - 1; i >= 0; i--) {
        long long cur = digits[i] + c * BASE;
        res.digits[i] = cur / n;
        c = cur % n;
    }
    res.removeLeadingZeros();
    return res;
}

long long BigInteger::operator%(long long n) const {
    long long c = 0;
    for (int i = digits.size() - 1; i >= 0; i--)
        c = (digits[i] + c * BASE) % n;
    return c;
}

BigInteger BigInteger::operator/(const BigInteger& b) const {
    BigInteger lo(0), hi(*this), res(0);
    while (lo <= hi) {
        BigInteger mid = (lo + hi) / 2LL;
        if (mid * b <= *this) {
            res = mid;
            lo = mid + BigInteger(1);
        } else {
            hi = mid - BigInteger(1);
        }
    }
    return res;
}

BigInteger BigInteger::operator%(const BigInteger& b) const {
    return *this - (*this / b) * b;
}

BigInteger BigInteger::gcd(BigInteger a, BigInteger b) {
    while (!b.isZero()) {
        a = a % b;
        std::swap(a, b);
    }
    return a;
}