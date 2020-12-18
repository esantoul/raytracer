#pragma once
#include "ndarray.hpp"

template <typename Scalar_t, std::size_t lines>
class Vector
    : public NdArray<Scalar_t, lines>
{
public:
  using NdArray<Scalar_t, lines>::NdArray;

  constexpr Vector operator*=(const Vector &other) = delete;
  constexpr Vector operator/=(const Vector &other) = delete;
  constexpr Vector operator*(const Vector &other) const = delete;
  constexpr Vector operator/(const Vector &other) const = delete;

  constexpr Scalar_t dot(const Vector &other) const
  {
    Scalar_t ret{};
    auto start = this->begin();
    for (const auto &oVal : other)
      ret += *(start++) * oVal;
    return ret;
  }
};
