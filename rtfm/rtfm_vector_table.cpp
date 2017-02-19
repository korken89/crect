
#include "rtfm/rtfm_srp.hpp"

/**
 * @brief Unhandled exception definition.
 */
extern "C" {
void _unhandled_exception(void) {
  while (true) {  }
}


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
 * @brief Used to find an ISR from a ISR position.
 *
 * @tparam ISR_POS    integral_constant containing the ISR position to find,
 */
template <typename ISR_POS>
struct find_job_isr
{
  template  <typename UID, typename Rhs>
  using compare_isrpos_to_constant =
    brigand::bool_< (UID::value == Rhs::ISR::index::value) >;

  /* Searches the resource tree for the Job with the correct ISR. */
  using type =
      brigand::find<rtfm::system_job_list,
                    brigand::bind<compare_isrpos_to_constant,
                                  ISR_POS, brigand::_1>>;
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
  using isr = typename brigand::front<JobList>::ISR;
};

/**
 * @brief Creates the Unused ISR when there is no Job matching.
 */
template <>
struct isr_selector<brigand::list<>>
{
  using isr = rtfm::MakeISR<&rtfm::DefaultISR::UnusedISR, 0>;
};

/**
 * @brief Extracts the ISR from a position.
 *
 * @tparam I   Position to extract the ISR rom.
 */
template <unsigned I>
struct get_vector_from_position
    : isr_selector<
        typename find_job_isr<brigand::integral_constant<unsigned, I>>::type
      >::isr
{
};

/**
 * @brief Vector table definition.
 */
struct vector_table
{
  const rtfm::details::ISRFunctionPointer arm_vectors[16]; /* SysTick Handler */
  const rtfm::details::ISRFunctionPointer mcu_vectors[__MCU_NUM_VECTORS];
};


/**
 * @brief Initialization function of the vector table.
 *
 * @tparam T   Type of the integer_sequence.
 * @tparam Is  Integer sequence.
 */
template <typename T, T... Is>
const constexpr vector_table generate_vector_table(
    std::integer_sequence<T, Is...>)
{
  return {{
              reinterpret_cast<rtfm::details::ISRFunctionPointer>(
                __STACK_END),     /* Stack */
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
          {get_vector_from_position<Is>::value...}};
}

/**
 * @brief Save vector table to the correct location.
 */
__attribute__((used, section(".isr_vector")))
const constexpr vector_table system_vectors = generate_vector_table(
    std::make_integer_sequence<
        unsigned, sizeof(vector_table::mcu_vectors) /
                      sizeof(rtfm::details::ISRFunctionPointer)>{});
