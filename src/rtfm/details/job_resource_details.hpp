
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/job_resource.hpp"

namespace rtfm
{
namespace details
{

/**
 * @brief Takes a job< resource... > and returns resource<job>..., base case
 *        that always will fail.
 */
template <typename... Ts>
struct job_to_resource_impl
{
  static_assert(kvasir::mpl::always_false<Ts...>{}, "Should not come here");
};

/**
 * @brief Takes a job< resource... > and returns resource<job>...
 *
 * @tparam I1   Job unique ID.
 * @tparam I2   Priority.
 * @tparam ISR  ISR definition.
 * @tparam Res  Parameter pack of resources.
 */
template <unsigned I1, unsigned I2, typename ISR, typename... Res>
struct job_to_resource_impl< job<I1, I2, ISR, Res...> >
{
  using f = kvasir::mpl::list< resource<typename Res::id, typename Res::object,
        typename Res::is_unique{}, job<I1, I2, ISR> >... >;
};


/**
 * @brief Compare two Job IDs.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template <typename A>
struct _same_job_id {
  template <typename B>
  using f = std::is_same<typename A::uid, typename B::uid>;
};

/**
 * @brief Checks if resource ID is same.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template <typename A>
struct _same_resource_id {
  template <typename B>
  using f = std::is_same<typename A::id, typename B::id>;
};

/**
 * @brief Checks if resource ID is different.
 *
 * @tparam A   Left hand side.
 * @tparam B   Right hand side.
 */
template <typename A>
struct _different_resource_id {
  template <typename B>
  using f = kvasir::mpl::invert< std::is_same<typename A::id, typename B::id> >;
};


/**
 * @brief Merges resources of same ID, base case - should not come here.
 */
template <typename... Ts>
struct merge_resources_impl
{
  static_assert(kvasir::mpl::always_false<Ts...>{},
                "Merging different resources are not allowed");
};

/**
 * @brief Merges resources of same ID.
 *
 * @tparam ID     Type of the ID.
 * @tparam Jobs1  Left hand side pack of jobs.
 * @tparam Job    Right hand side last job.
 */
template <typename ID, typename Obj1, typename Obj2, bool Unq1, bool Unq2,
          typename... Jobs1, typename Job>
struct merge_resources_impl< resource<ID, Obj1, Unq1, Jobs1...>,
                             resource<ID, Obj2, Unq2, Job> >
{
  static_assert( !kvasir::mpl::any< _same_job_id<Job>::template f,
                                   kvasir::mpl::list<Jobs1...> >{},
      "Duplicate jobs defined, each job must have a unique ID");

  static_assert( std::is_same<Obj1, Obj2>::value,
                 "Resource has different objects defined.");

  static_assert( (Unq1 == Unq2),
                 "Resource has different uniqueness defined.");

  using f = resource<ID, Obj1, Unq1, Jobs1..., Job>;
};


/**
 * @brief Work alias that merges resources of same ID.
 *
 * @tparam Ts    Parameters to be forwarded to merge_resources_impl.
 */
template <typename R1, typename R2>
using merge_resources = typename merge_resources_impl<R1, R2>::f;



/**
 * @brief Creates a list of resources where each job associated to a resource
 *        is bundled with it.
 *
 * @tparam ResList   List of resources.
 */
template <typename ResList>
struct make_resource_tree_impl
{

  using _current =  kvasir::mpl::pop_front< kvasir::mpl::remove_if<
                      _different_resource_id<
                        typename kvasir::mpl::pop_front<ResList>::front
                      >::template f,
                      ResList
                    > >;

  using _next =     kvasir::mpl::remove_if<
                      _same_resource_id<
                        typename kvasir::mpl::pop_front<ResList>::front
                      >::template f,
                      ResList
                    >;

  using f = kvasir::mpl::join< kvasir::mpl::list<
              kvasir::mpl::fold_right<
                merge_resources,
                typename _current::front,
                typename _current::rest
              > >,
              typename make_resource_tree_impl< _next >::f
            >;
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

template <typename T>
using job_to_resource = typename details::job_to_resource_impl<T>::f;


} /* END namespace rtfm */
