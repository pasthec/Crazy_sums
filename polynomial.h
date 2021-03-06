#include "fraction.h"
using namespace std;

template<typename T>
class Polynomial {
public:
  Polynomial(vector<T> _coeffs = vector<T>());
  Polynomial(int64_t _constant);
  size_t size() const;
  T getCoeff(size_t pos) const;
  void setCoeff(size_t pos, T coeff);
  void reduce();
private:
  vector<T> coeffs;
};

template<typename T>
Polynomial<T>::Polynomial(vector<T> _coeffs) {
  coeffs = _coeffs;
  reduce();
}

template<typename T>
Polynomial<T>::Polynomial(int64_t _constant) {
  coeffs.clear();
  coeffs.push_back(T(_constant));
  reduce();
}

template<typename T>
size_t Polynomial<T>::size() const {
  return coeffs.size();
}

template<typename T>
T Polynomial<T>::getCoeff(size_t pos) const {
  if(pos >= coeffs.size())
    return T(0);
  return coeffs[pos];
}

template<typename T>
void Polynomial<T>::setCoeff(size_t pos, T coeff) {
  while(pos >= coeffs.size()) {
    coeffs.push_back(T(0));
  }
  coeffs[pos] = coeff;
  reduce();
}

template<typename T>
bool operator == (const Polynomial<T>& a, const Polynomial<T>& b) {
  if(a.size() != b.size())
    return false;
    
  for(int iCoeff = 0;iCoeff < (int)a.size();iCoeff++) {
    if(!(a.getCoeff(iCoeff) == b.getCoeff(iCoeff)))
      return false;
  }
  return true;
}


template<typename T>
void Polynomial<T>::reduce() {
  while(!coeffs.empty() && coeffs.back() == T(0)) {
    coeffs.pop_back();
  }
}

template<typename T>
Polynomial<T> operator << (const Polynomial<T>& a, size_t shift) {
  Polynomial<T> sum;
  
  for(size_t iCoeff = 0;iCoeff < a.size();iCoeff++) {
    sum.setCoeff(iCoeff + shift, a.getCoeff(iCoeff));
  }
  
  sum.reduce();
  return sum;  
}

template<typename T>
Polynomial<T> operator + (const Polynomial<T>& a, const Polynomial<T>& b) {
  Polynomial<T> sum;
  for(size_t iCoeff = 0;iCoeff < max(a.size(), b.size());iCoeff++) {
    sum.setCoeff(iCoeff, a.getCoeff(iCoeff) + b.getCoeff(iCoeff));
  }
  sum.reduce();
  return sum;
}

template<typename V, typename T>
Polynomial<T> operator * (const V& a, const Polynomial<T>& b) {
  Polynomial<T> sum;
  for(size_t iCoeff = 0;iCoeff < b.size();iCoeff++) {
    sum.setCoeff(iCoeff, a * b.getCoeff(iCoeff));
  }
  sum.reduce();
  return sum;
}

template<typename V, typename T>
Polynomial<T> operator / (const Polynomial<T>& a, const V& b) {
  Polynomial<T> sum;
  for(size_t iCoeff = 0;iCoeff < a.size();iCoeff++) {
    sum.setCoeff(iCoeff, a.getCoeff(iCoeff) / b);
  }
  sum.reduce();
  return sum;
}


template<typename T>
Polynomial<T> operator - (const Polynomial<T>& a) {
  return T(-1) * a;
}

template<typename T>
Polynomial<T> operator - (const Polynomial<T>& a, const Polynomial<T>& b) {
  return a + (-b);
}

template<typename T>
Polynomial<T> operator * (const Polynomial<T>& a, const Polynomial<T>& b) {
  Polynomial<T> sum;
  for(size_t iCoeffA = 0;iCoeffA < a.size();iCoeffA++) {
    for(size_t iCoeffB = 0;iCoeffB < b.size();iCoeffB++) {
      sum.setCoeff(iCoeffA + iCoeffB, sum.getCoeff(iCoeffA + iCoeffB) + a.getCoeff(iCoeffA) * b.getCoeff(iCoeffB));
    }
  }
  
  sum.reduce();
  return sum;
}

