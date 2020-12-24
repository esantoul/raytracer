#pragma once
#include "ndarray.hpp"
#include "vector.hpp"

namespace Constor
{
  template <typename Scalar_t, std::size_t lines, std::size_t columns>
  class Matrix
      : public NdArray<Scalar_t, lines, columns>
  {
  public:
    using NdArray<Scalar_t, lines, columns>::NdArray;

    constexpr Matrix() : NdArray<Scalar_t, lines, columns>{} {}
    constexpr Matrix(const NdArray<Scalar_t, lines, columns> &nda) : NdArray<Scalar_t, lines, columns>{nda} {}

    constexpr Vector<Scalar_t, columns> &operator[](std::size_t idx) { return static_cast<Vector<Scalar_t, columns> &>(NdArray<Scalar_t, lines, columns>::operator[](idx)); }
    constexpr const Vector<Scalar_t, columns> &operator[](std::size_t idx) const { return static_cast<const Vector<Scalar_t, columns> &>(NdArray<Scalar_t, lines, columns>::operator[](idx)); }

    constexpr Matrix operator*=(const Matrix &other) = delete;
    constexpr Matrix operator/=(const Matrix &other) = delete;
    constexpr Matrix operator/(const Matrix &other) const = delete;

    template <std::size_t other_ncolumns>
    constexpr Matrix<Scalar_t, lines, other_ncolumns> operator*(const Matrix<Scalar_t, columns, other_ncolumns> &other) const
    {
      Matrix<Scalar_t, lines, other_ncolumns> ret;
      NdArray<Scalar_t, lines, other_ncolumns> &ret_ref = ret;
      const NdArray<Scalar_t, lines, columns> &this_ref{*this};
      const NdArray<Scalar_t, columns, other_ncolumns> &other_ref{other};
      for (std::size_t i = 0; i < lines; ++i)
      {
        for (std::size_t j = 0; j < other_ncolumns; ++j)
        {
          for (std::size_t k = 0; k < columns; ++k)
            ret_ref[i][j] += this_ref[i][k] * other_ref[k][j];
        }
      }
      return ret;
    }

    template <typename Other_Scalar_t>
    constexpr operator Matrix<Other_Scalar_t, lines, columns>()
    {
      return NdArray<Other_Scalar_t, lines, columns>{*this};
    }

    constexpr Matrix<Scalar_t, columns, lines> transpose() const
    {
      Matrix<Scalar_t, columns, lines> ret;
      NdArray<Scalar_t, columns, lines> &ret_ref = ret;
      const NdArray<Scalar_t, lines, columns> &this_ref = *this;
      for (std::size_t i = 0; i < lines; ++i)
        for (std::size_t j = 0; j < columns; ++j)
          ret_ref[j][i] = this_ref[i][j];
      return ret;
    }
  };
} // namespace Constor
