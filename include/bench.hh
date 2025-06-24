#pragma once

#include <iostream>

#include "timer.hh"
#include "pin.hh"

/** Query an elapsed time timer.
 *
 * This function returns a value representing the number
 * of microseconds elapsed since some unspecified epoch.
 */
class Bench {
  private:

    const std::string op;      /**< operation being benchmarked */
    double            nr;      /**< N*R product for analysis */
    Timer             t;       /**< timer for measuring elapsed time */
    double            dt_sec;  /**< measured elapsed time in seconds */
    double            ns_each; /**< nanoseconds per operation */
    bool              logged;  /**< flag set when results have been logged. */

  public:

    static bool disabled; /**< prevents recording results */

    /** Enable Results Collection.
     *
     * Initially, collection of benchmark results is disabled to avoid
     * including "cold cache" data when working out the steady state
     * behavior of the code being benchmarked.
     *
     * Call Bench::enable() to enable collection of results for all
     * benchmark cases that terminate in the future.
     */
    static void enable();

    /** Disable Results Collection.
     *
     * Call this function before resuming benchmarking after a major
     * change in the benchmarking context, to avoid recording results
     * with cold cache effects.
     */
    static void disable();

    /** Present benchmark results to the configured output stream.
     *
     * This function should be called at the end of the benchmark
     * after running the test suite, possibly many times. This will
     * generate a Python source block for an EMACS ORG document that
     * will append the data to a table, for use in "benchmarks.org"
     * found in the "doc" directory of this project.
     */
    static void dump_log(std::ostream &os);

    /** Start a benchmark case.
     *
     * The results of a benchmark are in "nanoseconds per operation"
     * where the notion of "operation" is defined by the caller in a
     * way that makes sense to that operation. Typically, the caller
     * will be performing R repetitions of a sequence of N operations.
     *
     * Construction of this operation starts the timer, which runs
     * until the destructor is called.
     */
    Bench(const int N, const int R, const std::string op)
            : op(op)
            , nr((double)N * (double)R)
            , t()
            , dt_sec(-1)
            , ns_each(-1)
            , logged(false)
    {
        pin();
        restart();
    }

    /** Start (or restart) the timer.
     *
     * Establish the benchmark starting time. It is automatically
     * called during construction, but can be called again if the
     * caller has other work to do between construction of the Bench
     * and starting the code to be benchmarked.
     */
    void restart()
    {
        ns_each = -1;
        dt_sec = -1;
        t = Timer();
    }

    /** Record the elapsed time.
     *
     * Establish the end time of the benchmark, if it has not already
     * been established, and return the elapsed time in seconds.
     *
     * This function is automatically called during object
     * destruction. This is used during object destruction but can be
     * called earlier if the caller wants to record the value.
     */
    double elapsed_sec()
    {
        if (dt_sec < 0) {
            dt_sec = t.elapsed_sec();
        }
        return dt_sec;
    }

    /** Compute nanoseconds per operation.
     *
     * Returns the elapsed nanoseconds per operation, which is computed
     * during the first nsea() call based on data from elapsed_sec(). This
     * is used during object destruction but can be called earlier if
     * the caller wants to record the value.
     */
    double nsea()
    {
        if (ns_each < 0) {
            ns_each = elapsed_sec() * 1.0e9 / nr;
        }
        return ns_each;
    }

    /** Log benchmark results to the configured stream.
     *
     * This function records the results of this benchmark to
     * static storage so they can be included in the final
     * table of results that will be dumped after the whole
     * benchmark suite has been run (possibly many times).
     *
     * This will be called during the destructor, but can be called
     * earlier if desired. Only the first call will record data to
     * static storage.
     */
    void log_result();

    /** Destructor logs the results to static storage.
     */
    ~Bench() { log_result(); }
};
