#pragma once

#include <ostream>

/** \file
 * \brief Simple Automated Testing
 */

/** Initialize the automated testing system. */
#define EXPECT_INIT()           Expect::init(__FILE__, __LINE__, __FUNCTION__)

/** Print automated test results. */
#define EXPECT_FINI()           Expect::fini(__FILE__, __LINE__, __FUNCTION__)

/** Verify value of expression is exactly the expected value. */
#define EXPECT_EQ(obs, exp)     Expect::eq(__FILE__, __LINE__, __FUNCTION__, obs, exp, #obs)

/** Verify value of expression is not the same as the disallowed value. */
#define EXPECT_NE(obs, exp)     Expect::ne(__FILE__, __LINE__, __FUNCTION__, obs, exp, #obs)

/** Verify value of pointer expression is a null */
#define EXPECT_NULL(obs)        Expect::null(__FILE__, __LINE__, __FUNCTION__, obs, #obs)

/** Verify value of pointer expression is not a null */
#define EXPECT_NOT_NULL(obs)    Expect::not_null(__FILE__, __LINE__, __FUNCTION__, obs, #obs)

namespace Expect {

    /** Track number of test cases checked */
    extern unsigned    count;

    /** Track number of test cases failed */
    extern unsigned    fail;

    /** Remember source file name from the EXPECT_INIT() invocation. */
    extern const char *init_file;

    /** Remember source file line from the EXPECT_INIT() invocation. */
    extern int         init_line;

    /** Re-initialize Expect */
    extern void init(const char *file, int line, const char *func);

    /** Report Success.
     *
     * YAGNI but keep in mind: anticipating future expansion where the
     * file name and line number will be used for further analysis;
     * for example, if we have a condition in a loop, which fails on
     * the 3rd and 5th of 8 iterations.
     */
    extern int  report_pass(const char *file, int line,
                            const char *func);

    /** Report Failure.
     *
     * This method initiates the failure report by
     * selecting the stream (for now, stderr) and
     * printing the source file location.
     *
     * The selected stream is returned so the caller
     * can add details.
     *
     * Anticipating future expansion where the file name and
     * line number will be used for further analysis; for
     * example, if we have a condition in a loop, which fails
     * on the 3rd and 5th of 8 iterations.
     *
     * YAGNI. But make the API support it.
     *
     * Additionally, could return an ostringstream that is
     * associated with the test at this line of this file.
     */
    extern std::ostream &report_fail(const char *file, int line,
                                     const char *func);

    /** Produce results of testing.
     *
     * The file name and line number of the EXPECT_FINI()
     * invocation are available.
     *
     * Anticipating future expansion where we accumulate results
     * during testing, organized by source file and line number,
     * perhaps comparing to historical data -- with the caveat that
     * if line numbers change, how do we associate data with the
     * same test (at a different line number)?
     */
    extern int  fini(const char *file, int line, const char *func);

    /** Redirect messages to a different stream.
     */
    extern void use_stream(std::ostream &str);

    /** Verify expected compares equal to observed.
     */
    template<typename T, typename U>
    int eq(const char *file, int line, const char *func, T const &obs,
           U const &exp, std::string const &src)
    {
        if (exp == obs) {
            report_pass(file, line, func);
            return 0;
        }
        report_fail(file, line, func)
                << "expression did not have the required value"
                << "\n  expression: " << src
                << "\n  expected:   " << exp
                << "\n  observed:   " << obs << std::endl;
        return 1;
    }

    /** Verify expected compares not equal to observed.
     */
    template<typename T, typename U>
    int ne(const char *file, int line, const char *func, T const &obs,
           U const &exp, std::string const &src)
    {
        if (exp != obs) {
            report_pass(file, line, func);
            return 0;
        }
        report_fail(file, line, func)
                << "expression had the disallowed value"
                << "\n  expression: " << src
                << "\n  expected:   " << exp
                << "\n  observed:   " << obs << std::endl;
        return 1;
    }

    /** Verify observed value is a null pointer
     */
    template<typename T>
    int null(const char *file, int line, const char *func, T const &obs,
             std::string const &src)
    {
        if (!obs) {
            report_pass(file, line, func);
            return 0;
        }
        report_fail(file, line, func)
                << "expression is a null pointer"
                << "\n  expression: " << src << std::endl;
        return 1;
    }

    /** Verify observed value is not a null pointer
     */
    template<typename T>
    int not_null(const char *file, int line, const char *func,
                 T const &obs, std::string const &src)
    {
        if (obs) {
            report_pass(file, line, func);
            return 0;
        }
        report_fail(file, line, func)
                << "expression is a null pointer"
                << "\n  expression: " << src << std::endl;
        return 1;
    }
}
