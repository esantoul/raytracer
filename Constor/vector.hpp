#pragma once
#include "ndarray.hpp"
#include "matrix.hpp"

namespace Constor
{
  template <typename Scalar_t, std::size_t length>
  class Vector
      : public NdArray<Scalar_t, length>,
        public NdArray_Arithmetics<Vector<Scalar_t, length>, Scalar_t, Scalar_t> // Overrides NdArray's
  {
  public:
    using NdArray<Scalar_t, length>::NdArray;
    using Base_t = NdArray<Scalar_t, length>;
    using Subscript_t = Scalar_t;
    using Arithmetics_Base = NdArray_Arithmetics<Vector<Scalar_t, length>, Scalar_t, Scalar_t>;

    constexpr Vector(const Base_t &nda) : Base_t{nda} {}

    using Arithmetics_Base::operator+=;
    using Arithmetics_Base::operator-=;
    using Arithmetics_Base::operator+;
    using Arithmetics_Base::operator-;

    constexpr Vector &operator*=(const Scalar_t &val) { return NdArray_Arithmetics<Vector<Scalar_t, length>, Scalar_t, Subscript_t>::operator*=(val); }
    constexpr Vector operator*(const Scalar_t &val) const { return NdArray_Arithmetics<Vector<Scalar_t, length>, Scalar_t, Subscript_t>::operator*(val); }

    constexpr Vector &operator/=(const Scalar_t &val) { return NdArray_Arithmetics<Vector<Scalar_t, length>, Scalar_t, Subscript_t>::operator/=(val); }
    constexpr Vector operator/(const Scalar_t &val) const { return NdArray_Arithmetics<Vector<Scalar_t, length>, Scalar_t, Subscript_t>::operator/(val); };

    template <std::size_t other_ncolumns>
    constexpr Matrix<Scalar_t, 1, other_ncolumns> operator*(const Matrix<Scalar_t, length, other_ncolumns> &other) const
    {
      return this->to_matrix() * other;
    }

    constexpr Scalar_t dot(const Vector &other) const
    {
      Scalar_t ret{};
      auto start = this->begin();
      for (const auto &oVal : other)
        ret += *(start++) * oVal;
      return ret;
    }

    constexpr const NdArray<Scalar_t, length> &to_ndarray() const { return *this; }

    constexpr Matrix<Scalar_t, 1, length> to_matrix() const { return {static_cast<const Base_t &>(*this)}; }

    constexpr Matrix<Scalar_t, length, 1> transpose() const { return this->to_matrix().transpose(); }

    constexpr operator Matrix<Scalar_t, length, 1>() const { return this->to_matrix(); }
  };
} // namespace Constor

template <typename Scalar_t, typename Value_t, std::size_t length>
constexpr Constor::Vector<Scalar_t, length> operator+(const Value_t &val, const Constor::Vector<Scalar_t, length> &vec)
{
  return vec + val;
}
template <typename Scalar_t, typename Value_t, std::size_t length>
constexpr Constor::Vector<Scalar_t, length> operator-(const Value_t &val, const Constor::Vector<Scalar_t, length> &vec)
{
  return -vec + val;
}
template <typename Scalar_t, typename Value_t, std::size_t length>
constexpr Constor::Vector<Scalar_t, length> operator*(const Value_t &val, const Constor::Vector<Scalar_t, length> &vec)
{
  return vec * val;
}
