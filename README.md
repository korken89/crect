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
* The static requirement for `async` is about 500 bytes (the linked list implementation).

**Runtime:**

* 4-6 instructions + 4 bytes of stack for a `lock`.
* 1-3 instructions for an unlock.
* 2-4 instructions for `pend` / `clear`.
* About 20-30 instructions * number of items in queue for `async`.

In this implementation of RTFM, heavy use of **C++ metaprogramming** and **C++14** allows, among other things, priority ceilings and interrupt masks to be automatically calculated at compile time, while resource locks are handled through RAII. This minimizes user error without the need for an external extra compile step, as is currently being investigated in the RTFM-core language (www.rtfm-lang.org).

More description will come...


## Usage

Small description on how to use this.

#### Resource definition
A resource definition is simple as follows, where `some_type` is a type that symbolizes the resource.
```C++
using R1 = rtfm::Resource<some_type>;
```

#### Job definition
A job definition consists of a few parts:
  1. A unique ID.
  2. The priority of the Job, from 0 meaning low, to max_priority meaning max.
  3. An ISR the Job is connected to (peripheral ISRs, 0 is the lowest, negative numbers are the system ISRs). If it is not connected to any, take any random ISR number for now.
  4. The list of resources that the Job may claim.
```C++
void job1(void);
using J1 = rtfm::Job<
              rtfm::util::hashit("Job1"), // Unique ID
              1,                          // Priority (0 = low)
              rtfm::MakeISR<job1, 1>,     // ISR connection and location
              R1, Rasync                  // List of possible resouce claims
            >;
```

#### ISR definition
```C++
// Peripheral ISR definition (I >= 0)
template <rtfm::details::ISRFunctionPointer P, int I>
using MakeISR = rtfm::details::ISR<P, details::Index<I>>;

// System ISR definition (I < 0)
template <int I>
using MakeSystemISR = rtfm::details::ISR<nullptr, rtfm::details::Index<I>>;
```

#### pend
Pend directly set a job for execution and will be as soon as it's priority is the highest.
```C++
// Compile time constant pend
rtfm::srp::pend<JobToPend>();

// Runtime dependent pend
rtfm::srp::pend(JobToPend_ISR_ID);
```

#### async
Async defers a jobs execution to some specific time.
```C++
// Using chrono to relate the system to time, the current max time is somewhere around 1500 years, depending on MCU :)
using namespace std::chrono_literals;

// Async in some specific duration using chrono
rtfm::srp::async<JobToPend>(100ms);

// Async in some specific time using a specific time
auto time_to_execute = rtfm::time::system_clock::now() + some_duration;
rtfm::srp::async<JobToPend>(time_to_execute);

// Async can be used as pend for runtime dependent execution
rtfm::srp::async(100ms, JobToPend_ISR_ID);
rtfm::srp::async(time_to_execute, JobToPend_ISR_ID);
```

## Definitions
**Job:**

* A function that runs to completion in a finite time (no forever loop),
 not as a "normal" thread that has a forever loop.
* Has a settable priority which indicates the urgency of the Job.

**Resource:**

* An entity symbolizing something lockable, i.e. any locked Resource may
only be accessed by a single Job at a time.

**Lock (Stack Resource Policy):**

* A lock on a resource keeps other jobs, that will also take said resource,
from running through manipulation of the systems ISR settings. A lock can only
be held within a job and must be released before the exit of a job.
