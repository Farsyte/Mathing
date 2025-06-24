#pragma once

#include <iomanip>
#include <sstream>

/** \file
 * \brief Conversion of values to strings
 *
 * This header assums that "headers.hh" has been included.
 */

/** Convert an object of type T to a string.
 *
 * The classes defined in this project provide a "put" method
 * that formats the object onto the given stream, and returns
 * the streawm for further processing.
 *
 * This str(T) method applies that API to a string stream, and
 * returns the resulting string.
 *
 * The 2nd parameter, if provided, specifies a minimum width
 * for output of data. In the general case, how this impacts the
 * result is under the control of the put() method in T. If not
 * specified, a width of 8 is used.
 *
 * If the 2nd parameter is positive, the output is RIGHT justified
 * within a field of that many characters. If it is negative, then
 * the output is LEFT justified within the field.
 *
 * The 3rd parameter, if provided, specifies a minimum precision
 * for output of data. In the general case, how this impacts the
 * result is under the control of the put() method in T. If not
 * specified, a precision of 3 is used.
 *
 * The 4th parameter, if provided, specifies the boolean flags
 * to be presented to the stream formatter.
 */
template<typename T>
inline std::string str(   //
        T const &e, int w = 8, int p = 3, std::ios_base::fmtflags f = std::ios_base::fixed)
{
    std::ostringstream ss;

    ss << std::setiosflags(f);

    // If the caller did not specify scientific notation,
    // explicitly select output in fixed notation.
    if (!(ss.flags() & std::ios_base::scientific)) {
        ss << std::fixed;
    }

    // iostream can't left justify numeric data,
    // so I'm doing the padding manually.
    ss << std::setprecision(p);
    ss << e;
    std::string s = ss.str();

    if (w < 0) {
        unsigned u = -w;
        while (s.length() < u)
            s = s + " ";
    }
    if (w > 0) {
        unsigned u = w;
        while (s.length() < u)
            s = " " + s;
    }
    return s;
}

/** str() but set formatting options from another stream.
 *
 * This template should be the only place where we interrogate
 * a stream for its formatting flags.
 */
template<typename T>
inline std::string str(   //
        T const &e, std::ostream const &os)
{
    int                     w = os.width();
    int                     p = os.precision();
    std::ios_base::fmtflags f = os.flags();

    if (f & std::ios_base::left)
        w = -w;
    return str(e, w, p, f);
}
