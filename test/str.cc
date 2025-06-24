#include "expect.hh"
#include "str.hh"

#include <string>
#include <iomanip>

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

    EXPECT_EQ(str(""),                                        //
              "        ");

    EXPECT_EQ(str("x", 0),                                    //
              "x");

    EXPECT_EQ(str("x", 4),                                    //
              "   x");

    EXPECT_EQ(str("x", -4),                                   //
              "x   ");

    EXPECT_EQ(str(0),                                         //
              "       0");

    EXPECT_EQ(str(0, 0),                                      //
              "0");

    EXPECT_EQ(str(0, 4),                                      //
              "   0");

    EXPECT_EQ(str(0, -4),                                     //
              "0   ");

    EXPECT_EQ(str(4.0 / 3),                                   //
              "   1.333");

    EXPECT_EQ(str(4.0 / 3, 0, 5),                             //
              "1.33333");

    EXPECT_EQ(str(4.0 / 3, -8, 3),                            //
              "1.333   ");

    EXPECT_EQ(str(4.0 / 3, 24, 15),                           //
              "       1.333333333333333");

    EXPECT_EQ(str(4.0 / 300, 24, 15),                         //
              "       0.013333333333333");

    EXPECT_EQ(str(4.0 / 300, 24, 15, std::ios::scientific),   //
              "   1.333333333333333e-02");

    EXPECT_EQ(str("lit"),                                     //
              "     lit");

    EXPECT_EQ(str(s),                                         //
              "     str");

    EXPECT_EQ(str(a),                                         //
              "     cca");

    EXPECT_EQ(str(p),                                         //
              "     ccp");

    {
        std::ostringstream ss;
        ss << setw(10) << setprecision(2) << right;
        EXPECT_EQ(str(4.0 / 3, ss),   //
                  "      1.33");
    }

    {
        std::ostringstream ss;
        ss << setw(10) << setprecision(2) << left;
        EXPECT_EQ(str(4.0 / 3, ss),   //
                  "1.33      ");
    }

    return EXPECT_FINI();
}
