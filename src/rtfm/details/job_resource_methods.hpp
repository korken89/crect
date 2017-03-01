
#pragma once


#include "brigand/brigand.hpp"
#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_details.hpp"

namespace rtfm
{

/**
 * @brief Interface to make_resource_tree_impl.
 *
 * @tparam Jobs   Pack of Jobs.
 */
template <typename... Jobs>
struct make_resource_tree
{
  /* Take all the job<resources> and transform to resource<jobs> */
  using j2r = brigand::flatten<
                brigand::transform<
                  brigand::flatten< brigand::list< Jobs... > >,
                  details::job_to_resource_impl<brigand::_1>
                > >;

  using result = typename details::make_resource_tree_impl<j2r>::type;
};

} /* END namespace rtfm */
