#include "exception.hh"
#include "expect.hh"

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

#define SB(exp, des)                                    \
    do {                                                \
        string const cmp(str(des));                     \
        try {                                           \
            string const act(str(exp));                 \
            if (cmp != act)                             \
                cout << __FILE__ << ":"                 \
                     << __LINE__ << ": "                \
                     << (#exp) << "\n"                  \
                     << "des:\t«" << (cmp) << "»\n"     \
                     << "act:\t«" << (act) << "»\n"     \
                     << "\n";                           \
        }                                               \
        catch (Exception const &e) {                    \
            string exc(e);                              \
            cout << __FILE__ << ":"                     \
                 << __LINE__ << ": "                    \
                 << (#exp) << "\n"                      \
                 << "des:\t«" << (cmp) << "»\n"         \
                 << "act[E]:\t«" << (exc) << "»\n"      \
                 << "\n";                               \
        }                                               \
    } while (0)

#define SBE(exp, des)					\
    do {						\
	string const cmp(str(des));                     \
	try {						\
	    string const act(str(exp));                 \
	    cout << __FILE__ << ":"			\
		 << __LINE__ << ": "			\
		 << (#exp) << "\n"			\
		 << "des[E]:\t«" << (cmp) << "»\n"	\
		 << "act:\t«" << (act) << "»\n"		\
		 << "\n";				\
	} catch (Exception const &e) {                  \
	    string exc(e);				\
            if (cmp != exc)				\
                cout << __FILE__ << ":"			\
                     << __LINE__ << ": "		\
                     << (#exp) << "\n"			\
                     << "des[E]:\t«" << (cmp) << "»\n"	\
                     << "act[E]:\t«" << (exc) << "»\n"	\
                     << "\n";				\
	}						\
    } while (0)

void test_IndexRange()
{
    SB(IndexRange::check(0, 0, 2), 0);
    SB(IndexRange::check(1, 0, 2), 0);
    SB(IndexRange::check(2, 0, 2), 0);

    SBE(IndexRange::check(-1, 0, 2),
        "IndexRange exception: value -1 outside inclusive range (0 .. 2)");
    SBE(IndexRange::check(3, 0, 2),
        "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
}

void test_RangeValue()
{
    SB(RangeValue::check(1.0, 2.0, 1.0), 0);
    SB(RangeValue::check(1.0, 2.0, 1.5), 0);
    SB(RangeValue::check(1.0, 2.0, 2.0), 0);

    SBE(RangeValue::check(1.0, 2.0, 0.5),
        "RangeValue exception: Value not within empty range:"   //
        "\n  minimum:  1"                                       //
        "\n  observed: 0.5"                                     //
        "\n  maximum:  2");

    SBE(RangeValue::check(1.0, 2.0, 2.5),
        "RangeValue exception: Value not within empty range:"   //
        "\n  minimum:  1"                                       //
        "\n  observed: 2.5"                                     //
        "\n  maximum:  2");
}

int main()
{
    EXPECT_INIT();
    test_IndexRange();
    test_RangeValue();
    return EXPECT_FINI();
}
