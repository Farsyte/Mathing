#pragma once

#include <ctime>

/** Measure Elapsed Time.
 */
class Timer {
  protected:

    /** Starting time in internal format. */
    clock_t t0;

  public:

    /** Construct a timer and start it running. */
    Timer()
            : t0(clock())
    {
    }

    /** Return elapsed time. */
    double elapsed_sec()
    {
        double result = clock();
        result -= t0;
        result /= CLOCKS_PER_SEC;
        return result;
    }
};
