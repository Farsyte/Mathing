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
    }

    {
        // Construct a matrix of given values (using T=int).
        Mat<3, 3, int> const X = {{{1, 2, 3},              //
                                   {4, 5, 6},              //
                                   {7, 8, 9}}};

        EXPECT_EQ(str(X),                                  //
                  "\n{{        1,        2,        3 },"   //
                  "\n {        4,        5,        6 },"   //
                  "\n {        7,        8,        9 }}");
    }

    {
        // Construct a matrix of given values (using T=double).
        Mat<3, 3, double> const X = {{{1.1, 2.2, 3.3},     //
                                      {4.4, 5.5, 6.6},     //
                                      {7.7, 8.8, 9.9}}};

        EXPECT_EQ(str(X),                                  //
                  "\n{{    1.100,    2.200,    3.300 },"   //
                  "\n {    4.400,    5.500,    6.600 },"   //
                  "\n {    7.700,    8.800,    9.900 }}");
    }
}

void test_subscr()
{
    {
        Mat<2, 3, int> const d = {{{1, 2, 3},   //
                                   {4, 5, 6}}};

        EXPECT_EX(d(-1, 0), "IndexRange exception: value -1 outside inclusive range (0 .. 1)");
        EXPECT_EX(d(0, -1), "IndexRange exception: value -1 outside inclusive range (0 .. 2)");

        EXPECT_EQ(d(0, 0), 1);
        EXPECT_EQ(d(0, 1), 2);
        EXPECT_EQ(d(0, 2), 3);
        EXPECT_EQ(d(1, 0), 4);
        EXPECT_EQ(d(1, 1), 5);
        EXPECT_EQ(d(1, 2), 6);

        EXPECT_EX(d(2, 0), "IndexRange exception: value 2 outside inclusive range (0 .. 1)");
        EXPECT_EX(d(0, 3), "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
    }

    {
        Mat<2, 3, int> const d = {{{1, 2, 3},   //
                                   {4, 5, 6}}};

        EXPECT_EX(d(-1), "IndexRange exception: value -1 outside inclusive range (0 .. 5)");

        EXPECT_EQ(d(0), 1);
        EXPECT_EQ(d(1), 2);
        EXPECT_EQ(d(2), 3);
        EXPECT_EQ(d(3), 4);
        EXPECT_EQ(d(4), 5);
        EXPECT_EQ(d(5), 6);

        EXPECT_EX(d(6), "IndexRange exception: value 6 outside inclusive range (0 .. 5)");
    }

    {
        Mat<2, 3, int> d = {{{1, 2, 3},   //
                             {4, 5, 6}}};

        EXPECT_EX(d(-1) = 0, "IndexRange exception: value -1 outside inclusive range (0 .. 5)");

        EXPECT_EQ(d(0), 1);
        EXPECT_EQ(d(1), 2);
        EXPECT_EQ(d(2), 3);
        d(2) = 9;
        EXPECT_EQ(d(2), 9);
        EXPECT_EQ(d(3), 4);
        EXPECT_EQ(d(4), 5);
        EXPECT_EQ(d(5), 6);

        EXPECT_EX(d(6) = 0, "IndexRange exception: value 6 outside inclusive range (0 .. 5)");
    }

    {
        Mat<1, 4, int> const rv = {{{1, 2, 3, 4}}};

        EXPECT_EX(rv(-1), "IndexRange exception: value -1 outside inclusive range (0 .. 3)");
        EXPECT_EQ(rv(0), 1);
        EXPECT_EQ(rv(1), 2);
        EXPECT_EQ(rv(2), 3);
        EXPECT_EQ(rv(3), 4);
        EXPECT_EX(rv(4), "IndexRange exception: value 4 outside inclusive range (0 .. 3)");
    }

    {
        Mat<4, 1, int> const cv = {{{1}, {2}, {3}, {4}}};

        EXPECT_EX(cv(-1), "IndexRange exception: value -1 outside inclusive range (0 .. 3)");
        EXPECT_EQ(cv(0), 1);
        EXPECT_EQ(cv(1), 2);
        EXPECT_EQ(cv(2), 3);
        EXPECT_EQ(cv(3), 4);
        EXPECT_EX(cv(4), "IndexRange exception: value 4 outside inclusive range (0 .. 3)");
    }

    {
        Mat<1, 4, int> rv = {{{1, 2, 3, 4}}};

        EXPECT_EX(rv(-1) = 0, "IndexRange exception: value -1 outside inclusive range (0 .. 3)");
        EXPECT_EQ(rv(0), 1);
        EXPECT_EQ(rv(1), 2);
        rv(1) = 4;
        EXPECT_EQ(rv(1), 4);
        EXPECT_EQ(rv(2), 3);
        EXPECT_EQ(rv(3), 4);
        EXPECT_EX(rv(4) = 0, "IndexRange exception: value 4 outside inclusive range (0 .. 3)");
    }

    {
        Mat<4, 1, int> cv = {{{1}, {2}, {3}, {4}}};

        EXPECT_EX(cv(-1) = 0, "IndexRange exception: value -1 outside inclusive range (0 .. 3)");
        EXPECT_EQ(cv(0), 1);
        EXPECT_EQ(cv(1), 2);
        cv(1) = 4;
        EXPECT_EQ(cv(1), 4);
        EXPECT_EQ(cv(2), 3);
        EXPECT_EQ(cv(3), 4);
        EXPECT_EX(cv(4) = 0, "IndexRange exception: value 4 outside inclusive range (0 .. 3)");
    }
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
        Mat<3, 3, int> s = {};
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

        Mat<3, 3, int> lhs = {};
        EXPECT_EQ(str(lhs),                                //
                  "\n{{        0,        0,        0 },"   //
                  "\n {        0,        0,        0 },"   //
                  "\n {        0,        0,        0 }}");

        lhs = rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{        1,        2,        3 },"   //
                  "\n {        4,        5,        6 },"   //
                  "\n {        7,        8,        9 }}");
    }

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
    {
        Mat<3, 3, double> const rhs = {{{1.1, 2.2, 3.3},   //
                                        {4.4, 5.5, 6.6},   //
                                        {7.7, 8.8, 9.9}}};
        EXPECT_EQ(str(rhs),                                //
                  "\n{{    1.100,    2.200,    3.300 },"   //
                  "\n {    4.400,    5.500,    6.600 },"   //
                  "\n {    7.700,    8.800,    9.900 }}");

        Mat<3, 3, int> lhs = {};
        EXPECT_EQ(str(lhs),                                //
                  "\n{{        0,        0,        0 },"   //
                  "\n {        0,        0,        0 },"   //
                  "\n {        0,        0,        0 }}");

        lhs = rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{        1,        2,        3 },"   //
                  "\n {        4,        5,        6 },"   //
                  "\n {        7,        8,        9 }}");
    }

    {
        Mat<3, 3, double> const rhs = {{{1.1, 2.2, 3.3},   //
                                        {4.4, 5.5, 6.6},   //
                                        {7.7, 8.8, 9.9}}};
        EXPECT_EQ(str(rhs),                                //
                  "\n{{    1.100,    2.200,    3.300 },"   //
                  "\n {    4.400,    5.500,    6.600 },"   //
                  "\n {    7.700,    8.800,    9.900 }}");

        Mat<3, 3, double> lhs = {};
        EXPECT_EQ(str(lhs),                                //
                  "\n{{    0.000,    0.000,    0.000 },"   //
                  "\n {    0.000,    0.000,    0.000 },"   //
                  "\n {    0.000,    0.000,    0.000 }}");

        lhs = rhs;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{    1.100,    2.200,    3.300 },"   //
                  "\n {    4.400,    5.500,    6.600 },"   //
                  "\n {    7.700,    8.800,    9.900 }}");
    }
}

