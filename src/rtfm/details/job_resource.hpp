
#pragma once

#include "kvasir/mpl/mpl.hpp"


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
 * @tparam UID_   Job unique ID.
 * @tparam Prio_  Priority.
 * @tparam ISR    ISR definition.
 * @tparam Res    Parameter pack of resources.
 */
template <unsigned UID_, unsigned Prio_, typename ISR_, typename... Res>
struct Job
{
  using UID = kvasir::mpl::integral_constant<unsigned, UID_>;
  using P = kvasir::mpl::integral_constant<unsigned, Prio_>;
  using ISR = ISR_;
  using resources = kvasir::mpl::flatten< kvasir::mpl::list<Res...> >;

  /* Using < for now, comes from setting basepri = 0 has no effect. */
  static_assert(Prio_ < max_priority{},
                "Priority is higher than the max allowed");
};


/**
 * @brief Resource type definition.
 *
 * @tparam ID_    Resouce "ID", a unique type that identifies the resouce.
 * @tparam Jobs   Parameter pack of jobs.
 */
template <typename ID_, typename... Jobs>
struct Resource
{
  using ID = ID_;
  using type = ID_;
  using jobs = kvasir::mpl::flatten< kvasir::mpl::list<Jobs...> >;
};

} /* END namespace rtfm */
