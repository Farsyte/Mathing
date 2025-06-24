#include "pin.hh"

#include <iostream>

#include <cerrno>
#include <cstring>

#include <sched.h>

void pin()
{
    static int called = 0;
    if (called)
        return;
    called = 1;

    int       target_cpu = 0;   // sched_getcpu();

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(target_cpu, &mask);

    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) < 0) {
        int oops = errno;
        std::cerr << "pin: Unable to set CPU affinity; sched_setaffinity indicates error "   //
                  << oops << ":\n  " << strerror(oops) << std::endl;
    } else {
        std::cerr << "pin: set affinity to cpu " << target_cpu << std::endl;
    }
}
