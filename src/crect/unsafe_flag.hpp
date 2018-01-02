//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace crect
{
//
// Unsafe flag implementation, used to replace flags |= some_flag.
// Does not check if set / reset goes out of bound on the storage.
//
template < std::size_t N >
class unsafe_flag
{
private:
  // Sets the N_bits number of top bits
  static constexpr std::uint32_t set_N_top_bits(std::size_t N_bits)
  {
    std::uint32_t r = 0;

    for (auto i = 0U; i < N_bits; i++)
      r |= (1 << (31 - i));

    return r;
  }

  // Constants
  static const constexpr auto N32 = (N + 31) / 32;
  static const constexpr std::uint32_t top_mask =
      ~set_N_top_bits(32 - (N % 32));

  // Storage for the flags
  std::uint32_t storage_[N32];

public:
  constexpr unsafe_flag() noexcept : storage_{}
  {
  }

  // Set all flags
  constexpr void set() noexcept
  {
    for (auto i   = 0U; i < N32 - 1; ++i)
      storage_[i] = 0xffffffff;

    storage_[N32 - 1] = top_mask;
  }

  // Set a flag at a specific index
  constexpr void set(const std::size_t idx) noexcept
  {
    storage_[idx / 32] |= (1 << (idx % 32));
  }

  // Reset all flags
  constexpr void reset() noexcept
  {
    for (auto i   = 0U; i < N32; ++i)
      storage_[i] = 0;
  }

  // Reset a flag as a specific index
  constexpr void reset(const std::size_t idx) noexcept
  {
    storage_[idx / 32] &= ~(1 << (idx % 32));
  }

  // Flip a flag at a specific index
  constexpr void flip(const std::size_t idx) noexcept
  {
    storage_[idx / 32] ^= (1 << (idx % 32));
  }

  // Check if no flags are set
  constexpr bool none() const noexcept
  {
    for (auto i = 0U; i < N32; ++i)
      if (storage_[i] != 0)
        return false;

    return true;
  }

  // Check if all flags are set
  constexpr bool all() const noexcept
  {
    for (auto i = 0U; i < N32 - 1; ++i)
      if (storage_[i] != 0xffffffff)
        return false;

    if (storage_[N32 - 1] != top_mask)
      return false;

    return true;
  }

  // Access a specific flag and check if it's set
  constexpr bool operator[](const std::size_t idx) const noexcept
  {
    return storage_[idx / 32] & (1 << (idx % 32));
  }
};

}  // namespace esl
