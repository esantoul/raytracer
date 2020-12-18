#pragma once

#include <cstddef>
#include <array>

template <typename Scalar_t, std::size_t... dims>
class NdArray;

template <typename Scalar_t, std::size_t lines>
class Vector;

template <typename Scalar_t, std::size_t lines, std::size_t columns>
class Matrix;

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class NdArray_Types_Helper
{
public:
  using Raw_t = typename NdArray_Types_Helper<Scalar_t, dims...>::Raw_t[dim0];
  using Subscript_t = NdArray<Scalar_t, dims...>;
  using Data_t = Subscript_t[dim0];
};

template <typename Scalar_t, std::size_t dim0>
class NdArray_Types_Helper<Scalar_t, dim0>
{
public:
  using Raw_t = Scalar_t[dim0];
  using Subscript_t = Scalar_t;
  using Data_t = Subscript_t[dim0];
};
