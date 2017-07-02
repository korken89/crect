
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "crect/details/job_resource.hpp"

namespace crect
{
namespace details
{
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

/**
 * @brief Compare two Job IDs.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A >
struct _same_job_id
{
  template < typename B >
  using f = std::is_same< typename A::isr, typename B::isr >;
};

/**
 * @brief Checks if resource ID is same.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A >
struct _same_resource_id
{
  template < typename B >
  using f = std::is_same< typename A::object, typename B::object >;
};

/**
 * @brief Checks if resource ID is different.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A >
struct _different_resource_id
{
  template < typename B >
  using f = kvasir::mpl::eager::invert<
      typename _same_resource_id< A >::template f< B > >;
};

/**
 * @brief Checks if resource ID is different.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template < typename A, typename B >
using _different_resource_id_2r =
    typename _different_resource_id< A >::template f< B >;

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

/**
 * @brief Creates a list of resources where each job associated to a resource
 *        is bundled with it.
 *
 * @tparam ResList   List of resources.
 */
template < typename ResList >
struct make_resource_tree_impl
{
  using _current = kvasir::mpl::eager::pop_front< kvasir::mpl::eager::remove_if<
      ResList, _different_resource_id< typename kvasir::mpl::eager::pop_front<
                   ResList >::front >::template f > >;

  using _next = kvasir::mpl::eager::remove_if<
      ResList, _same_resource_id< typename kvasir::mpl::eager::pop_front<
                   ResList >::front >::template f >;

  using f = kvasir::mpl::eager::join<
      kvasir::mpl::list< kvasir::mpl::eager::fold_right<
          typename _current::rest, typename _current::front,
          merge_resources > >,
      typename make_resource_tree_impl< _next >::f >;
};

/**
 * @brief Creates a list of resources where each job associated do a resource
 *        is bundled with it.
 *
 * @note  End of recursion.
 */
template <>
struct make_resource_tree_impl< kvasir::mpl::list<> >
{
  using f = kvasir::mpl::list<>;
};

} /* END namespace details */

template < typename T >
using job_to_resource = typename details::job_to_resource_impl< T >::f;

} /* END namespace crect */
