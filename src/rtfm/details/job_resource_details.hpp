
#pragma once

#include "kvasir/mpl/mpl.hpp"
#include "rtfm/details/job_resource.hpp"

namespace rtfm
{
namespace details
{

template <typename... Ts>
struct job_to_resource_impl
{
  static_assert(util::always_false<Ts...>::value, "Should not come here");
};

/**
 * @brief Takes a Job< Resource... > and returns Resource<Job>...
 *
 * @tparam I1   Job unique ID.
 * @tparam I2   Priority.
 * @tparam ISR  ISR definition.
 * @tparam Res  Parameter pack of resources.
 */
template <unsigned I1, unsigned I2, typename ISR, typename... Res>
struct job_to_resource_impl< Job<I1, I2, ISR, Res...> >
{
  using f = kvasir::mpl::list< Resource<typename Res::ID, Job<I1, I2, ISR> >... >;
};


/**
 * @brief Alias to compare two Job IDs.
 *
 * @tparam Lhs   Left hand side.
 * @tparam Rhs   Right hand side.
 */
template <typename Lhs, typename Rhs>
using _compare_job_ids = kvasir::mpl::bool_< (std::is_same<typename Lhs::UID, typename Rhs::UID>::value) >;

/**
 * @brief Alias to compare two Job IDs.
 *
 * @tparam Lhs   Left hand side.
 * @tparam Rhs   Right hand side.
 */
template  <typename UID, typename Rhs>
using _compare_job_ids_to_constant = kvasir::mpl::bool_< (UID::value == Rhs::UID::value) >;


template <typename... Ts>
struct _merge_resources_impl
{
  static_assert(util::always_false<Ts...>::value,
                "Merging different resources are not allowed");
};


/**
 * @brief Merges resources of same ID.
 *
 * @tparam ID    Type of the ID.
 * @tparam Jobs1   Left hand side pack of jobs.
 * @tparam Jobs2   Right hand side pack of jobs.
 * @tparam Job     Right hand side first job.
 */
template <typename ID, typename... Jobs1, typename... Jobs2, typename Job>
struct _merge_resources_impl< Resource<ID, Jobs1...>, Resource<ID, Job, Jobs2...> >
{
 // using count = kvasir::mpl::count_if<
 //       kvasir::mpl::list<Jobs1...>,
 //       kvasir::mpl::bind< _compare_job_ids, Job, kvasir::mpl::_1 >
 //   >;
//
 // static_assert(count::value < 1,
 //     "Duplicate jobs defined, each job must have a unique ID");
//
  using result =
    typename _merge_resources_impl< Resource<ID, Jobs1..., Job>,
                                    Resource<ID, Jobs2...> >::result::type;
};

/**
 * @brief Merges resources of same ID.
 *
 * @tparam ID    Type of the ID.
 * @tparam Jobs1 Left hand side pack of jobs.
 * @tparam Job     Right hand side last job.
 */
template <typename ID, typename... Jobs1, typename Job>
struct _merge_resources_impl< Resource<ID, Jobs1...>, Resource<ID, Job> >
{
//  using count = kvasir::mpl::count_if<
//        kvasir::mpl::list<Jobs1...>,
//        kvasir::mpl::bind< _compare_job_ids, Job, kvasir::mpl::_1 >
//    >;
//
//  static_assert(count::value < 1,
//      "Duplicate jobs defined, each job must have a unique ID");
//
  using result = Resource<ID, Jobs1..., Job>;
};

/**
 * @brief Implementation for recursive merges of resources.
 *
 * @tparam R1   Current resource.
 * @tparam R2   Next resource to be merged.
 * @tparam Rs   Pack of resources to be merged.
 */
template <typename R1, typename R2, typename... Rs>
struct _rec_merge_impl
{
  using _res = typename _merge_resources_impl<R1, R2>::result::type;
  using type = typename _rec_merge_impl<_res, Rs...>::type;
};

/**
 * @brief Implementation for recursive merges of resources (end case).
 *
 * @tparam R1   Current resource.
 * @tparam R2   Last resource to be merged.
 */
template <typename R1, typename R2>
struct _rec_merge_impl<R1, R2>
{
  using type = typename _merge_resources_impl<R1, R2>::result::type;
};





/**
 * @brief Merge of resource list interface.
 *
 * @tparam Rs   Pack of resources to be merged.
 */
template <typename... Rs>
struct merge_resource_list
{
  using type = typename _rec_merge_impl< Rs... >::type;
};

/**
 * @brief Merge of resource list interface, kvasir::mpl::list as input.
 *
 * @tparam Rs   Pack of resources to be merged.
 */
template <typename... Rs>
struct merge_resource_list< kvasir::mpl::list<Rs...> >
{
  using type = typename _rec_merge_impl< Rs... >::type;
};

/**
 * @brief Merge of resource list interface, single resource.
 *
 * @tparam R    Pack of resources to be merged.
 */
template <typename R>
struct merge_resource_list< kvasir::mpl::list<R> >
{
  using type = R;
};


/**
 * @brief Checks if the left-hand side is the same as the Resource ID.
 *
 * @tparam Lhs   Left hand side.
 * @tparam Rhs   Right hand side.
 */
//template <typename Lhs, typename Rhs>
//using _same_job_uid = brigand::bool_< (std::is_same<Lhs, typename Rhs::ID>::value) >;

/**
 * @brief Checks if the left-hand side is the same as the Resource ID.
 *
 * @tparam Lhs   Left hand side.
 * @tparam Rhs   Right hand side.
 */
template <typename Lhs, typename Rhs>
using _same_id = kvasir::mpl::bool_< (std::is_same<Lhs, typename Rhs::ID>::value) >;

/**
 * @brief Checks if the Resource ID is same.
 *
 * @tparam Lhs   Left hand side.
 * @tparam Rhs   Right hand side.
 */
template <typename Lhs, typename Rhs>
using _same_id_two_resources = _same_id<typename Lhs::ID, Rhs>;

/**
 * @brief Checks if the left-hand side is different from the Resource ID.
 *
 * @tparam Lhs   Left hand side.
 * @tparam Rhs   Right hand side.
 */
template <typename Lhs, typename Rhs>
using _different_id = kvasir::mpl::bool_< !(std::is_same<Lhs, typename Rhs::ID>::value) >;

/**
 * @brief Extracts all the resources of the same ID as the specified resource.
 *
 * @tparam List  List of resources.
 * @tparam Res   Resource to compare with.
 */
template <typename List, typename Res>
using keep_resource_if_same_id =
  kvasir::mpl::remove_if< kvasir::mpl::bind<
                            _different_id,
                            Res
                          >::template f,
                          List
                        >;

/**
 * @brief Extracts all the resources of different ID to the specified resource.
 *
 * @tparam List  List of resources.
 * @tparam Res   Resource to compare with.
 */
template <typename List, typename Res>
using remove_resource_if_same_id =
  kvasir::mpl::remove_if< kvasir::mpl::bind<
                            _same_id,
                            Res
                          >::template f,
                          List
                        >;



template <typename... Rs>
struct make_resource_tree_impl
{
  static_assert(util::always_false<Rs...>::value, "Implausible parameters");
};

/**
 * @brief Creates a list of resources where each job associated do a resource
 *        is bundled with it.
 *
 * @tparam ResList   List of resources.
 */
template <typename ResList>
struct make_resource_tree_impl<ResList>
{
  using _kr = keep_resource_if_same_id< ResList, typename kvasir::mpl::pop_front<ResList>::front >;
  using _rr = remove_resource_if_same_id< ResList, typename kvasir::mpl::pop_front<ResList>::front >;
  using f = kvasir::mpl::flatten< kvasir::mpl::list<
                 typename merge_resource_list< _kr >::type,
                 typename make_resource_tree_impl< _rr >::type
               > >;
};

template <>
struct make_resource_tree_impl< kvasir::mpl::list<> >
{
  using f = kvasir::mpl::list<>;
};

} /* END namespace details */

template <typename T>
using job_to_resource = typename details::job_to_resource_impl<T>::f;

// template <typename T>
// using make_resource_tree = typename details::make_resource_tree_impl<T>::f;

} /* END namespace rtfm */
