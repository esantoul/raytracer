#pragma once

#include <cstddef>
#include <algorithm>
#include "base_arithmetics_helper.hpp"
#include "utils.hpp"

template <typename Scalar_t, std::size_t... dims>
class NdArray;
template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class nda_types_helper
{
public:
  using Raw_t = typename nda_types_helper<Scalar_t, dims...>::Raw_t[dim0];
  using Subscript_t = NdArray<Scalar_t, dims...>;
  using Data_t = Subscript_t[dim0];
};

template <typename Scalar_t, std::size_t dim0>
class nda_types_helper<Scalar_t, dim0>
{
public:
  using Raw_t = Scalar_t[dim0];
  using Subscript_t = Scalar_t;
  using Data_t = Subscript_t[dim0];
};

template <typename Scalar_t, std::size_t... dims>
class NdArray
    : public Base_Arithmetics_Helper<NdArray<Scalar_t, dims...>, Scalar_t>
{
public:
  using Raw_t = typename nda_types_helper<Scalar_t, dims...>::Raw_t;
  using Data_t = typename nda_types_helper<Scalar_t, dims...>::Data_t;
  using Subscript_t = typename nda_types_helper<Scalar_t, dims...>::Subscript_t;

  constexpr NdArray() : mData{} {}
  constexpr NdArray(const Raw_t &data) { std::copy(std::begin(data), std::end(data), mData); }
  constexpr Subscript_t &operator[](std::size_t idx) { return mData[idx]; }
  constexpr const Subscript_t &operator[](std::size_t idx) const { return mData[idx]; }

  template <typename Other_Scalar_t>
  constexpr operator NdArray<Other_Scalar_t, dims...>()
  {
    NdArray<Other_Scalar_t, dims...> ret{};
    auto start = begin();
    for (auto &el : ret)
      el = *(start++);
    return ret;
  }

  constexpr Subscript_t *begin() { return mData; }
  constexpr Subscript_t *end() { return mData + utils::get_first_v<dims...>; }
  constexpr const Subscript_t *begin() const { return mData; }
  constexpr const Subscript_t *end() const { return mData + utils::get_first_v<dims...>; }

  constexpr std::size_t dimentions() const { return sizeof...(dims); }

private:
  Data_t mData;
};
