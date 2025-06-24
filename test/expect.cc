#include "headers.hh"

using std::string;
using std::ostream;
using std::ostringstream;
using std::cerr;
using std::endl;

/** \file
 * \brief test the Expect facility
 *
 * This file tests the testing facility without actually
 * using the testing facility.
 */

static int      problems = 0;
static int      conditions = 0;

static ostream &prefix(const string &file, const int line)
{
    cerr << file << ":" << line << ": ";
    return cerr;
}
#define PREFIX() prefix(__FILE__, __LINE__)

#define VERIFY_MEO(f, m, e, o)                  \
    verify_meo(__FILE__, __LINE__,              \
               eout, f, e, o, #m, #e, #o)

template<typename T, typename U>
static void verify_meo(        //
        const char    *file,   //
        int            line,   //
        ostringstream &eout,   //
        const int      f,      //
        const T       &e,      //
        const U       &o,      //
        const char     ms[],   //
        const char     es[],   //
        const char     os[])
{
    string eout_str = eout.str();
    eout.str("");

    conditions++;

    // TODO: verify that Expect bumped its test counter.

    if (f) {
        // TODO: verify that Expect bumped its fail counter.
        (void)e;
        (void)o;
        (void)ms;
        (void)es;
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

    EXPECT_EQ(1, argc);
    VERIFY_MEO(0, EXPECT_EQ, 1, argc);

    EXPECT_EQ(2, argc);
    VERIFY_MEO(1, EXPECT_EQ, 2, argc);

    EXPECT_NE(1, argc);
    VERIFY_MEO(1, EXPECT_NE, 1, argc);

    EXPECT_NE(2, argc);
    VERIFY_MEO(0, EXPECT_NE, 2, argc);

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
    VERIFY_MEO(0, EXPECT_EQ, foo, foo);

    EXPECT_NE(foo, foo);
    VERIFY_MEO(1, EXPECT_NE, foo, foo);

    EXPECT_EQ(foo, bar);
    VERIFY_MEO(1, EXPECT_EQ, foo, bar);

    EXPECT_NE(foo, bar);
    VERIFY_MEO(0, EXPECT_NE, foo, bar);

    EXPECT_EQ("foo", foo);
    VERIFY_MEO(0, EXPECT_EQ, "foo", foo);

    EXPECT_NE("foo", foo);
    VERIFY_MEO(1, EXPECT_NE, "foo", foo);

    EXPECT_EQ("foo", bar);
    VERIFY_MEO(1, EXPECT_EQ, "foo", bar);

    EXPECT_NE("foo", bar);
    VERIFY_MEO(0, EXPECT_NE, "foo", bar);

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
