
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "rtfm/rtfm_utils.hpp"


namespace rtfm
{

/**
 * @brief Job maximum priority.
 */
using max_priority = kvasir::mpl::integral_constant<unsigned,
                                                (1U << __NVIC_PRIO_BITS) - 1U>;

/**
 * @brief Job type definition.
 *
 * @tparam Prio   Priority.
 * @tparam ISR    ISR definition.
 * @tparam Res    Parameter pack of resources.
 */
template <unsigned Prio, typename ISR, typename... Res>
struct job
{
  using prio = kvasir::mpl::integral_constant<unsigned, Prio>;
  using isr = ISR;
  using resources = kvasir::mpl::eager::flatten< kvasir::mpl::list<Res...> >;

  /* Using < for now, comes from setting basepri = 0 has no effect. */
  static_assert(Prio < max_priority::value,
                "Priority is higher than the max allowed");
};


/**
 * @brief Resource type definition.
 *
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Unique   Flag to indicate if it is a unique resource.
 * @tparam Jobs     Parameter pack of jobs.
 */
template <typename Object, bool Unique, typename... Jobs>
struct resource
{
  static_assert(kvasir::mpl::eager::is_integral<Object>::value,
                "Object must be an integral constant.");

  static_assert((std::is_pointer<util::get_integral_type<Object>>::value ||
                 util::is_nullptr<Object>::value),
                "The type of the object must be a pointer.");

  static_assert(util::is_nullptr<Object>::value == false,
                "The object cannot be nullptr, does not make sense.");

  using object = Object;
  using has_object = typename kvasir::mpl::bool_<!util::is_nullptr<Object>::value>;
  using is_unique = kvasir::mpl::bool_<Unique>;
  using jobs = kvasir::mpl::eager::flatten< kvasir::mpl::list<Jobs...> >;
};

/**
 * @brief Resource convenience type definition.
 *
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Jobs     Parameter pack of jobs.
 */
template <typename Object, typename... Jobs>
using make_resource = resource<Object, false, Jobs...>;

/**
 * @brief Unique resource convenience type definition.
 *
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Jobs     Parameter pack of jobs.
 */
template <typename Object, typename... Jobs>
using make_unique_resource = resource<Object, true, Jobs...>;

/**
 * @brief Convenience define for creating an object link until we have
 *        auto templates.
 */
#define RTFM_OBJECT_LINK(obj) \
        kvasir::mpl::integral_constant< decltype(&obj), (&obj) >

} /* END namespace rtfm */
