#include "inputparser.h"
#include <stdexcept>

static long long charToDigit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}


std::vector<long long> InputParser::tokenize(const std::string& s, long long base) {
    std::vector<long long> result;
    int i = 0;
    while (i < (int)s.size()) {
        if (s[i] == '[') {

            int j = s.find(']', i);
            if (j == (int)std::string::npos)
                throw std::invalid_argument("не закрыта квадратная скобка");
            std::string inside = s.substr(i + 1, j - i - 1);
            if (inside.empty())
                throw std::invalid_argument("пустые квадратные скобки");
            long long val = std::stoll(inside);
            if (val >= base)
                throw std::invalid_argument("цифра [" + inside + "] недопустима в основании " + std::to_string(base));
            result.push_back(val);
            i = j + 1;
        } else {
            long long val = charToDigit(s[i]);
            if (val < 0 || val >= base)
                throw std::invalid_argument("недопустимый символ '" + std::string(1, s[i]) + "'");
            result.push_back(val);
            i++;
        }
    }
    return result;
}

BigInteger InputParser::digitsToInteger(const std::vector<long long>& digits, long long base) {
    BigInteger result(0);
    BigInteger b(base);
    for (long long d : digits) {
        result = result * b + BigInteger(d);
    }
    return result;
}


BigFraction InputParser::parse(const std::string& input, long long base) {
    if (input.empty())
        throw std::invalid_argument("входная строка пуста");

    int dotPos = -1;
    int periodOpen = -1;
    int periodClose = -1;

    bool inSquare = false;
    for (int i = 0; i < (int)input.size(); ++i) {
        if (input[i] == '[') { inSquare = true; continue; }
        if (input[i] == ']') { inSquare = false; continue; }
        if (inSquare) continue;
        if (input[i] == '.') {
            if (dotPos != -1)
                throw std::invalid_argument("более одной точки");
            dotPos = i;
        }
        if (input[i] == '(') periodOpen = i;
        if (input[i] == ')') periodClose = i;
    }

    std::string intPart, fracPart, periodPart;

    if (dotPos == -1 && periodOpen == -1) {
        intPart = input;
    } else if (dotPos != -1 && periodOpen == -1) {
        if (dotPos == 0)
            throw std::invalid_argument("отсутствует целая часть");
        if (dotPos == (int)input.size() - 1)
            throw std::invalid_argument("отсутствует дробная часть после точки");
        intPart = input.substr(0, dotPos);
        fracPart = input.substr(dotPos + 1);
    } else if (dotPos != -1 && periodOpen != -1) {
        if (dotPos == 0)
            throw std::invalid_argument("отсутствует целая часть");
        if (periodClose == -1)
            throw std::invalid_argument("не закрыта скобка периода");
        if (periodClose != (int)input.size() - 1)
            throw std::invalid_argument("после закрывающей скобки периода не должно быть других символов");
        if (periodClose == periodOpen + 1)
            throw std::invalid_argument("период не может быть пустым");
        intPart = input.substr(0, dotPos);
        fracPart = input.substr(dotPos + 1, periodOpen - dotPos - 1);
        periodPart = input.substr(periodOpen + 1, periodClose - periodOpen - 1);
    } else {
        throw std::invalid_argument("некорректный формат числа");
    }

    BigInteger b(base);
    auto intDigits = tokenize(intPart, base);
    BigInteger I = digitsToInteger(intDigits, base);

    if (fracPart.empty() && periodPart.empty()) {
        return BigFraction(I, BigInteger(1));
    }

    auto fracDigits = tokenize(fracPart, base);
    int k = fracDigits.size();
    BigInteger F = digitsToInteger(fracDigits, base);

    BigInteger pk(1);
    for (int i = 0; i < k; ++i) pk = pk * b;

    if (periodPart.empty()) {
        BigInteger num = I * pk + F;
        return BigFraction(num, pk);
    }

    auto periodDigits = tokenize(periodPart, base);
    int m = periodDigits.size();
    BigInteger P = digitsToInteger(periodDigits, base);

    BigInteger pm(1);
    for (int i = 0; i < m; ++i) pm = pm * b;

    BigInteger pm1 = pm - BigInteger(1);
    BigInteger num = I * pk * pm1 + F * pm1 + P;
    BigInteger den = pk * pm1;
    return BigFraction(num, den);
}