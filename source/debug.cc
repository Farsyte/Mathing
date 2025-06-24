#include "debug.hh"

/** \file
 * \brief Debug support source code
 */

/** Debug Message Disable Counter.
 *
 * If this counter is nonzero, the debug macros will terminate
 * without doing work.
 *
 * NOTE: this is compiled into the library whether NDEBUG is defined
 * or not, as NDEBUG can be defined (or not) independently during
 * compilation of each source file.
 */

namespace Debug {

    /** Nestable "debug disabled" counter. */
    int disabled = 0;

}