//TODO: faire mieux
Rational leading(const Rational& a) {
  return a;
}

template<typename T>
Rational leading(const Polynomial<T>& a) {
  return leading(a.getCoeff(a.size() - 1));
}

template<typename T> 
Polynomial<T> toMonic(const Polynomial<T>& a) {
  return inverse(leading(a)) * a;
}

template<typename T, typename... Args>
string toString(const Polynomial<T>& poly, string variable, Args... args) {
  string result;
  
  bool first = true;
  for(size_t iCoeff = 0;iCoeff < poly.size();iCoeff++) {
    if(!(poly.getCoeff(iCoeff) == T(0))) {
      if(!first) {
        result += " + ";
      }
      first = false;
      
     result += toString(poly.getCoeff(iCoeff), args...) + variable + "^" + to_string(iCoeff);
    }
  }
  
  return "(" + result + ")";
}

typedef Polynomial<Rational> Univariate;
typedef Polynomial<Univariate> Bivariate;

Univariate operator % (Univariate a, Univariate b) {
  a.reduce();
  b.reduce();
  
  while(a.size() >= b.size()) {
    size_t shift = a.size() - b.size();
    Univariate shifted_b = b << shift;
    size_t leading_index = a.size() - 1;
    
    a = a - (a.getCoeff(leading_index) / shifted_b.getCoeff(leading_index)) * shifted_b;
    
    a.reduce();
  }
  
  return a;
}

Univariate operator / (Univariate a, Univariate b) {
  Univariate quotient;
  a.reduce();
  b.reduce();
  
  while(a.size() >= b.size()) {
    size_t shift = a.size() - b.size();
    Univariate shifted_b = b << shift;
    size_t leading_index = a.size() - 1;
    
    quotient.setCoeff(shift, (a.getCoeff(leading_index) / shifted_b.getCoeff(leading_index)));
    a = a - (a.getCoeff(leading_index) / shifted_b.getCoeff(leading_index)) * shifted_b;
    
    a.reduce();
  }
  
  quotient.reduce();
  return quotient;
}

template<typename T>
Polynomial<T> gcd(Polynomial<T> a, Polynomial<T> b);

template<typename T>
Polynomial<T> univariate_gcd(Polynomial<T> a, Polynomial<T> b) {
  a = toMonic(a);
  b = toMonic(b);
  if(a.size() > b.size()) {
    return univariate_gcd(b, a);
  }
  
  if(a.size() == 0) {
    T commonFactor = b.getCoeff(b.size() - 1);
    for(int iCoeff = 0;iCoeff < (int)b.size();iCoeff++) {
      commonFactor = gcd(commonFactor, b.getCoeff(iCoeff));
    }
    
    return toMonic(b / commonFactor);
  }
  T commonFactor = gcd(a.getCoeff(a.size() - 1), b.getCoeff(b.size() - 1));
  
  T factor = b.getCoeff(b.size() - 1) / commonFactor;
  b = (a.getCoeff(a.size() - 1) / commonFactor) * b;
  return univariate_gcd(b - ((factor * a) << (b.size() - a.size())), a);
}

template<typename T>
Polynomial<T> gcd(Polynomial<T> a, Polynomial<T> b) {
  T auxiliary = a.getCoeff(a.size() - 1);
  
  for(int iCoeff = 0;iCoeff < (int)a.size();iCoeff++) {
    auxiliary = gcd(auxiliary, a.getCoeff(iCoeff));
  }
  
  for(int iCoeff = 0;iCoeff < (int)b.size();iCoeff++) {
    auxiliary = gcd(auxiliary, b.getCoeff(iCoeff));
  }
  
  return auxiliary * univariate_gcd(a, b);
}

template<typename T>
Polynomial<T> normalFactor(const Polynomial<T>& a, const Polynomial<T>& b) {
   return gcd(a, b);
}

