//                     Copyright Nicholas Palko 2012
//      Distributed under the Boost Software License, Version 1.0.
//          (See accompanying file LICENSE_1_0.txt or copy at
//                 http://www.boost.org/LICENSE_1_0.txt)

#ifndef FIXED_FIXED_HPP
#define FIXED_FIXED_HPP

#include <boost/cstdint.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/operators.hpp>
#include <iosfwd>
#include <iomanip>


namespace fixed {
namespace {

// cheap compile-time way to get integer powers of 10
template <unsigned exponent>
struct pow10 {
  enum { value = 10 * pow10<exponent-1>::value };
};
template <>
struct pow10<0> {
  enum { value = 1 };
};

} 
  

template <typename T, unsigned N>
class Fixed : boost::operators<Fixed<T,N>> {
 public:
  Fixed() : data_(0) { };
  Fixed(const Fixed& rhs) : data_(rhs.data_) { };
  Fixed(T x) : data_(x) { };
  Fixed(double x) : data_(DoubleToT::convert(x * multiplier_)) { };
  Fixed(float x) : data_(FloatToT::convert(x * multiplier_)) { };
  Fixed& operator=(const Fixed& rhs) {
    data_ = rhs.data_;
    return *this;
  };
  ~Fixed() { };
 public: // conversions
  explicit operator double() const {
    return static_cast<double>(data_) / multiplier_;
  };
  explicit operator float() const {
    return static_cast<float>(data_) / multiplier_;
  };
 public: // boost::operators
  bool operator<(const Fixed& rhs) const {
    return data_ < rhs.data_;
  };
  bool operator==(const Fixed& rhs) const {
    return data_ == rhs.data_;
  };
  Fixed& operator+=(const Fixed& rhs) {
    data_ += rhs.data_;
    return *this;
  };
  Fixed& operator-=(const Fixed& rhs) {
    data_ -= rhs.data_;
    return *this;
  };
  Fixed& operator*=(const Fixed& rhs) {
    data_ *= rhs.data_;
    return *this;
  };
  Fixed& operator/=(const Fixed& rhs) {
    data_ /= rhs.data_;
    return *this;  
  };
  Fixed& operator%=(const Fixed& rhs) {
    data_ %= rhs.data_;
    return *this;
  };
  Fixed& operator|=(const Fixed& rhs) {
    data_ |= rhs.data_;
    return *this;
  };
  Fixed& operator&=(const Fixed& rhs) {
    data_ &= rhs.data_;
    return *this;
  };
  Fixed& operator^=(const Fixed& rhs) {
    data_ ^= rhs.data_;
    return *this;  
  };
  Fixed& operator++() {
    data_ += multiplier_;
    return *this;
  };
  Fixed& operator--() {
    data_ -= multiplier_;
    return *this;
  };
 private:
  // TODO: make sure we convert to nearest int - Float2IntRounter::
  typedef boost::numeric::converter<double,T> DoubleToT;
  typedef boost::numeric::converter<float,T> FloatToT;
  static const int multiplier_ = pow10<N>::value;
  T data_;
};

template <typename T, unsigned N>
std::ostream& operator<<(std::ostream& os, const Fixed<T,N>& f) {
  os << std::setprecision(N) << std::fixed << static_cast<double>(f);
  return os;
};


typedef Fixed<boost::int32_t,2> Fixed2Int32;
typedef Fixed<boost::int32_t,4> Fixed4Int32;
typedef Fixed<boost::int32_t,6> Fixed6Int32;
typedef Fixed<boost::int32_t,8> Fixed8Int32;
}

#endif

  
