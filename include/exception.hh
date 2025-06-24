#pragma once

#include <string>
#include <ostream>
#include <sstream>

#include "debug.hh"

class Exception {
  public:

    Exception();
    virtual ~Exception() { }
    virtual std::string str() const = 0;
    operator std::string(void) const { return str(); }
};

inline std::string   str(Exception const &e) { return e.str(); }

inline std::ostream &operator<<(std::ostream &s, Exception const &e)
{
    return s << str(e);
}

class IndexRange : public Exception {
  public:

    const int ix;
    const int lo;
    const int hi;

  public:

    IndexRange(IndexRange const &e);

    IndexRange(int i, int l, int h);

    virtual std::string str(void) const;

    static void         check(int ix, int lo, int hi)
    {
        if ((ix < lo) || (ix > hi)) {
            throw IndexRange(ix, lo, hi);
        }
    }
};

class Permission : public Exception {
  public:

    virtual std::string str(void) const
    {
        return "Associated storage is Read-Only.";
    }
};
