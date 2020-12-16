#pragma once

template <typename Source_t, typename Scalar_t>
class Base_Arithmetics_Helper
{
public:
  Source_t &operator+=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &&l, auto &&r) { l += r; });
  }
  Source_t &operator-=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &&l, auto &&r) { l -= r; });
  }
  Source_t &operator*=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &&l, auto &&r) { l *= r; });
  }
  Source_t &operator/=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &&l, auto &&r) { l /= r; });
  }

  Source_t &operator+=(const Source_t &other)
  {
    Source_t &reference = static_cast<Source_t &>(*this);
    auto start = reference.begin();
    for (const auto &el : other)
      *(start++) += el;
    return reference;
  }

  Source_t &operator-=(const Source_t &other)
  {
    Source_t &reference = static_cast<Source_t &>(*this);
    auto start = reference.begin();
    for (const auto &el : other)
      *(start++) -= el;
    return reference;
  }

  Source_t operator+(const Source_t &other)
  {
    Source_t ret{static_cast<const Source_t &>(*this)};
    return ret += other;
  }

  Source_t operator-(const Source_t &other)
  {
    Source_t ret{static_cast<const Source_t &>(*this)};
    return ret -= other;
  }

  // TODO Implement hadamard_multiply
  // TODO Implement hadamard_divide

private:
  template <typename Fun_t>
  Source_t &cmpd_assign(const Scalar_t &val, Fun_t &&fun)
  {
    Source_t &reference = static_cast<Source_t &>(*this);
    for (auto &&el : reference)
      fun(el, val);
    return reference;
  }
};
