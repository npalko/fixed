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
  explicit Fixed(T x) : data_(x) { };
  explicit Fixed(double x)
    : data_(boost::numeric::converter<double,T>::convert(x * mult_)) { };
  explicit Fixed(float x)
    : data_(boost::numeric::converter<float,T>::convert(x * mult_)) { };
 public: // conversions
  explicit operator double() const {
    return static_cast<double>(data_) / mult_;
  };
  explicit operator float() const {
    return static_cast<float>(data_) / mult_;
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
    data_ = (data_ * rhs.data_) / mult_;
    return *this;
  };
  Fixed& operator/=(const Fixed& rhs) {
    data_ = (data_ * mult_) / rhs.data_;
    return *this;  
  };
  Fixed& operator++() {
    data_ += mult_;
    return *this;
  };
  Fixed& operator--() {
    data_ -= mult_;
    return *this;
  };
 private:
  // TODO: make sure we convert to nearest int - Float2IntRounter::
  static const int mult_ = pow10<N>::value;
  T data_;
};

template <typename T, unsigned N>
std::ostream& operator<<(std::ostream& os, const Fixed<T,N>& f) {
  os << std::setprecision(N) << std::fixed << static_cast<double>(f);
  return os;
};


typedef Fixed<boost::int32_t,2> int32p2_t;
typedef Fixed<boost::int32_t,4> int32p4_t;
typedef Fixed<boost::int32_t,6> int32p6_t;
typedef Fixed<boost::int32_t,8> int32p8_t;
}

#endif

  
