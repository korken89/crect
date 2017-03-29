# RTFM++: A proof-of-concept C++ implementation of the Stack Resource Policy (SRP) based Real-Time For the Masses (RTFM) kernel

**RTFM++** is a proof-of-concept C++ library for generating a compile time
scheduler for Cortex-M series MCUs, which guarantees dead-lock free execution, based on the papers by:

* T.P. Baker, **"A Stack-Based Resource Allocation Policy for Realtime Processes"**, (general theory of SRP)
* Johan Eriksson et. al., **"Real-Time For the Masses, Step 1: Programming API and Static Priority SRP Kernel Primitives"**, (SRP primitives, the RTFM name)
* Per Lindgren et. al., **"Abstract Timers and their Implementation onto the ARM Cortex-M family of MCUs"**, (async idea)

which utilizes the Nested Vector Interrupt Controller (NVIC) in Cortex-M
processors to implement a Stack Resource Policy (SRP) based scheduler. Thanks
to the compile time creation of the scheduler, the resource requirements at
run-time are minimal with:

**Initialization:**

* 4-5 instructions / job for initialization of the NVIC.
* 2-3 instructions / queue element for initializing the `async` queue.
* The static requirement for `async` is about 400 bytes (the linked list, SysTick and time implementation).

**Compile time:**
* Uses the Kvasir MPL library for Metaprogramming.

**Runtime:**

* 3-4 instructions + 4 bytes of stack for a `lock`.
* 1-3 instructions for an (implicit) unlock.
* `claim` has zero overhead, it decomposes into a `lock`.
* 5 instructions for a `unique_lock`.
* 2 instructions for a `unique_unlock`.
* 2-4 instructions for `pend` / `clear`.
* About 20-30 instructions * number of items in queue for `async`.

In this implementation of RTFM, heavy use of **C++ metaprogramming** and **C++14** allows, among other things, priority ceilings and interrupt masks to be automatically calculated at compile time, while resource locks are handled through RAII and resource access is handled via a monitor pattern. This minimizes user error without the need for an external extra compile step, as is currently being investigated in the RTFM-core language (www.rtfm-lang.org).

More description will come...

## Give it a test

In `./example` an example project is setup for the NUCLEO-F411RE board, that will blink a LED every one seconds.
It also contains an example of `rtfm_system_config.hpp` and `rtfm_user_config.hpp`, providing examples.

If there are any questions on the usage, throw me a message.

_Tested on Manjaro Linux using GCC 6.3.0 (arm-none-eabi) and a NUCLEO-F411RE for hardware testing. It is currently not working on Cortex-M0 devices._

## Future name suggestions
* ComTiS (Compile Time Scheduler)
* CReactive (Compile-time Reactive Scheduler)
* TRust++ (Task / Resource, pointing to the security of the system)
* SyRuP (SRP)

_Winner gets a cookie!_

## Definitions
**Job:**

* A function that runs to completion in a finite time (no forever loop),
 not as a "normal" thread that has a forever loop.
* Has a settable priority which indicates the urgency of the Job.

**Resource:**

* An entity symbolizing something lockable, _i.e._ any locked Resource may
only be accessed by a single Job at a time.

**Lock (Stack Resource Policy):**

* A lock on a resource keeps other jobs, that will also take said resource,
from running through manipulation of the systems NVIC/basepri settings. A lock
can only be held within a job and must be released before the exit of a job.

---

### TODO

- [ ] Cortex-M0 support, does not have `basepri` - will have to use interrupt masking.
- [ ] Add a debug mode for `lock` / `claim` / `unique_lock`, use `IPSR` to check that the ISR number is allowed to take the resource.
- [ ] Make the NVIC init merged into minimal set of operations, not one operation per job.

---

## Usage

Small description on how to use this.

#### Resource definition
A resource definition is as follows, where `ManageLED` is a type that symbolizes the resource.
```C++
using Rled = rtfm::make_resource<
    ManageLED,                      // Resource unique ID
    RTFM_OBJECT_LINK(led_resource)  // Link to some object to be protected
  >;
```
Currently 2 system resources exists:

1. The access to the `async_queue` is protected via `rtfm::Rasync`.
2. For getting the current time via `rtfm::clock::system::now()` is protected
via `rtfm::Rsystem_clock` - see **claim** for example usage.

Any job **using these resources** need to have the corresponding resource **in its resource claim** in `rtfm_user_config.hpp`.

