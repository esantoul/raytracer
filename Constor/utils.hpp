#pragma once

#include <cstddef>

namespace Constor
{
  namespace utils
  {
    template <std::size_t dim0, std::size_t... dims>
    struct get_first
    {
      static constexpr std::size_t value = dim0;
    };

    template <std::size_t... dims>
    constexpr std::size_t get_first_v = get_first<dims...>::value;
  } // namespace utils
} // namespace Constor
