//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "kvasir/mpl/mpl.hpp"
#include "crect/crect.hpp"
#include "crect/vector_table.hpp"

/**
 * @brief Unhandled exception definition.
 */
extern "C" void _unhandled_exception(void)
{
  while(1);
}

/**
 * @brief Save vector table to the correct location.
 */
__attribute__((used, section(".isr_vector")))
constexpr const vector_table system_vectors = generate_vector_table(
    std::make_integer_sequence<
        unsigned, sizeof(vector_table::mcu_vectors) /
                      sizeof(crect::details::isr_function_pointer)>{});
