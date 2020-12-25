#pragma once

namespace Constor
{
  template <typename Derived_t, typename Scalar_t, typename Subscript_t>
  class NdArray_Arithmetics
  {
  public:
    // Single value compound assignment add
    constexpr Derived_t &operator+=(const Scalar_t &val)
    {
      return cmpd_assign(val, plus_eq_val);
    }
    // Single value compound assignment subtract
    constexpr Derived_t &operator-=(const Scalar_t &val)
    {
      return cmpd_assign(val, minus_eq_val);
    }
    // Single value compound assignment multiply
    constexpr Derived_t &operator*=(const Scalar_t &val)
    {
      return cmpd_assign(val, mult_eq_val);
    }
    // Single value compound assignment divide
    constexpr Derived_t &operator/=(const Scalar_t &val)
    {
      return cmpd_assign(val, div_eq_val);
    }

    // Single value add
    constexpr Derived_t operator+(const Scalar_t &val) const { return Derived_t{static_cast<const Derived_t &>(*this)} += val; }
    // Single value subtract
    constexpr Derived_t operator-(const Scalar_t &val) const { return Derived_t{static_cast<const Derived_t &>(*this)} -= val; }
    // Single value multiply
    constexpr Derived_t operator*(const Scalar_t &val) const { return Derived_t{static_cast<const Derived_t &>(*this)} *= val; }
    // Single value divide
    constexpr Derived_t operator/(const Scalar_t &val) const { return Derived_t{static_cast<const Derived_t &>(*this)} /= val; }

    // Elementwise compound assignment add
    constexpr Derived_t &operator+=(const Derived_t &other)
    {
      return cmpd_assign_elwise(other, plus_eq_other);
    }

    // Elementwise compound assignment subtract
    constexpr Derived_t &operator-=(const Derived_t &other)
    {
      return cmpd_assign_elwise(other, minus_eq_other);
    }

    // Elementwise compound assignment multiply
    constexpr Derived_t &operator*=(const Derived_t &other)
    {
      return cmpd_assign_elwise(other, mult_eq_other);
    }

    // Elementwise compound assignment divide
    constexpr Derived_t &operator/=(const Derived_t &other)
    {
      return cmpd_assign_elwise(other, div_eq_other);
    }

    // Elementwise add
    constexpr Derived_t operator+(const Derived_t &other) const { return Derived_t{static_cast<const Derived_t &>(*this)} += other; }
    // Elementwise subtract
    constexpr Derived_t operator-(const Derived_t &other) const { return Derived_t{static_cast<const Derived_t &>(*this)} -= other; }
    // Elementwise multiply
    constexpr Derived_t operator*(const Derived_t &other) const { return Derived_t{static_cast<const Derived_t &>(*this)} *= other; }
    // Elementwise divide
    constexpr Derived_t operator/(const Derived_t &other) const { return Derived_t{static_cast<const Derived_t &>(*this)} /= other; }

    // Invertion operator
    constexpr Derived_t operator-() const { return static_cast<const Derived_t &>(*this) * -1; }

  private:
    static constexpr void plus_eq_val(Subscript_t &lhs, const Scalar_t &val) { lhs += val; }
    static constexpr void minus_eq_val(Subscript_t &lhs, const Scalar_t &val) { lhs -= val; }
    static constexpr void mult_eq_val(Subscript_t &lhs, const Scalar_t &val) { lhs *= val; }
    static constexpr void div_eq_val(Subscript_t &lhs, const Scalar_t &val) { lhs /= val; }

    static constexpr void plus_eq_other(Subscript_t &lhs, const Subscript_t &val) { lhs += val; }
    static constexpr void minus_eq_other(Subscript_t &lhs, const Subscript_t &val) { lhs -= val; }
    static constexpr void mult_eq_other(Subscript_t &lhs, const Subscript_t &val) { lhs *= val; }
    static constexpr void div_eq_other(Subscript_t &lhs, const Subscript_t &val) { lhs /= val; }

    constexpr Derived_t &cmpd_assign(const Scalar_t &val, void (&op)(Subscript_t &, const Scalar_t &))
    {
      Derived_t &reference = static_cast<Derived_t &>(*this);
      for (Subscript_t &el : reference)
        op(el, val);
      return reference;
    }

    constexpr Derived_t &cmpd_assign_elwise(const Derived_t &other, void (&op)(Subscript_t &, const Subscript_t &))
    {
      Derived_t &reference = static_cast<Derived_t &>(*this);
      Subscript_t *start = reference.begin();
      for (const Subscript_t &el : other)
        op(*(start++), el);
      return reference;
    }
  };
} // namespace Constor
