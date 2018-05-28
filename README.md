# crect: A C++, compile-time, reactive RTOS

**crect** (pronounced _correct_) is a C++ library for generating a
scheduler (_at compile time_) for Cortex-M series MCUs, which
**_guarantees_** dead-lock free and data-race free execution.
It utilizes the Nested Vector Interrupt Controller (NVIC) in Cortex-M
processors to implement a Stack Resource Policy (SRP) based scheduler.
Thanks to the compile time creation of the scheduler, the resource
requirements at run-time are minimal with:

**Initialization:**

* 4-5 instructions / job for initialization of the NVIC.
* 2-3 instructions / queue element for initializing the `async` queue.
* The static requirement for `async` is about 400 bytes (the linked list, SysTick and time implementation).

**Compile time:**

* Uses the Kvasir MPL library for metaprogramming.

**Runtime:**

* 3-4 instructions + 4 bytes of stack for a `lock`.
* 1-3 instructions for an (implicit) unlock.
* `claim` has zero overhead, it decomposes into a `lock`.
* 5 instructions for a `unique_lock` (will change).
* 2 instructions for a `unique_unlock` (will change).
* 2-4 instructions for `pend` / `clear`.
* About 20-30 instructions * number of items in queue for `async`.

In this scheduler, heavy use of C++ metaprogramming and C++14 allows,
among other things, priority ceilings and interrupt masks to be
automatically calculated at compile time, while resource locks are
handled through RAII and resource access is handled via a monitor
pattern. This minimizes user error without the need for an external
extra compile step, as is currently being investigated in the
[RTFM-core language](http://www.rtfm-lang.org/).

## Videos / tutorials

YouTube video from [embo++ 2018](https://www.embo.io/) describing the inner workings of **crect**: https://www.youtube.com/watch?v=SBij9W9GfBw

## Give it a test

In the `example` folder a few example projects are setup for the NUCLEO-F411RE board. For example:

* A program that will blink a LED every one seconds using `crect` primitives.
* An example for how to use `unique_lock` with a data pumping peripheral (for example DMA or a communication interface).

It also contains examples of `crect_system_config.hpp` and `crect_user_config.hpp`, providing references until a documentation is available.

If there are any questions on the usage, throw me a message.

_Tested on Ubuntu Linux using GCC 6.3.1 (arm-none-eabi) and a NUCLEO-F411RE for hardware testing. It is currently not working on Cortex-M0 devices._

## License
Boost Software License - Version 1.0

## Contributors

List of contributors in alphabetical order:

* [Emil Fresk](https://www.github.com/korken89)
* [Odin Holmes](https://www.github.com/odinthenerd)
* [Carlos van Rooijen](https://www.github.com/CvRXX)

## References

crect is based on the following academic papers:

* T.P. Baker, **"A Stack-Based Resource Allocation Policy for Realtime Processes"**, (general theory of SRP)
* Johan Eriksson et. al., **"Real-Time For the Masses, Step 1: Programming API and Static Priority SRP Kernel Primitives"**, (SRP primitives)
* Per Lindgren et. al., **"Abstract Timers and their Implementation onto the ARM Cortex-M family of MCUs"**, (async idea)

## Definitions
**Job:**

* A function that runs to completion in a finite time (no forever loop),
 not as a "normal" thread that has a forever loop.
* Has a settable priority which indicates the urgency of the Job.

**Resource:**

* An entity symbolizing something lockable, _i.e._ any locked Resource may
only be accessed by a single Job at a time.

**Lock:**

* A lock on a resource keeps other jobs, that will also take said resource,
from running through manipulation of the systems NVIC/basepri settings. A lock
can only be held within a job and must be released before the exit of a job.

---

## Usage

Small description on how to use this.

#### Resource definition
A resource definition is as follows:
```C++
using Rled = crect::make_resource<
    CRECT_OBJECT_LINK(led_resource)  // Link to some object to be protected
  >;
```
Currently 2 system resources exists:

1. The access to the `async_queue` is protected via `crect::Rasync`.
2. For getting the current time via `crect::clock::system::now()` is protected
via `crect::Rsystem_clock` - see **claim** for example usage.

Any job **using these resources** need to have the corresponding resource **in its resource claim** in `crect_user_config.hpp`.

#### Job definition
A job definition consists of a few parts:
  1. The priority of the Job, from 0 meaning low, to max_priority meaning max.
  2. An ISR the Job is connected to (peripheral ISRs, 0 is the lowest, negative numbers are the system ISRs). If it is not connected to any, take any random ISR number for now, in the future this will be automatic.
  3. The list of resources that the Job may claim.

The Job definitions are placed (directly or via include) in `crect_user_config.hpp`.
```C++
void job1(void);
using J1 = crect::job<
              1,                          // Priority (0 = low)
              crect::make_isr<job1, 1>,    // ISR connection and location
              R1, crect::Rasync            // List of possible resource claims
            >;
```
Each job need to be added to the `user_job_list< Jobs... >` in `crect_user_config.hpp`.

#### ISR definition
The ISR definitions available are split in the Peripheral ISRs (I >= 0), and System ISRs (I < 0).
```C++
// Peripheral ISR definition (I >= 0)
template <crect::details::isr_function_pointer<P, int I>
using make_isr = crect::details::isr<P, crect::details::index<I>>;

// System ISR definition (I < 0)
template <int I>
using make_system_isr = crect::details::isr<nullptr, crect::details::index<I>>;
```

#### lock
A lock keeps the system from running a job which will lock the same resource.
The analysis to determine which job can take which resource is done at
compile-time, which makes the lock very cheap to use as indicated at the start
of this document. Lock should however be **avoided** by the user, use **claim**
wherever possible.

```C++
// Lock the resource, remember locks are very cheap -- sprinkle them everywhere!
crect::lock< R1 > lock; // Locks are made in the constructor of the lock
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
crect::claim<Rled>([](auto &led){
  led.enable();
});
```

To guarantee the lock for resources with a return works just as good with `claim` (for
example when getting the current time, as the system time is a shared resource):
```C++
// Resource is handled within the claim, no risk of a data-race.
auto current_time = crect::claim<crect::Rsystem_clock>([](auto &now){
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
crect::pend<JobToPend>();
crect::clear<JobToPend>();

// Runtime dependent pend/clear
crect::pend(JobToPend_ISR_ID);
crect::clear(JobToPend_ISR_ID);
```

#### async
Async defers a jobs execution to some specific time.
```C++
// Using chrono to relate the system to time, the current max time is somewhere around 1500 years, depending on MCU :)
using namespace std::chrono_literals;

// Async in some specific duration using chrono
crect::async<JobToPend>(100ms);

// Async in some specific time using a specific time
auto time_to_execute = some_duration + crect::claim<crect::Rsystem_clock>([](auto &now){
  return now();
});
crect::async<JobToPend>(time_to_execute);

// Async can be used as pend for runtime dependent execution
crect::async(100ms, JobToPend_ISR_ID);
crect::async(time_to_execute, JobToPend_ISR_ID);
```
