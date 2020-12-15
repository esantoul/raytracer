#pragma once

#include <cstddef>

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class Tensor_ref;

template <typename Source_t, typename Scalar_t, std::size_t dim0, std::size_t... dims>
class TensorBase
{
public:
  using Decayed_t = Tensor_ref<Scalar_t, dims...>;
  using Const_Decayed_t = const Tensor_ref<const Scalar_t, dims...>;
  using Data_t = typename Decayed_t::Data_t[dim0];

  class Iterator
  {
    friend Source_t;

  public:
    Iterator &operator++()
    {
      if (mIndex < dim0)
        ++mIndex;
      return *this;
    }
    bool operator==(const Iterator &other) const { return pParent == other.pParent && mIndex == other.mIndex; }
    bool operator!=(const Iterator &other) const { return !(*this == other); }
    Decayed_t operator*() { return (*pParent)[mIndex]; }

  private:
    Iterator(Source_t *source, std::size_t index) : pParent{source}, mIndex{index} {}

    Source_t *pParent;
    std::size_t mIndex;
  };

  class Const_Iterator
  {
    friend Source_t;

  public:
    Const_Iterator &operator++()
    {
      if (mIndex < dim0)
        ++mIndex;
      return *this;
    }
    bool operator==(const Const_Iterator &other) const { return pParent == other.pParent && mIndex == other.mIndex; }
    bool operator!=(const Const_Iterator &other) const { return !(*this == other); }
    Const_Decayed_t operator*() const { return (*pParent)[mIndex]; }

  private:
    Const_Iterator(const Source_t *source, std::size_t index) : pParent{source}, mIndex{index} {}

    const Source_t *pParent;
    std::size_t mIndex;
  };
};

template <typename Source_t, typename Scalar_t, std::size_t dim0>
class TensorBase<Source_t, Scalar_t, dim0>
{
public:
  using Decayed_t = Scalar_t;
  using Const_Decayed_t = const Scalar_t;
  using Data_t = Decayed_t[dim0];

  class Iterator
  {
    friend Source_t;

  public:
    Iterator &operator++()
    {
      if (mIndex < dim0)
        ++mIndex;
      return *this;
    }
    bool operator==(const Iterator &other) { return pParent == other.pParent && mIndex == other.mIndex; }
    bool operator!=(const Iterator &other) { return !(*this == other); }
    Decayed_t &operator*() { return (*pParent)[mIndex]; }

  private:
    Iterator(Source_t *source, std::size_t index) : pParent{source}, mIndex{index} {}

    Source_t *pParent;
    std::size_t mIndex;
  };

  class Const_Iterator
  {
    friend Source_t;

  public:
    Const_Iterator &operator++()
    {
      if (mIndex < dim0)
        ++mIndex;
      return *this;
    }
    bool operator==(const Const_Iterator &other) { return pParent == other.pParent && mIndex == other.mIndex; }
    bool operator!=(const Const_Iterator &other) { return !(*this == other); }
    Const_Decayed_t &operator*() const { return (*pParent)[mIndex]; }

  private:
    Const_Iterator(const Source_t *source, std::size_t index) : pParent{source}, mIndex{index} {}

    const Source_t *pParent;
    std::size_t mIndex;
  };
};

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class Tensor : public TensorBase<Tensor<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>
{
public:
  using Base_t = TensorBase<Tensor<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>;
  using Decayed_t = typename Base_t::Decayed_t;
  using Data_t = typename Base_t::Data_t;
  using Iterator = typename Base_t::Iterator;
  using Const_Iterator = typename Base_t::Const_Iterator;

  Decayed_t operator[](std::size_t idx) { return mStorage[idx]; }
  typename Base_t::Const_Decayed_t operator[](std::size_t idx) const { return mStorage[idx]; }
  operator Tensor_ref<Scalar_t, dim0, dims...>() { return {mStorage}; }
  operator Tensor_ref<const Scalar_t, dim0, dims...>() const { return {mStorage}; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }
  Const_Iterator begin() const { return {this, 0}; }
  Const_Iterator end() const { return {this, dim0}; }

private:
  Data_t mStorage{};
};

template <typename Scalar_t, std::size_t dim0>
class Tensor<Scalar_t, dim0> : public TensorBase<Tensor<Scalar_t, dim0>, Scalar_t, dim0>
{
public:
  using Base_t = TensorBase<Tensor<Scalar_t, dim0>, Scalar_t, dim0>;
  using Decayed_t = typename Base_t::Decayed_t;
  using Data_t = typename Base_t::Data_t;
  using Iterator = typename Base_t::Iterator;
  using Const_Iterator = typename Base_t::Const_Iterator;

  Decayed_t &operator[](std::size_t idx) { return mStorage[idx]; }
  const Decayed_t &operator[](std::size_t idx) const { return mStorage[idx]; }
  operator Tensor_ref<Scalar_t, dim0>() { return {mStorage}; }
  operator Tensor_ref<const Scalar_t, dim0>() const { return {mStorage}; }
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
  using Iterator = typename Base_t::Iterator;
  using Const_Iterator = typename Base_t::Const_Iterator;

  Tensor_ref(Data_t &data) : mData{data} {}
  Decayed_t operator[](std::size_t idx) { return mData[idx]; }
  const Decayed_t operator[](std::size_t idx) const { return mData[idx]; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }
  Const_Iterator begin() const { return {this, 0}; }
  Const_Iterator end() const { return {this, dim0}; }

private:
  Data_t &mData{};
};

template <typename Scalar_t, std::size_t dim0>
class Tensor_ref<Scalar_t, dim0> : public TensorBase<Tensor_ref<Scalar_t, dim0>, Scalar_t, dim0>
{
public:
  using Base_t = TensorBase<Tensor_ref<Scalar_t, dim0>, Scalar_t, dim0>;
  using Decayed_t = typename Base_t::Decayed_t;
  using Data_t = typename Base_t::Data_t;
  using Iterator = typename Base_t::Iterator;
  using Const_Iterator = typename Base_t::Const_Iterator;

  Tensor_ref(Data_t &data) : mData{data} {}
  Decayed_t &operator[](std::size_t idx) { return mData[idx]; }
  const Decayed_t &operator[](std::size_t idx) const { return mData[idx]; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }
  Const_Iterator begin() const { return {this, 0}; }
  Const_Iterator end() const { return {this, dim0}; }

private:
  Data_t &mData{};
};
