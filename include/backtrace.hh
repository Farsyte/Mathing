#pragma once

/** \file
 * \brief Try to print a backtrace.
 *
 * This function attempts to print information about each currently
 * active stack frame, which if all goes well, means source file and
 * line numbers of the call sites. Success is highly variable based
 * on host operating systems, compiler, and compiler options.
 */
extern void print_backtrace();
