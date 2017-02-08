
#pragma once

namespace rtfm
{

/**
 * @brief Job type definition.
 *
 * @tparam UID_   Job unique ID.
 * @tparam Prio_  Priority.
 * @tparam ISR    ISR definition.
 * @tparam Res    Parameter pack of resources.
 */
template <int UID_, int Prio_, typename ISR, typename... Res>
struct Job
{
  using UID = brigand::integral_constant<int, UID_>;
  using P = brigand::integral_constant<int, Prio_>;
  using resources = brigand::flatten< brigand::list<Res...> >;
};


/**
 * @brief Resource type definition.
 *
 * @tparam ID_    Job unique ID.
 * @tparam Jobs   Parameter pack of jobs.
 */
template <typename ID_, typename... Jobs>
struct Resource
{
  using ID = ID_;
  using type = ID_;
  using jobs = brigand::flatten< brigand::list<Jobs...> >;
};

} /* END namespace rtfm */
