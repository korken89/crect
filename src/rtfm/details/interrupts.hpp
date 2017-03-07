
#pragma once

#include "kvasir/mpl/mpl.hpp"

namespace rtfm
{
namespace details
{
/**
 * @brief ISR function pointer alias.
 */
using ISRFunctionPointer = void (*)(void);

/**
 * @brief ISR index alias.
 */
template <int I>
using Index = kvasir::mpl::integral_constant<int, I>;

/**
 * @brief Base definition of an ISR type, should not be used.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index type.
 */
template <ISRFunctionPointer P, typename I>
struct ISR
{
  static_assert(kvasir::mpl::always_false< I >{}, "Index type error");
};

/**
 * @brief Specialization of an ISR type, system ISR definition.
 *
 * @tparam I  Index value.
 */
template <int I>
struct ISR<nullptr, Index<I>>
{
  static_assert((I < 0), // System ISR
               "A system ISR must have a negative Index.");

  static constexpr const ISRFunctionPointer value = nullptr;
  using type = ISR<nullptr, Index<I>>;
  using index = Index<I>;
};

/**
 * @brief Specialization of an ISR type, peripheral ISR definition.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <ISRFunctionPointer P, int I>
struct ISR<P, Index<I>>
{
  static_assert((I >= 0), // Peripheral ISR
                "A peripheral ISR must have a non-negaive Index.");

  static constexpr const ISRFunctionPointer value = P;
  using type = ISR<P, Index<I>>;
  using index = Index<I>;
};
} /* END namespace details */

/**
 * @brief Wrapper of an ISR type for ease of use.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <details::ISRFunctionPointer P, int I>
using MakeISR = details::ISR<P, details::Index<I>>;

/**
 * @brief Wrapper of an System ISR type for ease of use.
 *
 * @tparam I  Index value.
 */
template <int I>
using MakeSystemISR = details::ISR<nullptr, details::Index<I>>;

/**
 * @brief Default ISR definition, statically defined. A simple forever loop.
 */
class DefaultISR
{
public:
  static void UnusedISR()
  {
    while (1);
  }
};

}
