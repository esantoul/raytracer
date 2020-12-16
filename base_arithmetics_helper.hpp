#pragma once

template <typename Source_t, typename Scalar_t>
class Base_Arithmetics_Helper
{
public:
  // Single value compound assignment add
  constexpr Source_t &operator+=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l += r; });
  }
  // Single value compound assignment subtract
  constexpr Source_t &operator-=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l -= r; });
  }
  // Single value compound assignment multiply
  constexpr Source_t &operator*=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l *= r; });
  }
  // Single value compound assignment divide
  constexpr Source_t &operator/=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l /= r; });
  }

  // Single value add
  constexpr Source_t operator+(const Scalar_t &val) { return Source_t{static_cast<const Source_t &>(*this)} += val; }
  // Single value subtract
  constexpr Source_t operator-(const Scalar_t &val) { return Source_t{static_cast<const Source_t &>(*this)} -= val; }
  // Single value multiply
  constexpr Source_t operator*(const Scalar_t &val) { return Source_t{static_cast<const Source_t &>(*this)} *= val; }
  // Single value divide
  constexpr Source_t operator/(const Scalar_t &val) { return Source_t{static_cast<const Source_t &>(*this)} /= val; }

  // Elementwise compound assignment add
  constexpr Source_t &operator+=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l += r; });
  }

  // Elementwise compound assignment subtract
  constexpr Source_t &operator-=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l -= r; });
  }

  // Elementwise compound assignment multiply
  constexpr Source_t &operator*=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l *= r; });
  }

  // Elementwise compound assignment divide
  constexpr Source_t &operator/=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l /= r; });
  }

  // Elementwise add
  constexpr Source_t operator+(const Source_t &other) { return Source_t{static_cast<const Source_t &>(*this)} += other; }
  // Elementwise subtract
  constexpr Source_t operator-(const Source_t &other) { return Source_t{static_cast<const Source_t &>(*this)} -= other; }
  // Elementwise multiply
  constexpr Source_t operator*(const Source_t &other) { return Source_t{static_cast<const Source_t &>(*this)} *= other; }
  // Elementwise divide
  constexpr Source_t operator/(const Source_t &other) { return Source_t{static_cast<const Source_t &>(*this)} /= other; }

private:
  template <typename Fun_t>
  constexpr Source_t &cmpd_assign(const Scalar_t &val, Fun_t &&op)
  {
    Source_t &reference = static_cast<Source_t &>(*this);
    for (auto &el : reference)
      op(el, val);
    return reference;
  }

  template <typename Fun_t>
  constexpr Source_t &cmpd_assign_elwise(const Source_t &other, Fun_t &&op)
  {
    Source_t &reference = static_cast<Source_t &>(*this);
    auto start = reference.begin();
    for (const auto &el : other)
      op(*(start++), el);
    return reference;
  }
};
