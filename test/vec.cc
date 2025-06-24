#include "vec.hh"

#include "expect.hh"

#include <cstdlib>   // abort()

void test_ctor()
{
    {
        Vec<3, int> const z = {};
        EXPECT_EQ(str(z),   //
                  "{        0,        0,        0 }");
    }
    {
        Vec<3, double> const z = {};
        EXPECT_EQ(str(z),   //
                  "{    0.000,    0.000,    0.000 }");
    }
    {
        Vec<3, int> const v = {1, 2, 3};
        EXPECT_EQ(str(v),   //
                  "{        1,        2,        3 }");
    }
    {
        Vec<3, double> const v = {1.25, 2.50, 3.75};
        EXPECT_EQ(str(v),   //
                  "{    1.250,    2.500,    3.750 }");
    }
}

void test_subscr()
{
    {
        Vec<3, int> const v = {1, 2, 3};
        EXPECT_EX(v(-1),   //
                  "IndexRange exception: value -1 outside inclusive range (0 .. 2)");
        EXPECT_EQ(v(0), 1);
        EXPECT_EQ(v(1), 2);
        EXPECT_EQ(v(2), 3);
        EXPECT_EX(v(3),   //
                  "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
    }
    {
        Vec<3, double> const v = {1.25, 2.50, 3.75};
        EXPECT_EX(v(-1),   //
                  "IndexRange exception: value -1 outside inclusive range (0 .. 2)");
        EXPECT_EQ(v(0), 1.25);
        EXPECT_EQ(v(1), 2.50);
        EXPECT_EQ(v(2), 3.75);
        EXPECT_EX(v(3),   //
                  "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
    }
    {
        Vec<3, int> v = {1, 2, 3};
        EXPECT_EX(v(-1) = 0,   //
                  "IndexRange exception: value -1 outside inclusive range (0 .. 2)");
        EXPECT_EQ(v(0), 1);
        EXPECT_EQ(v(1), 2);
        EXPECT_EQ(v(1) = 5, 5);
        EXPECT_EQ(v(1), 5);
        EXPECT_EQ(v(2), 3);
        EXPECT_EX(v(3) = 0,   //
                  "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
    }
    {
        Vec<3, double> v = {1.25, 2.50, 3.75};
        EXPECT_EX(v(-1) = 0,   //
                  "IndexRange exception: value -1 outside inclusive range (0 .. 2)");
        EXPECT_EQ(v(0), 1.25);
        EXPECT_EQ(v(1), 2.50);
        EXPECT_EQ(v(1) = 2.75, 2.75);
        EXPECT_EQ(v(1), 2.75);
        EXPECT_EQ(v(2), 3.75);
        EXPECT_EX(v(3) = 0,   //
                  "IndexRange exception: value 3 outside inclusive range (0 .. 2)");
    }
}

void test_print()
{
    {
        Vec<3, double> const v = {1.25, 2.50, 3.75};
        std::ostringstream   s;
        s << std::setw(5) << std::setprecision(2) << v;
        EXPECT_EQ(s.str(),   //
                  "{  1.25,  2.50,  3.75 }");
    }
}

void test_fill()
{
    {
        Vec<3, int> v;
        v.fill(3);
        EXPECT_EQ(str(v),   //
                  "{        3,        3,        3 }");
        v.fill();
        EXPECT_EQ(str(v),   //
                  "{        0,        0,        0 }");
    }
}

void test_assign()
{
    {
        Vec<3, double> dv = {{1.25, 2.50, 3.75}};
        Vec<3, int>    iv = {0, 0, 0};
        iv = dv;
        EXPECT_EQ(str(iv),   //
                  "{        1,        2,        3 }");
        dv = iv;
        EXPECT_EQ(str(dv),   //
                  "{    1.000,    2.000,    3.000 }");
    }
}

void test_compare()
{
    {
        Vec<3, double> dv = {1.25, 2.50, 3.75};
        Vec<3, int>    iv = {1, 2, 3};

        EXPECT_EQ((iv == dv), false);
        EXPECT_EQ((dv == iv), false);
        EXPECT_EQ((iv != dv), true);
        EXPECT_EQ((dv != iv), true);

        dv = iv;

        EXPECT_EQ((iv == dv), true);
        EXPECT_EQ((dv == iv), true);
        EXPECT_EQ((iv != dv), false);
        EXPECT_EQ((dv != iv), false);
    }
}

void test_addsub()
{
    Vec<3, int> const rhs = {1, 2, 3};

    {
        EXPECT_EQ(str(-rhs),   //
                  "{       -1,       -2,       -3 }");
    }

    {
        Vec<3, int> lhs = {150, 250, 350};
        lhs += rhs;
        EXPECT_EQ(str(lhs),   //
                  "{      151,      252,      353 }");
    }

    {
        Vec<3, int> lhs = {150, 250, 350};
        EXPECT_EQ(str(lhs + rhs),   //
                  "{      151,      252,      353 }");
    }

    {
        Vec<3, int> lhs = {150, 250, 350};
        lhs -= rhs;
        EXPECT_EQ(str(lhs),   //
                  "{      149,      248,      347 }");
    }

    {
        Vec<3, int> lhs = {150, 250, 350};
        EXPECT_EQ(str(lhs - rhs),   //
                  "{      149,      248,      347 }");
    }
}

void test_muldiv()
{
    {
        Vec<3, int> lhs = {150, 250, 350};
        lhs *= 2;
        EXPECT_EQ(str(lhs), "{      300,      500,      700 }");
    }

    {
        Vec<3, int> lhs = {150, 250, 350};
        EXPECT_EQ(str(lhs * 2.5), "{      375,      625,      875 }");
    }

    {
        Vec<3, int> rhs = {150, 250, 350};
        EXPECT_EQ(str(2.5 * rhs), "{      375,      625,      875 }");
    }

    {
        Vec<3, int> lhs = {150, 250, 350};
        lhs /= 2.5;
        EXPECT_EQ(str(lhs), "{       60,      100,      140 }");
    }
    {
        Vec<3, int> lhs = {150, 250, 350};
        EXPECT_EQ(str(lhs / 2.5), "{       60,      100,      140 }");
    }
}

void test_vecmul()
{
    Vec<3, double> x = {1, 0, 0};
    Vec<3, double> y = {0, 1, 0};
    Vec<3, double> z = {0, 0, 1};

    Vec<3, double> xy = {1, 1, 0};
    Vec<3, double> yz = {0, 1, 1};
    Vec<3, double> zx = {1, 0, 1};

    EXPECT_EQ(str(x * x), "   1.000");
    EXPECT_EQ(str(y * y), "   1.000");
    EXPECT_EQ(str(z * z), "   1.000");

    EXPECT_EQ(str(x * y), "   0.000");
    EXPECT_EQ(str(y * z), "   0.000");
    EXPECT_EQ(str(z * x), "   0.000");

    EXPECT_EQ(str(x * z), "   0.000");
    EXPECT_EQ(str(y * x), "   0.000");
    EXPECT_EQ(str(z * y), "   0.000");

    EXPECT_EQ(str(cross(x, x)), "{    0.000,    0.000,    0.000 }");
    EXPECT_EQ(str(cross(y, y)), "{    0.000,    0.000,    0.000 }");
    EXPECT_EQ(str(cross(z, z)), "{    0.000,    0.000,    0.000 }");

    EXPECT_EQ(str(cross(y, z)), "{    1.000,    0.000,    0.000 }");
    EXPECT_EQ(str(cross(z, x)), "{    0.000,    1.000,    0.000 }");
    EXPECT_EQ(str(cross(x, y)), "{    0.000,    0.000,    1.000 }");

    EXPECT_EQ(str(cross(z, y)), "{   -1.000,    0.000,    0.000 }");
    EXPECT_EQ(str(cross(x, z)), "{    0.000,   -1.000,    0.000 }");
    EXPECT_EQ(str(cross(y, x)), "{    0.000,    0.000,   -1.000 }");

    EXPECT_EQ(str(xy * xy), "   2.000");
    EXPECT_EQ(str(yz * yz), "   2.000");
    EXPECT_EQ(str(zx * zx), "   2.000");

    EXPECT_EQ(str(xy * zx), "   1.000");
    EXPECT_EQ(str(yz * xy), "   1.000");
    EXPECT_EQ(str(zx * yz), "   1.000");

    EXPECT_EQ(str(zx * xy), "   1.000");
    EXPECT_EQ(str(xy * yz), "   1.000");
    EXPECT_EQ(str(yz * zx), "   1.000");

    EXPECT_EQ(str(cross(xy, xy)), "{    0.000,    0.000,    0.000 }");
    EXPECT_EQ(str(cross(yz, yz)), "{    0.000,    0.000,    0.000 }");
    EXPECT_EQ(str(cross(zx, zx)), "{    0.000,    0.000,    0.000 }");

    EXPECT_EQ(str(cross(xy, zx)), "{    1.000,   -1.000,   -1.000 }");
    EXPECT_EQ(str(cross(yz, xy)), "{   -1.000,    1.000,   -1.000 }");
    EXPECT_EQ(str(cross(zx, yz)), "{   -1.000,   -1.000,    1.000 }");

    EXPECT_EQ(str(cross(zx, xy)), "{   -1.000,    1.000,    1.000 }");
    EXPECT_EQ(str(cross(xy, yz)), "{    1.000,   -1.000,    1.000 }");
    EXPECT_EQ(str(cross(yz, zx)), "{    1.000,    1.000,   -1.000 }");
}

void test_normsq()
{
    {
        Vec<3, int> v = {1, 2, 3};
        EXPECT_EQ(str(v * v),       //
                  "      14");
        EXPECT_EQ(str(normsq(v)),   //
                  "      14");
    }
    {
        Vec<3, double> v = {1.1, 2.2, 3.3};
        EXPECT_EQ(str(v * v),       //
                  "  16.940");
        EXPECT_EQ(str(normsq(v)),   //
                  "  16.940");
    }
}

int main()
{
    EXPECT_INIT();

    try {

        test_ctor();
        test_subscr();
        test_print();
        test_fill();
        test_assign();
        test_compare();
        test_addsub();
        test_muldiv();
        test_vecmul();
        test_normsq();

        return EXPECT_FINI();

    } catch (Exception const &e) {
        DEBUG_MESSAGE("Caught Exception: " << e);
        print_backtrace();
        abort();
    }
}
