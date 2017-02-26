
#pragma once


namespace rtfm
{
namespace srp
{

/**
 * @brief Code generator for the NVIC initialization code.
 */
struct job_to_nvic_printer
{
  template <typename Job>
  void operator()(brigand::type_<Job>) const
  {
#ifndef PC_DEBUG
    using ISRn = typename Job::ISR::index;
    using Prio = typename Job::P;


    if (ISRn::value < 0)
    {
      constexpr auto index = (static_cast<uint32_t>(ISRn::value) & 0xFUL) - 4UL;

      /* Enable system interrupt. */
      SCB->SHP[index] = util::priority_to_NVIC_priority(Prio::value);
    }
    else
    {
      /* Enable interrupt. */
      NVIC->ISER[ISRn::value >> 5UL] = (1UL << (ISRn::value & 0x1FUL));

      /* Set priority */
      NVIC->IP[ISRn::value] = util::priority_to_NVIC_priority(Prio::value);
    }
#endif
  }
};

/**
 * @brief Initialize the NVIC based on a list of RTFM jobs.
 *
 * @tparam JobList    The system's job list.
 */
template <typename JobList>
constexpr void initialize_jobs_impl()
{
  /* Enable DWT */
  DWT->CTRL |= (1 << DWT_CTRL_CYCCNTENA_Pos);

  /* Fill the ISR settings. */
  brigand::for_each<JobList>(job_to_nvic_printer{});
}


} /* END namespace srp */
} /* END namespace rtfm */
