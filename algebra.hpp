#pragma once

#include <cmath>
#include <numeric>
#include <cassert>
#include <algorithm>
#include <optional>

template <std::size_t dim, typename Scalar_t>
class Vec
{
public:
  template <typename... Args>
  constexpr Vec(Args... components) : mComp{static_cast<Scalar_t>(components)...} {}

  constexpr const Scalar_t &operator[](std::size_t index) const
  {
    assert("Out of bound access" && index < dim);
    return mComp[index];
  }

  constexpr Vec operator+(const Vec &other) const
  {
    Vec res{*this};
    for (std::size_t i = 0; i < dim; ++i)
      res.mComp[i] += other.mComp[i];
    return res;
  }

  constexpr Vec operator+=(const Vec &other)
  {
    for (std::size_t i = 0; i < dim; ++i)
      mComp[i] += other.mComp[i];
    return *this;
  }

  constexpr Vec operator-(const Vec &other) const
  {
    Vec res{*this};
    for (std::size_t i = 0; i < dim; ++i)
      res.mComp[i] -= other.mComp[i];
    return res;
  }

  constexpr Vec operator*(const Scalar_t &factor) const
  {
    Vec res{*this};
    for (auto &el : res.mComp)
      el *= factor;
    return res;
  }

  constexpr Vec operator/(const Scalar_t &factor) const
  {
    Vec res{*this};
    for (auto &el : res.mComp)
      el /= factor;
    return res;
  }

  constexpr Vec operator/=(const Scalar_t &factor)
  {
    for (auto &el : mComp)
      el /= factor;
    return *this;
  }

  constexpr Vec hadamard(const Vec &other) const
  {
    Vec res{*this};
    for (std::size_t i = 0; i < dim; ++i)
      res.mComp[i] *= other.mComp[i];
    return res;
  }

  constexpr Scalar_t *begin() { return mComp; }
  constexpr Scalar_t *end() { return mComp + dim; }

  constexpr Scalar_t norm() const
  {
    return norm<2>();
  }

  constexpr Scalar_t norm_inf() const
  {
    return *std::max_element(mComp, mComp + dim, [](Scalar_t lhs, Scalar_t rhs) { return std::abs(lhs) < std::abs(rhs); });
  }

  constexpr Vec &normalize()
  {
    *this /= this->norm();
    return *this;
  }

  constexpr Scalar_t dot(const Vec &other) const
  {
    Scalar_t s{0};
    for (std::size_t i = 0; i < dim; ++i)
      s += mComp[i] * other.mComp[i];
    return s;
  }

private:
  template <std::size_t n>
  constexpr Scalar_t norm() const
  {
    return std::pow(std::accumulate(mComp, mComp + dim, 0.0, [](Scalar_t ps, Scalar_t el) { return ps + std::pow(std::abs(el), n); }), 1 / static_cast<double>(n));
  }

  Scalar_t mComp[dim]{};
};

template <std::size_t dim, typename Scalar_t, typename Factor_t>
constexpr Vec<dim, Scalar_t> operator*(const Factor_t &f, const Vec<dim, Scalar_t> &vec)
{
  return vec * f;
}

using Vec3f = Vec<3, float>;

// template <typename Scalar_t, std::size_t... dims>
// class Tensor
// {
// public:
//   Tensor<Scalar_t,
// private:
//   Scalar_t mComp[(dims * ...)]{};
// };