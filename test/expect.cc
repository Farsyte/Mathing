#include "expect.hh"

/** \file
 * \brief test the Expect facility
 *
 * This file tests the testing facility without actually
 * using the testing facility.
 */

#include <iostream>
#include <sstream>
#include <string>

using std::ostream;
using std::ostringstream;
using std::string;
using std::cerr;
using std::endl;

static int      problems = 0;
static int      conditions = 0;

static ostream &prefix(const string &file, const int line)
{
    cerr << file << ":" << line << ": ";
    return cerr;
}
#define PREFIX() prefix(__FILE__, __LINE__)

#define VERIFY_MOE(f, m, o, e)                  \
    verify_moe(__FILE__, __LINE__,              \
               eout, f, o, e, #m, #o, #e)

template<typename T, typename U>
static void verify_moe(        //
        const char    *file,   //
        int            line,   //
        ostringstream &eout,   //
        const int      f,      //
        const T       &o,      //
        const U       &e,      //
        const char     ms[],   //
        const char     os[],   //
        const char     es[])
{
    string eout_str = eout.str();
    eout.str("");

    conditions++;

    // TODO: verify that Expect bumped its test counter.

    if (f) {
        // TODO: verify that Expect bumped its fail counter.
        (void)o;
        (void)e;
        (void)ms;
        (void)os;
        (void)es;
        if (eout_str == "") {
            prefix(file, line)   //
                    << "EXPECT was silent for a FAILING test." << endl;
            problems++;
        } else {

            // TODO: verify format of the failure message (maybe)
            //
            // 1. it has to end with a newline
            // 2. the first line should be in "filename:line: message" form.
            // 3. the filename should match our file parameter
            // 4. the line should be less than our line parameter and not by too much
            //
        }
    } else {
        if (eout_str != "") {
            prefix(file, line)   //
                    << "EXPECT was not silent for a PASSING test."
                    << endl;
            problems++;
        } else {
            ;
        }
    }
}

#define VERIFY_MO(f, m, o)                      \
    verify_mo(__FILE__, __LINE__,               \
              eout, f, o, #m, #o)

template<typename T>
static void verify_mo(         //
        const char    *file,   //
        int            line,   //
        ostringstream &eout,   //
        const int      f,      //
        const T       &o,      //
        const char     ms[],   //
        const char     os[])
{
    string eout_str = eout.str();
    eout.str("");

    conditions++;

    // TODO: verify that Expect bumped its test counter.

    if (f) {
        // TODO: verify that Expect bumped its fail counter.

        // Currently, we only demand that EXPECT has made some kind
        // of output for each failing test.

        (void)o;
        (void)ms;
        (void)os;
        if (eout_str == "") {
            prefix(file, line)   //
                    << "EXPECT was silent for a FAILING test." << endl;
            problems++;
        } else {
            // TODO: verify format of the failure message (maybe)
            //
            // 1. it has to end with a newline
            // 2. the first line should be in "filename:line: message" form.
            // 3. the filename should match our file parameter
            // 4. the line should be less than our line parameter and not by too much
        }
    } else {
        if (eout_str != "") {
            prefix(file, line)   //
                    << "EXPECT was not silent for a PASSING test."
                    << endl;
            problems++;
        } else {
            ;
        }
    }
}

int main(int argc, const char *argv[])
{
    ostringstream eout;
    const char   *mynull = 0;

    (void)argc;
    (void)argv;

    {
        EXPECT_INIT();
        Expect::use_stream(eout);
        int retcode = EXPECT_FINI();
        if (retcode != 0) {
            PREFIX()   //
                    << "FINI returned " << retcode
                    << " after zero test cases,\n"
                    << "should have returned zero." << endl;
            return 1;
        }
        eout.str("");
    }

    EXPECT_INIT();
    Expect::use_stream(eout);

    // Exercise EXPECT_EQ and EXPECT_NE for integer data.

    EXPECT_EQ(argc, 1);
    VERIFY_MOE(0, EXPECT_EQ, argc, 1);

    EXPECT_EQ(argc, 2);
    VERIFY_MOE(1, EXPECT_EQ, argc, 2);

    EXPECT_NE(argc, 1);
    VERIFY_MOE(1, EXPECT_NE, argc, 1);

    EXPECT_NE(argc, 2);
    VERIFY_MOE(0, EXPECT_NE, argc, 2);

    // Exercise EXPECT_NULL and EXPECT_NOT_NULL for pointers.

    EXPECT_NULL(argv);
    VERIFY_MO(1, EXPECT_NULL, argv);

    EXPECT_NOT_NULL(argv);
    VERIFY_MO(0, EXPECT_NOT_NULL, argv);

    EXPECT_NULL(mynull);
    VERIFY_MO(0, EXPECT_NULL, mynull);

    EXPECT_NOT_NULL(mynull);
    VERIFY_MO(1, EXPECT_NOT_NULL, mynull);

    // Exercise EXPECT_EQ and EXPECT_NE for string data.

    string foo = "foo";
    string bar = "bar";

    EXPECT_EQ(foo, foo);
    VERIFY_MOE(0, EXPECT_EQ, foo, foo);

    EXPECT_NE(foo, foo);
    VERIFY_MOE(1, EXPECT_NE, foo, foo);

    EXPECT_EQ(bar, foo);
    VERIFY_MOE(1, EXPECT_EQ, bar, foo);

    EXPECT_NE(bar, foo);
    VERIFY_MOE(0, EXPECT_NE, bar, foo);

    EXPECT_EQ(foo, "foo");
    VERIFY_MOE(0, EXPECT_EQ, foo, "foo");

    EXPECT_NE(foo, "foo");
    VERIFY_MOE(1, EXPECT_NE, foo, "foo");

    EXPECT_EQ(bar, "foo");
    VERIFY_MOE(1, EXPECT_EQ, bar, "foo");

    EXPECT_NE(bar, "foo");
    VERIFY_MOE(0, EXPECT_NE, bar, "foo");

    int retcode = EXPECT_FINI();
    if (retcode == 0) {
        PREFIX()   //
                << "FINI returned " << retcode
                << " after some test cases,\n"
                << "including some cases that failed,\n"
                << "should have returned nonzero." << endl;
        problems++;
    }

    if (problems) {
        PREFIX()                        //
                << "FAILED "            //
                << problems << " of "   //
                << conditions           //
                << " test conditions." << endl;
    } else {

        PREFIX()                //
                << "passed "    //
                << conditions   //
                << " test conditions." << endl;
    }

    return problems ? 1 : 0;
}
