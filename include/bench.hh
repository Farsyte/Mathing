#pragma once

#include <iostream>

#include "timer.hh"

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

    Timer             t;

    double            dt_sec;

    double            ns_each;

    void              log_result(                 //
            std::string const &op,   //
            double             nsea);

  public:

    static bool log_disable;
    static void enable();

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
            , t()
            , dt_sec(-1)
            , ns_each(-1)
    {
        restart();
    }

    void restart()
    {
        t.restart();
        dt_sec = -1;
    }

    double elapsed_sec()
    {
        if (dt_sec < 0) {
            dt_sec = t.elapsed_sec();
        }
        return dt_sec;
    }

    double nsea()
    {
        if (ns_each < 0) {
            ns_each = elapsed_sec() * 1.0e9 / nr;
        }
        return ns_each;
    }

    ~Bench() { log_result(op, nsea()); }
};
