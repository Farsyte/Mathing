#include "headers.hh"

using std::cout;
using std::endl;
using std::string;
using std::setw;
using std::setprecision;
using std::right;
using std::left;

int main()
{
    string      s = "str";
    const char  a[] = "cca";
    const char *p = "ccp";

    (void)a;
    (void)p;
    (void)s;

    // This program should run quickly
    // and produce only one line of output:
    //
    //     Passed 18 test conditions.

    EXPECT_INIT();

    EXPECT_EQ("        ", str(""));
    EXPECT_EQ("x", str("x", 0));
    EXPECT_EQ("   x", str("x", 4));
    EXPECT_EQ("x   ", str("x", -4));

    EXPECT_EQ("       0", str(0));
    EXPECT_EQ("0", str(0, 0));
    EXPECT_EQ("   0", str(0, 4));
    EXPECT_EQ("0   ", str(0, -4));

    EXPECT_EQ("   1.333", str(4.0 / 3));
    EXPECT_EQ("1.33333", str(4.0 / 3, 0, 5));
    EXPECT_EQ("1.333   ", str(4.0 / 3, -8, 3));
    EXPECT_EQ("       1.333333333333333", str(4.0 / 3, 24, 15));
    EXPECT_EQ("       0.013333333333333", str(4.0 / 300, 24, 15));
    EXPECT_EQ("   1.333333333333333e-02",
              str(4.0 / 300, 24, 15, std::ios::scientific));

    EXPECT_EQ("     lit", str("lit"));
    EXPECT_EQ("     str", str(s));
    EXPECT_EQ("     cca", str(a));
    EXPECT_EQ("     ccp", str(p));

    {
        std::ostringstream ss;
        ss << setw(10) << setprecision(2) << right;
        EXPECT_EQ("      1.33", str(4.0 / 3, ss));
    }

    {
        std::ostringstream ss;
        ss << setw(10) << setprecision(2) << left;
        EXPECT_EQ("1.33      ", str(4.0 / 3, ss));
    }

    return EXPECT_FINI();
}
