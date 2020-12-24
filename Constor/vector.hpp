#pragma once
#include "ndarray.hpp"
#include "matrix.hpp"

namespace Constor
{
  template <typename Scalar_t, std::size_t lines>
  class Vector
      : public NdArray<Scalar_t, lines>
  {
  public:
    using NdArray<Scalar_t, lines>::NdArray;

    constexpr Vector(const NdArray<Scalar_t, lines> &nda) : NdArray<Scalar_t, lines>{nda} {}

    constexpr Vector operator*=(const Vector &other) = delete;
    constexpr Vector operator/=(const Vector &other) = delete;
    constexpr Vector operator/(const Vector &other) const = delete;

    template <std::size_t other_ncolumns>
    constexpr Matrix<Scalar_t, 1, other_ncolumns> operator*(const Matrix<Scalar_t, lines, other_ncolumns> &other) const
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

    constexpr const NdArray<Scalar_t, lines> &to_ndarray() const { return *this; }

    constexpr Matrix<Scalar_t, 1, lines> to_matrix() const { return {static_cast<const NdArray<Scalar_t, lines> &>(*this)}; }

    constexpr Matrix<Scalar_t, lines, 1> transpose() const { return this->to_matrix().transpose(); }

    constexpr operator Matrix<Scalar_t, lines, 1>() const { return this->to_matrix(); }
  };
} // namespace Constor
