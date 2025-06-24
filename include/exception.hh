#pragma once

#include <string>
#include <ostream>
#include <sstream>

#include "debug.hh"
#include "str.hh"

/** Base Class for Exceptions
 *
 * This class can not be instantiated.
 */
class Exception {
  public:

    /** Default Constructor does nothing. */
    Exception() { }

    /** Virtual Destructor does nothing. */
    virtual ~Exception() { }

    /** Provide operator to convert these into a string. */
    operator std::string(void) const { return str(); }

    /** Require derived classes to provide a str(w,p,f) function.
     *
     * See "str.hh" for the rest of this mechanism.
     */
    virtual std::string str(            //
            int w = 8,                  /**< width for formatted output data */
            int p = 3,                  /**< precision for formatted output data */
            std::ios_base::fmtflags f = /**< other stream flags */
            std::ios_base::fixed) const = 0;
};

/** Support printing an exception onto an output stream. */
extern std::ostream &operator<<(std::ostream &s, Exception const &e);

/** Support applying str() to exceptions */
extern std::string   str(                             //
        const Exception &e, int w = 8, int p = 3,   //
        std::ios_base::fmtflags f = std::ios_base::fixed);

/** IndexRange exception includes the range checker.
 *
 * The exception object records the index value and the
 * allowed range. It does not record the source location
 * where the range is checked -- the problem will be in
 * code upward in the stack.
 *
 * Code that catches and reports these exceptions will probably want
 * to print the backtrace, and probably also do something to
 * breakpoint any debugger being used.
 */
class IndexRange : public Exception {
  private:

    /** Copy Constructor does not exist. */
    IndexRange(IndexRange const &e);
    /** Assignment Operator does not exist. */
    IndexRange &operator=(IndexRange const &that);

  public:

    const int ix; /**< value of the index checked */
    const int lo; /**< inclusive lower limit for index */
    const int hi; /**< inclusive upper limit for index */

  public:

    /** Capture information useful when reporting the range error. */
    IndexRange(int ix, int lo, int hi)
            : ix(ix)
            , lo(lo)
            , hi(hi)
    {
    }

    /** Implements .str(w,p,f) for formatting, but not inline. */
    virtual std::string str(        //
            int w = 8, int p = 3,   //
            std::ios_base::fmtflags f = std::ios_base::fixed) const;

    /** Index Range Checker.
     *
     * Performs the range check; if it fails, constructs a new
     * IndexRange object and throws it.
     */
    static int          check(int ix, int lo, int hi)
    {
        if ((ix < lo) || (ix > hi)) {
            throw IndexRange(ix, lo, hi);
        }
        return 0;
    }
};

/** ValueRange exception includes the range checker.
 *
 * The exception object records a floating point value and its allowed
 * range. It does not record the source location where the range is
 * checked -- the problem will be in code upward in the stack.
 *
 * Code that catches and reports these exceptions will probably want
 * to print the backtrace, and probably also do something to
 * breakpoint any debugger being used.
 */
class ValueRange : public Exception {
  private:

    /** Copy Constructor does not exist. */
    ValueRange &operator=(ValueRange const &that);
    /** Assignment Operator does not exist. */
    ValueRange(ValueRange const &e);

  public:

    const double val; /**< value that was range checked */
    const double lo;  /**< inclusive lower limit for index */
    const double hi;  /**< inclusive upper limit for index */

  public:

    /** Capture information useful when reporting the range error. */
    ValueRange(const double val, const double lo, const double hi)
            : val(val)
            , lo(lo)
            , hi(hi)
    {
    }

    /** Implements .str(w,p,f) for formatting, but not inline. */
    virtual std::string str(        //
            int w = 8, int p = 3,   //
            std::ios_base::fmtflags f = std::ios_base::fixed) const;

    /** Index Range Checker.
     *
     * Performs the range check; if it fails, constructs a new
     * IndexRange object and throws it.
     */
    static int          check(const double val, const double lo, const double hi)
    {
        if ((val < lo) || (val > hi)) {
            throw ValueRange(val, lo, hi);
        }
        return 0;
    }
};
