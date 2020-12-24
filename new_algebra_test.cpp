#include "ndarray.hpp"
#include "matrix.hpp"

constexpr float arr[2][2] = {{4.9, 2}, {3.4, 11}};

constexpr NdArray<float, 2> nda4{arr[0]};

constexpr Vector<float, 2> vec1{nda4};

constexpr Matrix<float, 1, 2> vec_to_mat = vec1.to_matrix();

constexpr auto ndaT = static_cast<Vector<float, 2>>(arr[0]).transpose();

constexpr const Matrix<float, 2, 2> &mat{arr};

constexpr Matrix<float, 2, 2> mat2 = mat;

constexpr const Vector<float, 2> &mm{static_cast<const NdArray<float, 2, 2> &>(mat2)[0]};

constexpr auto vec_mul = mat2 * mat2;

template <typename Scalar_t, std::size_t N>
constexpr Matrix<float, N, N> Identity()
{
  Matrix<float, N, N> id;
  for (std::size_t i = 0; i < N; ++i)
    id[i][i] = 1;
  return id;
}

//*
#include <iostream>
template <typename T, std::size_t lines, std::size_t columns>
void prt_mat(const NdArray<T, lines, columns> &mat)
{
  for (auto &line : mat)
  {
    for (auto &val : line)
      std::cout << val << "\t";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

template <typename T, std::size_t lines>
void prt_vec(const NdArray<T, lines> &vec)
{
  for (auto &val : vec)
    std::cout << val << "\n";
  std::cout << std::endl;
}
//*/

int main()
{
  (void)vec_to_mat;
  (void)ndaT;
  (void)mat2;
  (void)vec_mul;
  prt_mat(vec_mul);
  Matrix<float, 2, 2> m;
  m[0] = (ndaT.transpose() * Identity<float, 2>())[0];
  return static_cast<NdArray<float, 2, 2> &>(m)[0][0];
}
