#pragma once

template <typename Derived_t, typename Scalar_t>
class NdArray_Arithmetics
{
public:
  // Single value compound assignment add
  constexpr Derived_t &operator+=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l += r; });
  }
  // Single value compound assignment subtract
  constexpr Derived_t &operator-=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l -= r; });
  }
  // Single value compound assignment multiply
  constexpr Derived_t &operator*=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l *= r; });
  }
  // Single value compound assignment divide
  constexpr Derived_t &operator/=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l /= r; });
  }

  // Single value add
  constexpr Derived_t operator+(const Scalar_t &val) { return Derived_t{static_cast<const Derived_t &>(*this)} += val; }
  // Single value subtract
  constexpr Derived_t operator-(const Scalar_t &val) { return Derived_t{static_cast<const Derived_t &>(*this)} -= val; }
  // Single value multiply
  constexpr Derived_t operator*(const Scalar_t &val) { return Derived_t{static_cast<const Derived_t &>(*this)} *= val; }
  // Single value divide
  constexpr Derived_t operator/(const Scalar_t &val) { return Derived_t{static_cast<const Derived_t &>(*this)} /= val; }

  // Elementwise compound assignment add
  constexpr Derived_t &operator+=(const Derived_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l += r; });
  }

  // Elementwise compound assignment subtract
  constexpr Derived_t &operator-=(const Derived_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l -= r; });
  }

  // Elementwise compound assignment multiply
  constexpr Derived_t &operator*=(const Derived_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l *= r; });
  }

  // Elementwise compound assignment divide
  constexpr Derived_t &operator/=(const Derived_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l /= r; });
  }

  // Elementwise add
  constexpr Derived_t operator+(const Derived_t &other) { return Derived_t{static_cast<const Derived_t &>(*this)} += other; }
  // Elementwise subtract
  constexpr Derived_t operator-(const Derived_t &other) { return Derived_t{static_cast<const Derived_t &>(*this)} -= other; }
  // Elementwise multiply
  constexpr Derived_t operator*(const Derived_t &other) { return Derived_t{static_cast<const Derived_t &>(*this)} *= other; }
  // Elementwise divide
  constexpr Derived_t operator/(const Derived_t &other) { return Derived_t{static_cast<const Derived_t &>(*this)} /= other; }

private:
  template <typename Fun_t>
  constexpr Derived_t &cmpd_assign(const Scalar_t &val, Fun_t &&op)
  {
    Derived_t &reference = static_cast<Derived_t &>(*this);
    for (auto &el : reference)
      op(el, val);
    return reference;
  }

  template <typename Fun_t>
  constexpr Derived_t &cmpd_assign_elwise(const Derived_t &other, Fun_t &&op)
  {
    Derived_t &reference = static_cast<Derived_t &>(*this);
    auto start = reference.begin();
    for (const auto &el : other)
      op(*(start++), el);
    return reference;
  }
};
