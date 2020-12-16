#pragma once

template <typename Source_t, typename Scalar_t>
class Base_Arithmetics_Helper
{
public:
  Source_t &operator+=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l += r; });
  }
  Source_t &operator-=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l -= r; });
  }
  Source_t &operator*=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l *= r; });
  }
  Source_t &operator/=(const Scalar_t &val)
  {
    return cmpd_assign(val, [](auto &l, const auto &r) { l /= r; });
  }

  Source_t &operator+=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l += r; });
  }

  Source_t &operator-=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l -= r; });
  }

  Source_t &operator*=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l *= r; });
  }

  Source_t &operator/=(const Source_t &other)
  {
    return cmpd_assign_elwise(other, [](auto &l, const auto &r) { l /= r; });
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

  Source_t operator*(const Source_t &other)
  {
    Source_t ret{static_cast<const Source_t &>(*this)};
    return ret *= other;
  }

  Source_t operator/(const Source_t &other)
  {
    Source_t ret{static_cast<const Source_t &>(*this)};
    return ret /= other;
  }

private:
  template <typename Fun_t>
  Source_t &cmpd_assign(const Scalar_t &val, Fun_t &&op)
  {
    Source_t &reference = static_cast<Source_t &>(*this);
    for (auto &el : reference)
      op(el, val);
    return reference;
  }

  template <typename Fun_t>
  Source_t &cmpd_assign_elwise(const Source_t &other, Fun_t &&op)
  {
    Source_t &reference = static_cast<Source_t &>(*this);
    auto start = reference.begin();
    for (const auto &el : other)
      op(*(start++), el);
    return reference;
  }
};
