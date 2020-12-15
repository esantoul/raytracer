#pragma once

#include <cstddef>

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class Tensor_ref;

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

template <typename Source_t, typename Stored_t, typename Dereferenced_t, std::size_t dim0>
class TensorIteratorHelper
{
  friend Source_t;

public:
  TensorIteratorHelper &operator++()
  {
    if (mIndex < dim0)
      ++mIndex;
    return *this;
  }
  bool operator==(const TensorIteratorHelper &other) const { return pParent == other.pParent && mIndex == other.mIndex; }
  bool operator!=(const TensorIteratorHelper &other) const { return !(*this == other); }
  Dereferenced_t operator*() { return (*pParent)[mIndex]; }

private:
  TensorIteratorHelper(Stored_t *source, std::size_t index) : pParent{source}, mIndex{index} {}

  Stored_t *pParent;
  std::size_t mIndex;
};

template <typename Source_t, typename Scalar_t, std::size_t dim0, std::size_t... dims>
class TensorBase
    : public Base_Arithmetics_Helper<Source_t, Scalar_t>
{
public:
  using Decayed_t = Tensor_ref<Scalar_t, dims...>;
  using Data_t = typename Decayed_t::Data_t[dim0];
  using Subscript_t = Decayed_t;
  using Iterator = TensorIteratorHelper<Source_t, Source_t, Decayed_t, dim0>;
  using Const_Iterator = TensorIteratorHelper<Source_t, const Source_t, Decayed_t, dim0>;
};

template <typename Source_t, typename Scalar_t, std::size_t dim0>
class TensorBase<Source_t, Scalar_t, dim0>
    : public Base_Arithmetics_Helper<Source_t, Scalar_t>
{
public:
  using Decayed_t = Scalar_t;
  using Data_t = Decayed_t[dim0];
  using Subscript_t = Decayed_t &;
  using Iterator = TensorIteratorHelper<Source_t, Source_t, Decayed_t &, dim0>;
  using Const_Iterator = TensorIteratorHelper<Source_t, const Source_t, Decayed_t &, dim0>;
};

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class Tensor : public TensorBase<Tensor<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>
{
public:
  using Base_t = TensorBase<Tensor<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>;
  using Decayed_t = typename Base_t::Decayed_t;
  using Data_t = typename Base_t::Data_t;
  using Subscript_t = typename Base_t::Subscript_t;
  using Iterator = typename Base_t::Iterator;
  using Const_Iterator = typename Base_t::Const_Iterator;

  Subscript_t operator[](std::size_t idx) { return mStorage[idx]; }
  const Subscript_t operator[](std::size_t idx) const { return mStorage[idx]; }
  operator Tensor_ref<Scalar_t, dim0, dims...>() { return {mStorage}; }
  operator Tensor_ref<const Scalar_t, dim0, dims...>() const { return {mStorage}; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }
  Const_Iterator begin() const { return {this, 0}; }
  Const_Iterator end() const { return {this, dim0}; }

private:
  Data_t mStorage{};
};

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class Tensor_ref : public TensorBase<Tensor_ref<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>
{
public:
  using Base_t = TensorBase<Tensor_ref<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>;
  using Decayed_t = typename Base_t::Decayed_t;
  using Data_t = typename Base_t::Data_t;
  using Subscript_t = typename Base_t::Subscript_t;
  using Iterator = typename Base_t::Iterator;
  using Const_Iterator = typename Base_t::Const_Iterator;

  Tensor_ref(Data_t &data) : mData{data} {}
  Subscript_t operator[](std::size_t idx) { return mData[idx]; }
  const Subscript_t operator[](std::size_t idx) const { return mData[idx]; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }
  Const_Iterator begin() const { return {this, 0}; }
  Const_Iterator end() const { return {this, dim0}; }

private:
  Data_t &mData{};
};
