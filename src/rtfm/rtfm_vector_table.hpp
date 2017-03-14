
#pragma once


#include "kvasir/mpl/mpl.hpp"
#include "rtfm/rtfm_srp.hpp"

extern "C"
{
  void _unhandled_exception(void);
  void Reset_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void NMI_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void HardFault_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void MemManage_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void BusFault_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void UsageFault_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void SVCall_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void DebugMon_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void PendSV_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
  void SysTick_Handler(void) __attribute__((weak, alias("_unhandled_exception")));
}
/**
 * @brief Compare two Job IDs.
 *
 * @tparam A   Integral constant.
 * @tparam B   Job to check.
 */
template <typename A>
struct _same_isr_value {
  template <typename B>
  using f = kvasir::mpl::bool_<A::value ==  B::isr::index::value>;
};

/**
 * @brief Used to find an ISR from a ISR position.
 *
 * @tparam ISR_POS    integral_constant containing the ISR position to find,
 */
template <typename ISR_POS>
struct find_job_isr
{
  /* Searches the resource tree for the Job with the correct ISR. */
  using f =
      kvasir::mpl::find_if<rtfm::system_job_list,
                           _same_isr_value<ISR_POS>::template f>;
};

/**
 * @brief Extracts the ISR from a Job.
 *
 * @tparam JobList  Job list to get ISR from.
 */
template <typename JobList>
struct isr_selector
{
  /* TODO: Check so the ISR is unique... */
  using isr = typename kvasir::mpl::pop_front<JobList>::front::isr;
};

/**
 * @brief Creates the Unused ISR when there is no Job matching.
 */
template <>
struct isr_selector<kvasir::mpl::list<>>
{
  using isr = rtfm::MakeISR<_unhandled_exception, 0>;
};

/**
 * @brief Extracts the ISR from a position.
 *
 * @tparam I   Position to extract the ISR rom.
 */
template <unsigned I>
struct get_vector_from_position
{
  using f = typename isr_selector<
        typename find_job_isr<kvasir::mpl::integral_constant<unsigned, I>>::f
      >::isr;
};

/**
 * @brief Vector table definition.
 */
struct vector_table
{
  uint32_t *stack_end;
  rtfm::details::isr_function_pointer arm_vectors[15];
  rtfm::details::isr_function_pointer mcu_vectors[__MCU_NUM_VECTORS];
};


/**
 * @brief Initialization function of the vector table.
 *
 * @tparam T   Type of the integer_sequence.
 * @tparam Is  Integer sequence.
 */
template <typename T, T... Is>
constexpr vector_table generate_vector_table(std::integer_sequence<T, Is...>)
{
  return {
      __STACK_END,
      {
        Reset_Handler,      /* Reset Handler */
        NMI_Handler,        /* NMI Handler */
        HardFault_Handler,  /* Hard Fault Handler */
        MemManage_Handler,  /* MPU Fault Handler */
        BusFault_Handler,   /* Bus Fault Handler */
        UsageFault_Handler, /* Usage Fault Handler */
        nullptr,            /* Reserved */
        nullptr,            /* Reserved */
        nullptr,            /* Reserved */
        nullptr,            /* Reserved */
        SVCall_Handler,     /* SVCall Handler */
        DebugMon_Handler,   /* Debug Monitor Handler */
        nullptr,            /* Reserved */
        PendSV_Handler,     /* PendSV Handler */
        SysTick_Handler     /* SysTick Handler */
    },
    {
      get_vector_from_position<Is>::f::value...
    }
  };
}
