#include "exception.hh"
#include "str.hh"

std::string str(                     //
        const Exception        &e,   //
        int                     w,   //
        int                     p,   //
        std::ios_base::fmtflags f)
{
    return e.str(w, p, f);
}

std::ostream &operator<<(     //
        std::ostream    &s,   //
        Exception const &e)
{
    // NOTE: not passing along formatting from s,
    // or "cout << e" looks bad.
    return s << str(e);
}

std::string IndexRange::str(         //
        int                     w,   //
        int                     p,   //
        std::ios_base::fmtflags f)   //
        const
{
    std::ostringstream os;
    os << "IndexRange exception: value " << ix
       << ((lo > hi)                                        //
                   ? " outside (empty!) inclusive range "   //
                   : " outside inclusive range ")           //
       << "(" << lo << " .. " << hi << ")";

    (void)w;                                                // ignored, or "cout << e" looks bad.
    (void)p;                                                // ignored, or "cout << e" looks bad.
    (void)f;                                                // ignored, or "cout << e" looks bad.

    return os.str();
}

std::string ValueRange::str(         //
        int                     w,   //
        int                     p,   //
        std::ios_base::fmtflags f)   //
        const
{
    std::ostringstream os;

    os << "ValueRange exception: "
       << "Value not within" << ((lo <= hi) ? "" : " inclusive") << ((lo != hi) ? "" : " degenerate")
       << ((lo >= hi) ? "" : " empty") << " range."
       << "\n  observed: " << val   //
       << "\n  minimum:  " << lo    //
       << "\n  maximum:  " << hi;

    (void)w;                        // ignored, or "cout << e" looks bad.
    (void)p;                        // ignored, or "cout << e" looks bad.
    (void)f;                        // ignored, or "cout << e" looks bad.

    return os.str();
}
