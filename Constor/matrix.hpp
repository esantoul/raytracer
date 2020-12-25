#pragma once
#include "ndarray.hpp"
#include "vector.hpp"

namespace Constor
{
  template <typename Scalar_t, std::size_t lines, std::size_t columns>
  class Matrix
      : public NdArray_Arithmetics<Matrix<Scalar_t, lines, columns>, Scalar_t, Vector<Scalar_t, columns>>
  {
  public:
    using Subscript_t = Vector<Scalar_t, columns>;

    constexpr Matrix() : mData{} {}
    constexpr Matrix(const NdArray<Scalar_t, lines, columns> &nda) : Matrix{nda, std::make_index_sequence<lines>()} {}

    constexpr const auto begin() const { return std::begin(mData); }
    constexpr const auto end() const { return std::end(mData); }
    constexpr auto begin() { return mData; }
    constexpr auto end() { return mData + lines; }

    constexpr Subscript_t &operator[](std::size_t idx) { return mData[idx]; }
    constexpr const Subscript_t &operator[](std::size_t idx) const { return mData[idx]; }

    constexpr Matrix &operator*=(const Scalar_t &val) { return NdArray_Arithmetics<Matrix<Scalar_t, lines, columns>, Scalar_t, Subscript_t>::operator*=(val); }
    constexpr Matrix operator*(const Scalar_t &val) const { return NdArray_Arithmetics<Matrix<Scalar_t, lines, columns>, Scalar_t, Subscript_t>::operator*(val); }

    constexpr Matrix &operator/=(const Scalar_t &val) { return NdArray_Arithmetics<Matrix<Scalar_t, lines, columns>, Scalar_t, Subscript_t>::operator/=(val); }
    constexpr Matrix operator/(const Scalar_t &val) const { return NdArray_Arithmetics<Matrix<Scalar_t, lines, columns>, Scalar_t, Subscript_t>::operator/(val); };

    template <std::size_t other_ncolumns>
    constexpr Matrix<Scalar_t, lines, other_ncolumns> operator*(const Matrix<Scalar_t, columns, other_ncolumns> &other) const
    {
      Matrix<Scalar_t, lines, other_ncolumns> ret;
      for (std::size_t i = 0; i < lines; ++i)
      {
        for (std::size_t j = 0; j < other_ncolumns; ++j)
        {
          for (std::size_t k = 0; k < columns; ++k)
            ret[i][j] += (*this)[i][k] * other[k][j];
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
      for (std::size_t i = 0; i < lines; ++i)
        for (std::size_t j = 0; j < columns; ++j)
          ret[j][i] = (*this)[i][j];
      return ret;
    }

    constexpr NdArray<Scalar_t, lines, columns> to_ndarray() const { return to_ndarray_helper(std::make_index_sequence<lines>()); }

    constexpr operator NdArray<Scalar_t, lines, columns>() const { return {mData}; }

  private:
    template <std::size_t... rng>
    constexpr NdArray<Scalar_t, lines, columns> to_ndarray_helper(std::index_sequence<rng...> &&) const
    {
      return {static_cast<const NdArray<Scalar_t, columns> &>(mData[rng])...};
    }

    template <std::size_t... rng>
    constexpr Matrix(const NdArray<Scalar_t, lines, columns> &nda, std::index_sequence<rng...> &&)
        : mData{nda[rng]...}
    {
    }

    Subscript_t mData[lines];
  };
} // namespace Constor

template <typename Scalar_t, typename Value_t, std::size_t lines, std::size_t columns>
constexpr Constor::Matrix<Scalar_t, lines, columns> operator+(const Value_t &val, const Constor::Matrix<Scalar_t, lines, columns> &mat)
{
  return mat + val;
}
template <typename Scalar_t, typename Value_t, std::size_t lines, std::size_t columns>
constexpr Constor::Matrix<Scalar_t, lines, columns> operator-(const Value_t &val, const Constor::Matrix<Scalar_t, lines, columns> &mat)
{
  return -mat + val;
}
template <typename Scalar_t, typename Value_t, std::size_t lines, std::size_t columns>
constexpr Constor::Matrix<Scalar_t, lines, columns> operator*(const Value_t &val, const Constor::Matrix<Scalar_t, lines, columns> &mat)
{
  return mat * val;
}
