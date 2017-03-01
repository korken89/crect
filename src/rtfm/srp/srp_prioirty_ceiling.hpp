

#pragma once

/****************************************************************************
 *
 * This file is intended to be included after the definition of the system's
 * job list.
 *
 ****************************************************************************/

#include "brigand/brigand.hpp"
#include "util/utils.hpp"
#include "rtfm/details/job_resource.hpp"

namespace rtfm
{
namespace details
{

/**
 * @brief Alias for the entire system's resource tree.
 *
 * @tparam  JobList   The system's job list.
 */
template <typename JobList>
using resource_tree = typename make_resource_tree<JobList>::result;

/**
 * @brief Finds a resource in the resource tree (implementation).
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to find.
 */
template <typename JobList, typename Resource>
struct find_resource_impl
{
  /* Searches the resource tree for the same Resource ID. */
  using type = brigand::find< resource_tree<JobList>,
                              brigand::bind<
                                details::_same_id,
                                typename Resource::ID,
                                brigand::_1
                              > >;

  static_assert(!std::is_same< type, brigand::list<> >::value,
                "The resource in not registered in RTFM");
};

/**
 * @brief Finds a resource in the resource tree (alias).
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to find.
 */
template <typename JobList, typename Resource>
using find_resource = brigand::front<
                      typename find_resource_impl<JobList, Resource>::type >;

template <typename... Ts>
struct job_to_priority
{
  static_assert(util::always_false<Ts...>::value, "Should not come here");
};

/**
 * @brief Converts a job to an integral_constant containing the job's priority.
 *
 * @tparam ID   Job unique ID.
 * @tparam PRIO Priority.
 * @tparam ISR  ISR definition.
 * @tparam Res  Parameter pack of resources.
 */
template <unsigned ID, unsigned PRIO, typename ISR, typename... Res>
struct job_to_priority< Job<ID, PRIO, ISR, Res...> > : brigand::integral_constant<int, PRIO>
{
};

/**
 * @brief Extracts the priorities of the jobs connected to the resource.
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to get priority list from.
 */
template <typename JobList, typename Resource>
using resource_to_priority_list =
                brigand::flatten< brigand::transform<
                  brigand::flatten< typename find_resource<JobList, Resource>::jobs >,
                  job_to_priority<brigand::_1>
                > >;

} /* END namespace details */

/**
 * @brief Extracts the priority ceiling of the jobs connected to the resource.
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to get priority ceiling for.
 */
template <typename JobList, typename Resource>
using get_priority_ceiling =
                brigand::fold<
                  details::resource_to_priority_list<JobList, Resource>,
                  brigand::integral_constant<unsigned, 0>,
                  brigand::max<brigand::_state, brigand::_element>
                >;

} /* END namespace rtfm */
