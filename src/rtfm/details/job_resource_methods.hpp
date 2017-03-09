
#pragma once


#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/interrupts.hpp"
#include "rtfm/details/job_resource.hpp"
#include "rtfm/details/job_resource_details.hpp"

namespace rtfm
{

/**
 * @brief Interface to generate the resource tree.
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

template <typename ID, typename OT, OT *Object>
using MakeResource = Resource<ID, OT, Object, false>;

template <typename ID, typename OT, OT *Object>
using MakeUniqueResource = Resource<ID, OT, Object, true>;

template <typename ID>
using MakeVirtualResource = Resource<ID, decltype(nullptr), nullptr, false>;

template <typename ID>
using MakeUniqueVirtualResource =
    Resource<ID, decltype(nullptr), nullptr, true>;

template <typename... Resources>
using MakeResourceAlias = kvasir::mpl::list<Resources...>;

} /* END namespace rtfm */
