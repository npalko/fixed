//                     Copyright Nicholas Palko 2012
//      Distributed under the Boost Software License, Version 1.0.
//          (See accompanying file LICENSE_1_0.txt or copy at
//                 http://www.boost.org/LICENSE_1_0.txt)

#ifndef FIXED_FIXED_HPP
#define FIXED_FIXED_HPP

#include <boost/numeric/conversion/converter.hpp>
#include <boost/operators.hpp>
#include <cstdint>
#include <cstdio>
#include <iosfwd>
#include <iomanip>


namespace fixed {

namespace {
// compile-time integer powers of 10
template <unsigned exponent>
struct p10 {
  enum { n = 10 * p10<exponent-1>::n };
};
template <>
struct p10<0> {
  enum { n = 1 };
};
}


/** Fixed point POD class. 
 */

template <typename T, unsigned N>
class Fixed : boost::operators<Fixed<T,N>> {
 public:
  Fixed() = default;
  explicit Fixed(T x) : value_(x) { };
  explicit Fixed(double x) : value_(Double2T::convert(x * p10<N>::n)) { };
  explicit Fixed(float x) : value_(Float2T::convert(x * p10<N>::n)) { };
 public:
  explicit operator double() const {
    return static_cast<double> (value_) / p10<N>::n;
  };
  explicit operator float() const {
    return static_cast<float> (value_) / p10<N>::n;
  };
 public: // functions required by boost::operators
  bool operator<(const Fixed& rhs) const {
    return value_ < rhs.value_;
  };
  bool operator==(const Fixed& rhs) const {
    return value_ == rhs.value_;
  };
  Fixed& operator+=(const Fixed& rhs) {
    value_ += rhs.value_;
    return *this;
  };
  Fixed& operator-=(const Fixed& rhs) {
    value_ -= rhs.value_;
    return *this;
  };
  Fixed& operator*=(const Fixed& rhs) {
    value_ = (value_ * rhs.value_) / p10<N>::n;
    return *this;
  };
  Fixed& operator/=(const Fixed& rhs) {
    value_ = (value_ * p10<N>::n) / rhs.value_;
    return *this;  
  };
  Fixed& operator++() {
    value_ += mult_;
    return *this;
  };
  Fixed& operator--() {
    value_ -= mult_;
    return *this;
  };
 private:
  typedef boost::numeric::converter<double,T> Double2T;
  typedef boost::numeric::converter<float,T> Float2T;
  T value_;
};



// define operators w/ integers

template <typename T, unsigned N>
int str (char* s, size_t n, const Fixed<T,N>& f) {
  return std::snprintf(s, n, '%f', static_cast<double>(f));
};

template <typename T, unsigned N>
std::ostream& operator<<(std::ostream& os, const Fixed<T,N>& f) {
  //std::ios_base::flags flags

  os << std::setprecision(N) << std::fixed << static_cast<double>(f);
  return os;
};


typedef Fixed<std::int32_t,2> fix2_32;
typedef Fixed<std::int32_t,4> fix4_32;
typedef Fixed<std::int32_t,6> fix6_32;
typedef Fixed<std::int32_t,8> fix8_32;
}

#endif

  
