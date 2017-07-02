
#pragma once

#include "kvasir/mpl/mpl.hpp"

namespace crect
{
namespace details
{
/**
 * @brief ISR function pointer alias.
 */
using isr_function_pointer = void (*)(void);

/**
 * @brief ISR index alias.
 */
template <int I>
using idx = kvasir::mpl::integral_constant<int, I>;

/**
 * @brief Base definition of an ISR type, should not be used.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index type.
 */
template <isr_function_pointer P, typename I>
struct isr
{
  static_assert(kvasir::mpl::eager::always_false< I >::value, "Index type error");
};

/**
 * @brief Specialization of an ISR type, system ISR definition.
 *
 * @tparam I  Index value.
 */
template <int I>
struct isr<nullptr, idx<I>>
{
  static_assert((I < 0), // System ISR
               "A system ISR must have a negative Index.");

  static constexpr const isr_function_pointer value = nullptr;
  using type = isr<nullptr, idx<I>>;
  using index = idx<I>;
};

/**
 * @brief Specialization of an ISR type, peripheral ISR definition.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <isr_function_pointer P, int I>
struct isr<P, idx<I>>
{
  static_assert((I >= 0), // Peripheral ISR
                "A peripheral ISR must have a non-negative Index.");

  static constexpr const isr_function_pointer value = P;
  using type = isr<P, idx<I>>;
  using index = idx<I>;
};
} /* END namespace details */

/**
 * @brief Wrapper of an ISR type for ease of use.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <details::isr_function_pointer P, int I>
using make_isr = details::isr<P, details::idx<I>>;

/**
 * @brief Wrapper of an System ISR type for ease of use.
 *
 * @tparam I  Index value.
 */
template <int I>
using make_system_isr = details::isr<nullptr, details::idx<I>>;

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

} /* END namespace crect */
