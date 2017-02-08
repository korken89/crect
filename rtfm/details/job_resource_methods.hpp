
#pragma once


#include "brigand/brigand.hpp"
#include "util/utils.hpp"
#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_details.hpp"

namespace rtfm
{

/**
 * @brief Interface to make_resource_tree_impl.
 *
 * @tparam Res   Pack of resources.
 */
template <typename... Res>
struct make_resource_tree
{
  /* Take all the job<resources> and transform to resource<jobs> */
  using j2r = brigand::flatten<
                brigand::transform<
                  brigand::flatten< brigand::list< Res... > >,
                  details::job_to_resource_impl<brigand::_1>
                > >;

  using result = typename details::make_resource_tree_impl<j2r>::type;
};

} /* END namespace rtfm */
