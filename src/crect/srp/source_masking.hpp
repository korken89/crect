//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/****************************************************************************
 *
 * This file is intended to be included after the definition of the system's
 * job list.
 *
 ****************************************************************************/

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource_definitions.hpp"
#include "crect/details/job_resource_comparisons.hpp"
#include "crect/details/job_resource_transformations.hpp"
#include "crect/details/resource_tree.hpp"
#include "crect/details/resource_search.hpp"

namespace crect
{
namespace details
{

/**
 * @brief Extracts the interrupt mask of each of the jobs connected to the
 *        resource.
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to get source mask from.
 */
template <typename JobList, typename Resource>
using resource_to_isr_mask_list =
                kvasir::mpl::eager::join< kvasir::mpl::eager::transform<
                  kvasir::mpl::eager::join< typename find_resource<JobList,
                                                            Resource>::jobs >,
                  job_to_isr_mask
                > >;

} /* END namespace details */

/**
 * @brief Extracts the interrupt mask of the jobs connected to the resource.
 *
 * @tparam  JobList   The system's job list.
 * @tparam Resource   Resource to get source mask for.
 */
template <typename JobList, typename Resource>
using get_source_masking =
                 kvasir::mpl::eager::fold_right<
                   details::resource_to_isr_mask_list<JobList, Resource>,
                   kvasir::mpl::integral_constant<unsigned, 0>,
                   kvasir::mpl::eager::bitwise_or
                 >;

} /* END namespace crect */

