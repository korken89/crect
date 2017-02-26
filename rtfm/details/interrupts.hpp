
#pragma once

#include "brigand/brigand.hpp"
#include "util/utils.hpp"

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
using Index = brigand::integral_constant<int, I>;

/**
 * @brief Base definition of an ISR type, should not be used.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index type.
 */
template <ISRFunctionPointer P, typename I>
struct ISR
{
  static_assert(util::always_false< I >::value, "Index type error");
};

/**
 * @brief Specialization of an ISR type, main usage definition.
 *
 * @tparam P  Function pointer.
 * @tparam I  Index value.
 */
template <ISRFunctionPointer P, int I>
struct ISR<P, Index<I>>
{
  // 1) (I < 0) || (I >= 0 && P != nullptr)    Peripheral ISR
  // 2) (I >= 0) || (I < 0 && P == nullptr)    System ISR
  
  static_assert(((I < 0) || (I >= 0 && P != nullptr)), // Peripheral ISR
                "A peripheral ISR cannot have a nullptr as function.");

  static_assert(((I >= 0) || (I < 0 && P == nullptr)), // System ISR
               "A system ISR must have a nullptr as function, the pointer is " \
               "not used in the generation of the vector table.");

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
