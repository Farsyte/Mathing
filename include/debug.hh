#pragma once

/** \file
 * \brief common debugging macros
 *
 * This file provides a collection of macros that produce output
 * tagged with source file name and line number.
 *
 * If the IS_REL preprocessor macro is defined with a nonzero value,
 * all work is removed from all macros in this file.
 *
 * Otherwise, this file produces macros that, when used, assume
 * that the iostream header has been included and various symbols
 * from that header have been pulled into the current scope.
 */

#if NDEBUG

#define DEBUG_DISABLE()			do { ; } while (0)
#define DEBUG_ENABLE()			do { ; } while (0)
#define DEBUG_MESSAGE(msg)		do { ; } while (0)
#define DEBUG_VALUE(expr)		do { ; } while (0)

#else

namespace Debug {
    /** Nestable "debug disabled" counter. */
    extern int disabled;
}

/** Start a "debug disabled" block.
 *
 * Within a debug disabled block, debug macros do no work.
 *
 * These blocks nest. Leaving an inner block will not turn
 * the macros back on, if there is an outer block active.
 */
#define DEBUG_DISABLE()                                         \
    do {                                                        \
        Debug::disabled ++;                                     \
    } while (0)

/** End a "debug disabled" block, which might nest.
 *
 * Within a debug disabled block, debug macros do no work.
 *
 * These blocks nest. Leaving an inner block will not turn
 * the macros back on, if there is an outer block active.
 */
#define DEBUG_ENABLE()                                          \
    do {                                                        \
        Debug::disabled --;                                     \
    } while (0)

/** Produce a debug message.
 *
 * All debug messages are sent to the standard output, start
 * with the source file name and line number, and end with
 * a newline (and a stream flush).
 *
 * The parameter is allowed to use "<<" operators to send multiple
 * parts of the message to the stream; consequently, if the message
 * uses operators with lower precedence than "<<", the caller is
 * expected to parenthesize appropriately.
 */
#define DEBUG_MESSAGE(msg)                                      \
    do {                                                        \
        if (Debug::disabled > 0) break;                         \
        std::cout << __FILE__ << ":" << __LINE__ << ": "        \
                  << msg << std::endl;                          \
    } while (0)

/** Debug message for an expression value.
 *
 * The output includes the source string for the expression and
 * the result of evaluating it.
 */
#define DEBUG_VALUE(expr)                                       \
    DEBUG_MESSAGE(#expr << ": " << (expr))

#endif   // NDEBUG
