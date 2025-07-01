#pragma once

#include <string>
#include <ostream>
#include <sstream>

#include "debug.hh"
#include "str.hh"

class Exception {
  public:

    Exception() { }
    virtual ~Exception() { }

    operator std::string(void) const { return str(); }

    virtual std::string str(        //
            int w = 8, int p = 3,   //
            std::ios_base::fmtflags f = std::ios_base::fixed) const = 0;
};

extern std::ostream &operator<<(    //
        std::ostream    &s,         //
        Exception const &e);

extern std::string   str(           //
        const Exception &e,       //
        int              w = 8,   //
        int p = 3,                //
        std::ios_base::fmtflags f = std::ios_base::fixed);

class IndexRange : public Exception {
  private:

    IndexRange(IndexRange const &e);
    IndexRange &operator=(IndexRange const &that);

  public:

    const int ix;
    const int lo;
    const int hi;

  public:

    IndexRange(int ix, int lo, int hi)
            : ix(ix)
            , lo(lo)
            , hi(hi)
    {
    }

    virtual std::string str(        //
            int w = 8, int p = 3,   //
            std::ios_base::fmtflags f = std::ios_base::fixed) const;

    static int          check(int ix, int lo, int hi)
    {
        if ((ix < lo) || (ix > hi)) {
            throw IndexRange(ix, lo, hi);
        }
        return 0;
    }
};

class RangeValue : public Exception {
  private:

    RangeValue &operator=(RangeValue const &that);
    RangeValue(RangeValue const &e);

  public:

    const double lo;
    const double hi;
    const double val;

  public:

    RangeValue(                 //
            const double lo,    //
            const double hi,    //
            const double val)   //
            : lo(lo)
            , hi(hi)
            , val(val)
    {
    }

    virtual std::string str(             //
            int w = 8, int p = 3,        //
            std::ios_base::fmtflags f = std::ios_base::fixed) const;

    static int          check(           //
            const double lo,    //
            const double hi,    //
            const double val)   //
    {
        if ((val < lo) || (val > hi)) {
            throw RangeValue(lo, hi, val);
        }
        return 0;
    }
};

class Permission : public Exception {
  public:

    virtual std::string str(        //
            int w = 8, int p = 3,   //
            std::ios_base::fmtflags f = std::ios_base::fixed) const;
};
