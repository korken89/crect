
#pragma once


#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_details.hpp"

namespace rtfm
{

/**
 * @brief Interface to make_resource_tree_impl.
 *
 * @tparam JobList   List of Jobs.
 */
template <typename JobList>
struct make_resource_tree
{
  /* Take all the job<resources> and transform to resource<jobs> */
  using j2r = kvasir::mpl::flatten<
                kvasir::mpl::transform<
                  job_to_resource,
                  kvasir::mpl::flatten< JobList >
                > >;

  using result = typename details::make_resource_tree_impl<j2r>::f;
};

} /* END namespace rtfm */
