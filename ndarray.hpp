#pragma once

#include <cstddef>
#include <utility>

#include "ndarray_arithmetics.hpp"
#include "ndarray_types_helper.hpp"
#include "ndarray_cast_helper.hpp"
#include "utils.hpp"

template <typename Scalar_t, std::size_t... dims>
class NdArray
    : public NdArray_Arithmetics<NdArray<Scalar_t, dims...>, Scalar_t>,
      public NdArray_Cast_Helper<Scalar_t, dims...>
{
public:
  using Raw_t = typename NdArray_Types_Helper<Scalar_t, dims...>::Raw_t;
  using Data_t = typename NdArray_Types_Helper<Scalar_t, dims...>::Data_t;
  using Subscript_t = typename NdArray_Types_Helper<Scalar_t, dims...>::Subscript_t;

  constexpr NdArray() : mData{} {}
  constexpr NdArray(const Raw_t &data)
      : NdArray(data, std::make_index_sequence<utils::get_first_v<dims...>>())
  {
  }

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
  template <std::size_t... rng>
  constexpr NdArray(const Raw_t &data,
                    const std::index_sequence<rng...> &)
      : mData{data[rng]...}
  {
  }

  Data_t mData;
};
