# RTFM++: A C++ implementation of the Stack Resource Policy (SRP) based Real-Time For the Masses (RTFM) kernel

**RTFM++** is a C++ library for generating a compile time
scheduler for Cortex-M series MCUs, which guarantees dead-lock free execution, based on the papers by:

* T.P. Baker, **"A Stack-Based Resource Allocation Policy for Realtime Processes"**, (general theory of SRP)
* Johan Eriksson et. al., **"Real-Time For the Masses, Step 1: Programming API and Static Priority SRP Kernel Primitives"**, (SRP primitives)
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

### Contributors

* Emil Fresk

_Contributors are most welcome! Contact me and we can have a chat._

## Usage



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
