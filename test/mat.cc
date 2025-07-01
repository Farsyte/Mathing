#include "mat.hh"
#include "str.hh"
#include "expect.hh"
#include "debug.hh"
#include "normsq.hh"

#include "exception.hh"
#include "backtrace.hh"

#include <iostream>
#include <cstdlib>

using std::ostringstream;
using std::cerr;
using std::cout;
using std::endl;
using std::setw;
using std::setprecision;
using std::string;

void test_ctor()
{
    {
        // Verify that init to {} produces a zero filled matrix.
        Mat<3, 3, int> const Z = {};
        EXPECT_EQ(str(Z),                                  //
                  "\n{{        0,        0,        0 },"   //
                  "\n {        0,        0,        0 },"   //
                  "\n {        0,        0,        0 }}");
        EXPECT_EQ(Z.nr(), 3);
        EXPECT_EQ(Z.nc(), 3);
    }

    {
        // Construct a matrix of given values.
        Mat<3, 3, int> const X = {{{1, 2, 3},              //
                                   {4, 5, 6},              //
                                   {7, 8, 9}}};

        EXPECT_EQ(str(X),                                  //
                  "\n{{        1,        2,        3 },"   //
                  "\n {        4,        5,        6 },"   //
                  "\n {        7,        8,        9 }}");
    }

    {
        Mat<3, 3, int> U = {};
        U(0, 0) = U(1, 1) = U(2, 2) = 1;
        EXPECT_EQ(str(U),                                  //
                  "\n{{        1,        0,        0 },"   //
                  "\n {        0,        1,        0 },"   //
                  "\n {        0,        0,        1 }}");
    }
}

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
	} catch (Exception const &e) {           \
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

void test_oobi()
{
    Mat<3, 3, double> const d = {{{1.1, 2.2, 3.3},   //
                                  {4.4, 5.5, 6.6},   //
                                  {7.7, 8.8, 9.9}}};

    SBE(d(-1, 0),                                    //
        "IndexRange exception: value -1 outside inclusive range (0 .. 2)");
    SBE(d(3, 0),                                     //
        "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
    SBE(d(0, -1),                                    //
        "IndexRange exception: value -1 outside inclusive range (0 .. 2)");
    SBE(d(0, 3),                                     //
        "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
    SBE(d(-1),                                       //
        "IndexRange exception: value -1 outside inclusive range (0 .. 8)");
    SBE(d(9),                                        //
        "IndexRange exception: value 9 outside inclusive range (0 .. 8)");
}

void test_print()
{
    Mat<3, 3, double> const d = {{{1.1, 2.2, 3.3},   //
                                  {4.4, 5.5, 6.6},   //
                                  {7.7, 8.8, 9.9}}};

    ostringstream           s;

    s << setw(5) << setprecision(2) << d;
    EXPECT_EQ(s.str(),   //
              "\n{{  1.10,  2.20,  3.30 },"
              "\n {  4.40,  5.50,  6.60 },"
              "\n {  7.70,  8.80,  9.90 }}");
}

void test_init()
{
    {
        Mat<3, 3, int> s;
        s.fill(3);
        EXPECT_EQ(str(s),                                  //
                  "\n{{        3,        3,        3 },"   //
                  "\n {        3,        3,        3 },"   //
                  "\n {        3,        3,        3 }}");
        s.diag(5);
        EXPECT_EQ(str(s),                                  //
                  "\n{{        5,        0,        0 },"   //
                  "\n {        0,        5,        0 },"   //
                  "\n {        0,        0,        5 }}");
        s.diag(4, 2);
        EXPECT_EQ(str(s),                                  //
                  "\n{{        4,        2,        2 },"   //
                  "\n {        2,        4,        2 },"   //
                  "\n {        2,        2,        4 }}");
        s.fill();
        EXPECT_EQ(str(s),                                  //
                  "\n{{        0,        0,        0 },"   //
                  "\n {        0,        0,        0 },"   //
                  "\n {        0,        0,        0 }}");
    }
}

void test_assign()
{
    {
        Mat<3, 3, int> const rhs = {{{1, 2, 3},            //
                                     {4, 5, 6},            //
                                     {7, 8, 9}}};
        EXPECT_EQ(str(rhs),                                //
                  "\n{{        1,        2,        3 },"   //
                  "\n {        4,        5,        6 },"   //
                  "\n {        7,        8,        9 }}");

        Mat<3, 3, double> lhs = {};
        EXPECT_EQ(str(lhs),                                //
                  "\n{{    0.000,    0.000,    0.000 },"   //
                  "\n {    0.000,    0.000,    0.000 },"   //
                  "\n {    0.000,    0.000,    0.000 }}");

        lhs = rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{    1.000,    2.000,    3.000 },"   //
                  "\n {    4.000,    5.000,    6.000 },"   //
                  "\n {    7.000,    8.000,    9.000 }}");
    }
}

void test_compare()
{

    Mat<3, 3, int> const lhs = {{{1, 2, 3},   //
                                 {4, 5, 6},   //
                                 {7, 8, 9}}};

    {
        Mat<3, 3, int> const rhs = {{{1, 2, 3},   //
                                     {4, 5, 6},   //
                                     {7, 8, 9}}};

        EXPECT_EQ((lhs == rhs), true);
        EXPECT_EQ((lhs != rhs), false);
    }

    {
        Mat<3, 3, int> rhs = {{{1, 2, 3},   //
                               {4, 5, 6},   //
                               {7, 8, 9}}};
        EXPECT_EQ((lhs == rhs), true);
        EXPECT_EQ((lhs != rhs), false);

        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c) {
                rhs(r, c)++;
                EXPECT_EQ((lhs == rhs), false);
                EXPECT_EQ((lhs != rhs), true);
                rhs(r, c)--;
                EXPECT_EQ((lhs == rhs), true);
                EXPECT_EQ((lhs != rhs), false);
            }
    }
}

void test_addsub()
{
    Mat<3, 3, double> const rhs = {{{1, 2, 3},   //
                                    {4, 5, 6},   //
                                    {7, 8, 9}}};

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        lhs += rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{      151,      252,      353 },"   //
                  "\n {      454,      555,      656 },"   //
                  "\n {      757,      858,      959 }}");
    }

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        EXPECT_EQ(str(lhs + rhs),                          //
                  "\n{{      151,      252,      353 },"   //
                  "\n {      454,      555,      656 },"   //
                  "\n {      757,      858,      959 }}");
    }

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        lhs -= rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{      149,      248,      347 },"   //
                  "\n {      446,      545,      644 },"   //
                  "\n {      743,      842,      941 }}");
    }

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        EXPECT_EQ(str(lhs - rhs),                          //
                  "\n{{      149,      248,      347 },"   //
                  "\n {      446,      545,      644 },"   //
                  "\n {      743,      842,      941 }}");
    }
}

