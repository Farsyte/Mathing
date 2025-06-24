#pragma once

/** \file
 * \brief One-Stop-Shop for Common Headers.
 *
 * This file has include lines for all of the header files defined in
 *     INCITS ISO/IEC 14882:2003 "The C++ Programming Language"
 * except for strstream, which is explicitly deprecated.
 * This includes the ones that provide access to the C Standard Library.
 *
 * The lines are initially commented out, and will be activated as headers
 * are needed by this project.
 *
 * This file also has include lines for project headers that are
 * expected to be needed in every source file. Define "HEADERS_STD_ONLY"
 * in your source file if you want to avoid including these headers.
 */

// #include <algorithm>
// #include <bitset>
// #include <complex>
// #include <deque>
// #include <exception>
// #include <fstream>
// #include <functional>
#include <iomanip>   // setw, ...
// #include <ios>
// #include <iosfwd>
#include <iostream>   // cerr, ...
// #include <istream>
// #include <iterator>
// #include <limits>
// #include <list>
// #include <locale>
// #include <map>
// #include <memory>
// #include <new>
// #include <numeric>
#include <ostream>   // ostream, ...
// #include <queue>
// #include <set>
#include <sstream>   // ostringstream, ...
// #include <stack>
// #include <stdexcept>
// #include <streambuf>
#include <string>   // string, ...
// #include <typeinfo>
// #include <utility>
// #include <valarray>
// #include <vector>

// #include <cassert>
// #include <cctype>
// #include <cerrno>
// #include <cfloat>
// #include <ciso646>
// #include <climits>
// #include <clocale>
// #include <cmath>
// #include <csetjmp>
// #include <csignal>
// #include <cstdarg>
// #include <cstddef>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
#include <ctime>   // clock_t, clock(), ...
// #include <cwchar>
// #include <cwctype>

#ifndef HEADERS_STD_ONLY

/*
 * Include addtional headers that are expected to be needed by a
 * substantial fraction of project sources.
 */
#include "debug.hh"
#include "tostring.hh"
#include "expect.hh"
#include "bench.hh"

#endif
