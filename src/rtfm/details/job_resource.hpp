
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
 * @tparam UID    Job unique ID.
 * @tparam Prio   Priority.
 * @tparam ISR    ISR definition.
 * @tparam Res    Parameter pack of resources.
 */
template <unsigned UID, unsigned Prio, typename ISR, typename... Res>
struct job
{
  using uid = kvasir::mpl::integral_constant<unsigned, UID>;
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
 * @tparam ID       Resource "ID", a unique type that identifies the resource.
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Unique   Flag to indicate if it is a unique resource.
 * @tparam Jobs     Parameter pack of jobs.
 */
template <typename ID, typename Object, bool Unique, typename... Jobs>
struct resource
{
  static_assert(kvasir::mpl::is_integral<Object>::value,
                "Object must be an integral constant.");

  static_assert((std::is_pointer<util::get_integral_type<Object>>::value ||
                 util::is_nullptr<Object>::value),
                "The type of the object must be a pointer.");

  using id = ID;
  using object = Object;
  using has_object = typename kvasir::mpl::bool_<!util::is_nullptr<Object>::value>;
  using is_unique = kvasir::mpl::bool_<Unique>;
  using jobs = kvasir::mpl::eager::flatten< kvasir::mpl::list<Jobs...> >;
};

/**
 * @brief Resource convenience type definition.
 *
 * @tparam ID       Resource ID, a unique type that identifies the resource.
 * @tparam Object   Integral_constant that contains a pointer to an object.
 * @tparam Jobs     Parameter pack of jobs.
 */
template <typename ID, typename Object, typename... Jobs>
using make_resource = resource<ID, Object, false, Jobs...>;

/**
 * @brief Virtual resource convenience type definition.
 *
 * @tparam ID       Resource ID, a unique type that identifies the resource.
 * @tparam Jobs     Parameter pack of jobs.
 */
template <typename ID, typename... Jobs>
using make_virtual_resource =
    resource<ID, kvasir::mpl::integral_constant<decltype(nullptr), nullptr>,
             false, Jobs...>;

/**
 * @brief Unique virtual resource convenience type definition.
 *
 * @tparam ID       Resource ID, a unique type that identifies the resource.
 * @tparam Jobs     Parameter pack of jobs.
 */
template <typename ID, typename... Jobs>
using make_unique_resource =
    resource<ID, kvasir::mpl::integral_constant<decltype(nullptr), nullptr>,
             true, Jobs...>;

/**
 * @brief Convenience define for creating an object link until we have
 *        auto templates.
 */
#define RTFM_OBJECT_LINK(obj) \
        kvasir::mpl::integral_constant< decltype(&obj), (&obj) >

} /* END namespace rtfm */
