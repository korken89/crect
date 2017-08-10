//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource_definitions.hpp"
#include "crect/details/job_resource_comparisons.hpp"

namespace crect
{
namespace details
{

/**************************************************************************/
/* Job to resource (impl)                                                 */
/**************************************************************************/

/**
 * @brief Takes a job< resource... > and returns resource<job>..., base case
 *        that always will fail.
 */
template < typename... Ts >
struct job_to_resource_impl
{
  static_assert(kvasir::mpl::eager::always_false< Ts... >::value,
                "Should not come here");
};

/**
 * @brief Takes a job< resource... > and returns resource<job>...
 *
 * @tparam Prio   Priority.
 * @tparam ISR    ISR definition.
 * @tparam Res    Parameter pack of resources.
 */
template < unsigned Prio, typename ISR, typename... Res >
struct job_to_resource_impl< job< Prio, ISR, Res... > >
{
  using f =
      kvasir::mpl::list< resource< typename Res::object, Res::is_unique::value,
                                   job< Prio, ISR > >... >;
};



/**************************************************************************/
/* Job to priority                                                        */
/**************************************************************************/

/**
 * @brief Converts a job to an integral_constant containing the job's priority.
 * @note Base case, should not come here.
 *
 * @note Used for Cortex-M3 and above, as it has basepri.
 */
template <typename... Ts>
struct job_to_priority
{
  static_assert(kvasir::mpl::eager::always_false<Ts...>::value,
                "Should not come here");
};

/**
 * @brief Converts a job to an integral_constant containing the job's priority.
 *
 * @note  Used for Cortex-M3 and above, as it has basepri.
 *
 * @tparam PRIO Priority.
 * @tparam ISR  ISR definition.
 * @tparam Res  Parameter pack of resources.
 */
template <unsigned PRIO, typename ISR, typename... Res>
struct job_to_priority< job<PRIO, ISR, Res...> > :
    kvasir::mpl::integral_constant<unsigned, PRIO>
{
};



/**************************************************************************/
/* Job to ISR position                                                    */
/**************************************************************************/

/**
 * @brief Converts a job to an integral_constant containing the job's ISR mask.
 * @note  Base case, should not come here.
 *
 * @note  Used for Cortex-M0, as it does not have basepri -- needs to use source
 *        masking.
 */
template <typename... Ts>
struct job_to_isr_mask
{
  static_assert(kvasir::mpl::eager::always_false<Ts...>::value,
                "Should not come here");
};

/**
 * @brief Converts a job to an integral_constant containing the job's ISR mask.
 *
 * @note  Used for Cortex-M0, as it does not have basepri -- needs to use source
 *        masking.
 *
 * @tparam PRIO Priority.
 * @tparam ISR  ISR definition.
 * @tparam Res  Parameter pack of resources.
 */
template <unsigned PRIO, typename ISR, typename... Res>
struct job_to_isr_mask< job<PRIO, ISR, Res...> > :
    kvasir::mpl::integral_constant<unsigned, (1u << (ISR::index::value % 32))>
{
};


/**************************************************************************/
/* Merging of resources (impl)                                            */
/**************************************************************************/

/**
 * @brief Merges resources of same ID, base case - should not come here.
 */
template < typename... Ts >
struct merge_resources_impl
{
  static_assert(kvasir::mpl::eager::always_false< Ts... >::value,
                "Merging different resources are not allowed");
};

/**
 * @brief Merges resources of same ID.
 *
 * @tparam ID     Type of the ID.
 * @tparam Jobs1  Left hand side pack of jobs.
 * @tparam Job    Right hand side last job.
 */
template < typename Obj, bool Unq1, bool Unq2, typename... Jobs1, typename Job >
struct merge_resources_impl< resource< Obj, Unq1, Jobs1... >,
                             resource< Obj, Unq2, Job > >
{
  static_assert(!kvasir::mpl::call< kvasir::mpl::any< _same_job_id< Job > >,
                                    Jobs1... >::value,
                "Duplicate jobs defined, each job must have a unique ID");

  static_assert((Unq1 == Unq2), "Resource has different uniqueness defined.");

  using f = resource< Obj, Unq1, Jobs1..., Job >;
};

/**
 * @brief Work alias that merges resources of same ID.
 *
 * @tparam Ts    Parameters to be forwarded to merge_resources_impl.
 */
template < typename R1, typename R2 >
using merge_resources = typename merge_resources_impl< R1, R2 >::f;

} /* END namespace details */

/**
 * @brief Takes a job< resource... > and returns resource<job>...
 *
 * @tparam Job  The job to turn inside out.
 */
template < typename Job >
using job_to_resource = typename details::job_to_resource_impl< Job >::f;

} /* END namespace crect */
