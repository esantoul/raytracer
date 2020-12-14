#pragma once

#include <cstddef>

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class Tensor_ref;

template <typename Source_t, typename Scalar_t, std::size_t dim0, std::size_t... dims>
class TensorBase
{
public:
  using Decayed_t = Tensor_ref<Scalar_t, dims...>;
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
    bool operator==(const Iterator &other) { return pParent == other.pParent && mIndex == other.mIndex; }
    bool operator!=(const Iterator &other) { return !(*this == other); }
    Decayed_t operator*() { return (*pParent)[mIndex]; }

  private:
    Iterator(Source_t *source, std::size_t index) : pParent{source}, mIndex{index} {}

    Source_t *pParent;
    std::size_t mIndex;
  };
};

template <typename Source_t, typename Scalar_t, std::size_t dim0>
class TensorBase<Source_t, Scalar_t, dim0>
{
public:
  using Decayed_t = Scalar_t;
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
};

template <typename Scalar_t, std::size_t dim0, std::size_t... dims>
class Tensor : public TensorBase<Tensor<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>
{
public:
  using Base_t = TensorBase<Tensor<Scalar_t, dim0, dims...>, Scalar_t, dim0, dims...>;
  using Decayed_t = typename Base_t::Decayed_t;
  using Data_t = typename Base_t::Data_t;
  using Iterator = typename Base_t::Iterator;

  Decayed_t operator[](std::size_t idx) { return mStorage[idx]; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }

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

  Decayed_t &operator[](std::size_t idx) { return mStorage[idx]; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }

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

  Tensor_ref(Data_t &data) : mData{data} {}
  Decayed_t operator[](std::size_t idx) { return mData[idx]; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }

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

  Tensor_ref(Data_t &data) : mData{data} {}
  Decayed_t &operator[](std::size_t idx) { return mData[idx]; }
  Iterator begin() { return {this, 0}; }
  Iterator end() { return {this, dim0}; }

private:
  Data_t &mData{};
};
