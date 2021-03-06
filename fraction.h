#include "bigint.h"

template<typename T>
class Fraction {
public:
  Fraction(T _numerator, T _denominator);
  Fraction(T _numerator);
  Fraction(int64_t _constant);
  T getNumerator() const;
  T getDenominator() const;
private:
  T numerator, denominator;
};

template<typename T>
Fraction<T>::Fraction(T _numerator, T _denominator) {
  numerator = _numerator;
  denominator = _denominator;
  
  T factor = normalFactor(numerator, denominator);
  numerator = numerator / factor;
  denominator = denominator / factor;
}

template<typename T>
Fraction<T>::Fraction(T _numerator) {
  numerator = T(_numerator);
  denominator = T(1);
}

template<typename T>
Fraction<T>::Fraction(int64_t _constant) {
  numerator = T(_constant);
  denominator = T(1);
}

template<typename T>
T Fraction<T>::getNumerator() const {
  return numerator;
}

template<typename T>
T Fraction<T>::getDenominator() const {
  return denominator;
}

template<typename T>
bool operator == (const Fraction<T>& a, const Fraction<T>& b) {
  return (a.getNumerator() == b.getNumerator()) && (a.getDenominator() == b.getDenominator());
}

template<typename T>
Fraction<T> operator + (const Fraction<T>& a, const Fraction<T>& b) {
  return Fraction<T>(
    a.getNumerator() * b.getDenominator() + a.getDenominator() * b.getNumerator(),
    a.getDenominator() * b.getDenominator()
  );
}

template<typename T>
Fraction<T> operator - (const Fraction<T>& a) {
  return Fraction<T>(-a.getNumerator(), a.getDenominator());
}

template<typename T>
Fraction<T> operator - (const Fraction<T>& a, const Fraction<T>& b) {
  return a + (-b);
}

template<typename T>
Fraction<T> operator * (const Fraction<T>& a, const Fraction<T>& b) {
  return Fraction<T>(
    a.getNumerator() * b.getNumerator(),
    a.getDenominator() * b.getDenominator()
  );
}

template<typename T>
Fraction<T> inverse(const Fraction<T>& a) {
  return Fraction<T>(a.getDenominator(), a.getNumerator());
}

template<typename T>
Fraction<T> operator / (const Fraction<T>& a, const Fraction<T>& b) {
  return a * inverse(b);
}

template<typename T>
string toString(const Fraction<T>& a) {
  if(a.getNumerator() == T(0))
    return "0";
  if(a.getDenominator() == T(1))
    return toString(a.getNumerator());
  return toString(a.getNumerator()) + "/" + toString(a.getDenominator());
}

template<typename T>
Fraction<T> gcd(const Fraction<T>& a, const Fraction<T>& b) {
  return Fraction<T>(
    gcd(a.getNumerator(), b.getNumerator()),
    gcd(a.getDenominator(), b.getDenominator())
  );
}

typedef Fraction<BigInt> Rational;

ostream& operator << (ostream& out, const Rational &r) {
   out << toString(r);
   return out;
}

Rational round(Rational a) {
   return Rational((a.getNumerator() + a.getDenominator() / 2) / a.getDenominator());
}