void test_muldiv()
{
    double rhs = 2.5;

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        lhs *= rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{      375,      625,      875 },"   //
                  "\n {     1125,     1375,     1625 },"   //
                  "\n {     1875,     2125,     2375 }}");
    }

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        EXPECT_EQ(str(lhs * rhs),                          //
                  "\n{{      375,      625,      875 },"   //
                  "\n {     1125,     1375,     1625 },"   //
                  "\n {     1875,     2125,     2375 }}");
    }

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        lhs /= rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{       60,      100,      140 },"   //
                  "\n {      180,      220,      260 },"   //
                  "\n {      300,      340,      380 }}");
    }
    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        EXPECT_EQ(str(lhs / rhs),                          //
                  "\n{{       60,      100,      140 },"   //
                  "\n {      180,      220,      260 },"   //
                  "\n {      300,      340,      380 }}");
    }
}

void test_matmul()
{
    Mat<2, 3, int> const m23 = {{{1, 2, 3}, {4, 5, 6}}};
    Mat<3, 2, int> const m32 = {{{1, 2}, {3, 4}, {5, 6}}};
    Mat<3, 3, int> const m33 = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};

    EXPECT_EQ(str(m23 * m32, 3),        //
              "\n{{  22,  28 },"        //
              "\n {  49,  64 }}");
    EXPECT_EQ(str(m32 * m23, 3),        //
              "\n{{   9,  12,  15 },"   //
              "\n {  19,  26,  33 },"   //
              "\n {  29,  40,  51 }}");
    EXPECT_EQ(str(m33 * m33, 3),        //
              "\n{{  30,  36,  42 },"   //
              "\n {  66,  81,  96 },"   //
              "\n { 102, 126, 150 }}");
}

