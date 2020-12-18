#pragma once

#include <cstddef>

template <typename Scalar_t, std::size_t lines>
class Vector;

template <typename Scalar_t, std::size_t lines, std::size_t columns>
class Matrix;

template <typename Scalar_t, std::size_t... dims>
class NdArray_Cast_Helper
{
};

template <typename Scalar_t, std::size_t lines>
class NdArray_Cast_Helper<Scalar_t, lines>
{
public:
  constexpr operator Vector<Scalar_t, lines> &() { return static_cast<Vector<Scalar_t, lines> &>(*this); };
};

template <typename Scalar_t, std::size_t lines, std::size_t columns>
class NdArray_Cast_Helper<Scalar_t, lines, columns>
{
public:
  constexpr operator Matrix<Scalar_t, lines, columns> &() { return static_cast<Matrix<Scalar_t, lines, columns> &>(*this); };
};
