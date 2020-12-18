#include "ndarray.hpp"
#include "matrix.hpp"
#include "vector.hpp"

constexpr float arr[2][2] = {{4.9, 2}, {3.4, 11}};

constexpr NdArray<float, 2> nda4{arr[0]};

constexpr const Matrix<float, 2, 2> &mat = static_cast<const Matrix<float, 2, 2> &>(arr);

Matrix<float, 2, 2> mat2 = mat;

constexpr std::size_t ss = sizeof(NdArray<float, 1>);

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

int main()
{
  prt_vec(nda4);
  prt_mat(mat);
  mat2[1][1] = 15;
  prt_mat(mat2);
  return 0;
}