void test_trans()
{
    Mat<2, 3, int> const m23 = {{{1, 2, 3}, {4, 5, 6}}};
    Mat<3, 2, int> const m32 = {{{1, 2}, {3, 4}, {5, 6}}};
    Mat<3, 3, int> const m33 = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};

    EXPECT_EQ(str(~m23, 3),             //
              "\n{{   1,   4 },"        //
              "\n {   2,   5 },"        //
              "\n {   3,   6 }}");

    EXPECT_EQ(str(~m32, 3),             //
              "\n{{   1,   3,   5 },"   //
              "\n {   2,   4,   6 }}");

    EXPECT_EQ(str(~m33, 3),             //
              "\n{{   1,   4,   7 },"   //
              "\n {   2,   5,   8 },"   //
              "\n {   3,   6,   9 }}");

    EXPECT_EQ(str(m23 * ~m23, 3),       //
              "\n{{  14,  32 },"        //
              "\n {  32,  77 }}");

    EXPECT_EQ(str(~m23 * m23, 3),       //
              "\n{{  17,  22,  27 },"   //
              "\n {  22,  29,  36 },"   //
              "\n {  27,  36,  45 }}");

    EXPECT_EQ(str(m32 * ~m32, 3),       //
              "\n{{   5,  11,  17 },"   //
              "\n {  11,  25,  39 },"   //
              "\n {  17,  39,  61 }}");

    EXPECT_EQ(str(~m32 * m32, 3),       //
              "\n{{  35,  44 },"        //
              "\n {  44,  56 }}");

    EXPECT_EQ(str(m33 * ~m33, 3),       //
              "\n{{  14,  32,  50 },"   //
              "\n {  32,  77, 122 },"   //
              "\n {  50, 122, 194 }}");

    EXPECT_EQ(str(~m33 * m33, 3),       //
              "\n{{  66,  78,  90 },"   //
              "\n {  78,  93, 108 },"   //
              "\n {  90, 108, 126 }}");
}

void test_rvec()
{
    Mat<1, 3, int> m = {{{1, 2, 3}}};

    EXPECT_EQ(str(m), "\n{{        1,        2,        3 }}");
}

void test_cvec()
{
    Mat<3, 1, int> m = {{{1}, {2}, {3}}};

    EXPECT_EQ(str(m),
              "\n{{        1 },"
              "\n {        2 },"
              "\n {        3 }}");
}

void test_norm()
{
    EXPECT_EQ(normsq(0), 0);
    EXPECT_EQ(normsq(1), 1);
    EXPECT_EQ(normsq(2), 4);

    EXPECT_EQ(normsq(0.0), 0.0);
    EXPECT_EQ(normsq(1.0), 1.0);
    EXPECT_EQ(normsq(2.0), 4.0);

    {
        Mat<2, 2, int> m = {};
        EXPECT_EQ(normsq(m), 0);

        m(0, 0) = m(1, 1) = 1;
        EXPECT_EQ(normsq(m), 2);

        m(0, 1) = m(1, 0) = 2;
        EXPECT_EQ(normsq(m), 10);
    }

    {
        Mat<2, 2, double> m = {};
        EXPECT_EQ(normsq(m), 0);

        m(0, 0) = m(1, 1) = 1.25;
        EXPECT_EQ(normsq(m), 3.125);

        m(0, 1) = m(1, 0) = 2.50;
        EXPECT_EQ(normsq(m), 15.625);
    }
}

int main()
{
    EXPECT_INIT();

    try {

        test_ctor();
        test_oobi();
        test_print();
        test_init();
        test_assign();
        test_compare();
        test_addsub();
        test_muldiv();
        test_matmul();
        test_trans();

        test_rvec();
        test_cvec();

        test_norm();

        return EXPECT_FINI();

    } catch (Exception const &e) {
        DEBUG_MESSAGE("Caught Exception: " << e);
        print_backtrace();
        abort();
    }
}
