
#pragma once

#include "brigand/brigand.hpp"
#include "util/utils.hpp"

namespace details
{
/**
 * @brief ISR function pointer alias.
 */
using ISRFunctionPointer = void (*)(void);

/**
 * @brief ISR index alias.
 */
template <unsigned I>
using Index = brigand::integral_constant<unsigned int, I>;

/**
 * @brief Base definition of an ISR type, should not be used.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index type.
 */
template <ISRFunctionPointer P, typename I>
struct ISR
{
  static_assert(always_false< I >::value, "Index type error");
};

/**
 * @brief Specialization of an ISR type, main usage definition.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <ISRFunctionPointer P, unsigned I>
struct ISR<P, Index<I>>
{
  static constexpr const ISRFunctionPointer value = P;
  using type = ISR<P, Index<I>>;
  using index = Index<I>;
};
}

/**
 * @brief Wrapper of an ISR type for ease of use.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <details::ISRFunctionPointer P, unsigned I>
using MakeISR = details::ISR<P, details::Index<I>>;

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
