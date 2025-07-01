#pragma once

#include <ctime>

/** Measure Elapsed Time.
 */
class Timer {
  public:

    clock_t t0;

    Timer()
            : t0(clock())
    {
    }

    void   restart() { t0 = clock(); }

    double elapsed_sec()
    {
        double result = clock();
        result -= t0;
        result /= CLOCKS_PER_SEC;
        return result;
    }

    ~Timer() { }
};