void test_compare()
{

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

        {
            Mat<3, 3, double> const rhs = {{{1, 2, 3},   //
                                            {4, 5, 6},   //
                                            {7, 8, 9}}};

            EXPECT_EQ((lhs == rhs), true);
            EXPECT_EQ((lhs != rhs), false);
        }

        {
            Mat<3, 3, double> rhs = {{{1, 2, 3},   //
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

    {
        Mat<3, 3, double> const lhs = {{{1, 2, 3},   //
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

        {
            Mat<3, 3, double> const rhs = {{{1, 2, 3},   //
                                            {4, 5, 6},   //
                                            {7, 8, 9}}};

            EXPECT_EQ((lhs == rhs), true);
            EXPECT_EQ((lhs != rhs), false);
        }

        {
            Mat<3, 3, double> rhs = {{{1, 2, 3},   //
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
}

void test_addsub()
{
    Mat<3, 3, int> const rhs = {{{1, 2, 3},   //
                                 {4, 5, 6},   //
                                 {7, 8, 9}}};

    {
        EXPECT_EQ(str(-rhs),                               //
                  "\n{{       -1,       -2,       -3 },"   //
                  "\n {       -4,       -5,       -6 },"   //
                  "\n {       -7,       -8,       -9 }}");
    }

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
    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},            //
                               {450, 550, 650},            //
                               {750, 850, 950}}};
        lhs *= 2;
        EXPECT_EQ(str(lhs),                                //
                  "\n{{      300,      500,      700 },"   //
                  "\n {      900,     1100,     1300 },"   //
                  "\n {     1500,     1700,     1900 }}");
    }

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},   //
                               {450, 550, 650},   //
                               {750, 850, 950}}};
        EXPECT_EQ(str(lhs * 2.5),                 //
                  "\n{{      375,      625,      875 },"
                  "\n {     1125,     1375,     1625 },"
                  "\n {     1875,     2125,     2375 }}");
    }

    {
        Mat<3, 3, int> rhs = {{{150, 250, 350},   //
                               {450, 550, 650},   //
                               {750, 850, 950}}};
        EXPECT_EQ(str(2.5 * rhs),                 //
                  "\n{{      375,      625,      875 },"
                  "\n {     1125,     1375,     1625 },"
                  "\n {     1875,     2125,     2375 }}");
    }

    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},   //
                               {450, 550, 650},   //
                               {750, 850, 950}}};
        lhs /= 2.5;
        EXPECT_EQ(str(lhs),                       //
                  "\n{{       60,      100,      140 },"
                  "\n {      180,      220,      260 },"
                  "\n {      300,      340,      380 }}");
    }
    {
        Mat<3, 3, int> lhs = {{{150, 250, 350},   //
                               {450, 550, 650},   //
                               {750, 850, 950}}};
        EXPECT_EQ(str(lhs / 2.5),                 //
                  "\n{{       60,      100,      140 },"
                  "\n {      180,      220,      260 },"
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
        test_subscr();
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
