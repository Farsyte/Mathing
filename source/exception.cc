#include "exception.hh"

Exception::Exception() { }
IndexRange::IndexRange(IndexRange const &e)
        : Exception()
        , ix(e.ix)
        , lo(e.lo)
        , hi(e.hi)
{
}

IndexRange::IndexRange(int i, int l, int h)
        : ix(i)
        , lo(l)
        , hi(h)
{
}

std::string IndexRange::str(void) const
{
    std::ostringstream os;
    os << "IndexRange value " << ix
       << ((lo > hi) ? " outside (empty!) inclusive range "
                     : " outside inclusive range ")
       << "(" << lo << " .. " << hi << ")" << std::endl;
    return os.str();
}