#### Job definition
A job definition consists of a few parts:
  1. A unique ID, used for static checking.
  2. The priority of the Job, from 0 meaning low, to max_priority meaning max.
  3. An ISR the Job is connected to (peripheral ISRs, 0 is the lowest, negative numbers are the system ISRs). If it is not connected to any, take any random ISR number for now, in the future this will be automatic.
  4. The list of resources that the Job may claim.

The Job definitions are placed (directly or via include) in `rtfm_user_config.hpp`.
```C++
void job1(void);
using J1 = rtfm::job<
              rtfm::util::hashit("Job1"), // Unique ID (here through a hash of text)
              1,                          // Priority (0 = low)
              rtfm::MakeISR<job1, 1>,     // ISR connection and location
              R1, rtfm::Rasync            // List of possible resource claims
            >;
```
Each job need to be added to the `system_job_list< Jobs... >` in `rtfm_user_config.hpp`.

#### ISR definition
The ISR definitions available are split in the Peripheral ISRs (I >= 0), and System ISRs (I < 0).
```C++
// Peripheral ISR definition (I >= 0)
template <rtfm::details::isr_function_pointer<P, int I>
using MakeISR = rtfm::details::isr<P, rtfm::details::index<I>>;

// System ISR definition (I < 0)
template <int I>
using MakeSystemISR = rtfm::details::isr<nullptr, rtfm::details::index<I>>;
```

#### lock
A lock keeps the system from running a job which will lock the same resource.
The analysis to determine which job can take which resource is done at
compile-time, which makes the lock very cheap to use as indicated at the start
of this document. Lock should however be **avoided** by the user, use **claim**
wherever possible.

```C++
// Lock the resource, remember locks are very cheap -- sprinkle them everywhere!
rtfm::srp::lock< R1 > lock; // Locks are made in the constructor of the lock
// ...
// Unlock is automatic in the destructor of lock
```

There is no `unlock`, this is by design.

#### claim
Even with **lock**, it is easy to leak a resource, and to minimize this chance
`claim` uses a _Monitor Pattern_ to guard the resource. Hence the resource is
only available within the lambda of `claim`:
```C++
// Access the LED resource through the claim following a monitor pattern (just as cheap as a lock)
rtfm::srp::claim<Rled>([](auto &led){
  led.enable();
});
```

To guarantee the lock for resources with a return works just as good with `claim` (for
example when getting the current time, as the system time is a shared resource):
```C++
// Resource is handled within the claim, no risk of a data-race.
auto current_time = rtfm::srp::claim<rtfm::Rsystem_clock>([](auto &now){
  return now(); // now is a function reference
});
```

For a full example please see `./examples/blinky`.

#### unique_lock
Add a unique resource and its corresponding lock is to support lock over job
boundaries. This allows for
producer/consumer patterns as the unique job can work by reading a queue of
actions. _Ex._ a DMA, where jobs push to a "DMA transfer queue",
and the job which has the unique DMA resource will read this queue and
perform the desired transactions.

A `unique_lock` effectively decomposes into disabling the interrupt vector of
the job, while the `unique_unlock` re-enables the interrupt vector.

For a full example please see `./examples/unique`.

```C++
```

There is no `unlock`, this is by design.
#### pend / clear
`pend` directly sets a job for execution and will be as soon as its priority is the highest, while `clear` removes the job for execution.
```C++
// Compile time constant pend/clear
rtfm::srp::pend<JobToPend>();
rtfm::srp::clear<JobToPend>();

// Runtime dependent pend/clear
rtfm::srp::pend(JobToPend_ISR_ID);
rtfm::srp::clear(JobToPend_ISR_ID);
```

#### async
Async defers a jobs execution to some specific time.
```C++
// Using chrono to relate the system to time, the current max time is somewhere around 1500 years, depending on MCU :)
using namespace std::chrono_literals;

// Async in some specific duration using chrono
rtfm::srp::async<JobToPend>(100ms);

// Async in some specific time using a specific time
auto time_to_execute = some_duration + rtfm::srp::claim<rtfm::Rsystem_clock>([](auto &now){
  return now();
});
rtfm::srp::async<JobToPend>(time_to_execute);

// Async can be used as pend for runtime dependent execution
rtfm::srp::async(100ms, JobToPend_ISR_ID);
rtfm::srp::async(time_to_execute, JobToPend_ISR_ID);
```
