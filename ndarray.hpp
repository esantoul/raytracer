#pragma once

#include <cstddef>

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
struct ndarray
{
  using Decayed_t = typename ndarray<Scalar_t, dims...>::type;
  using type = Decayed_t[dim0];
  type mData{};
  constexpr Decayed_t &operator[](std::size_t idx) { return mData[idx]; }
};

template <typename Scalar_t, std::size_t dim0>
struct ndarray<Scalar_t, dim0>
{
  using type = Scalar_t[dim0];
  type mData{};
  constexpr Scalar_t &operator[](std::size_t idx) { return mData[idx]; }
};

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
struct ndarray_ref
{
  using Decayed_t = typename ndarray_ref<Scalar_t, dims...>::Data_t;
  using Data_t = Decayed_t[dim0];
  Data_t &mData;
  constexpr Decayed_t &operator[](std::size_t idx) { return mData[idx]; }
};

template <typename Scalar_t, std::size_t dim0>
struct ndarray_ref<Scalar_t, dim0>
{
  using Data_t = Scalar_t[dim0];
  Data_t &mData;
  constexpr Scalar_t &operator[](std::size_t idx) { return mData[idx]; }
};
