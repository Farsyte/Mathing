#pragma once

#include <iostream>

/** Query an elapsed time timer.
 *
 * This function returns a value representing the number
 * of microseconds elapsed since some unspecified epoch.
 */
class Bench {
  private:

    std::ostream     &os;
    const std::string op;

    double            nr;

    clock_t           t0;
    clock_t           tF;
    clock_t           dt;

    double            ns_each;

    static void       log_result(          //
            std::string const &op,   //
            double             nsea);

  public:

    static void dump_log(            //
            std::ostream      &os,   //
            std::string const &tblname);

    Bench(std::ostream     &os,      //
          const int         N,       //
          const int         R,       //
          const std::string op)
            : os(os)
            , op(op)
            , nr((double)N * (double)R)

            , t0(-1)
            , tF(-1)
            , dt(-1)

            , ns_each(-1)
    {
        restart();
    }

    void restart()
    {
        tF = -1;
        dt = -1;
        t0 = clock();
    }

    clock_t elapsed()
    {
        if (dt < 0) {
            tF = clock();
            dt = tF - t0;
        }
        return dt;
    }

    double nsea()
    {
        if (ns_each < 0) {
            ns_each = elapsed() * 1.0e9 / (CLOCKS_PER_SEC * nr);
        }
        return ns_each;
    }

    ~Bench() { log_result(op, nsea()); }
};
