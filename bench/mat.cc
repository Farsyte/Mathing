#include "mat.hh"
#include "bench.hh"
#include "exception.hh"
#include "backtrace.hh"
#include "timer.hh"

#include "expect.hh"

#include <cstdlib>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

// when reducing N, be sure none of the individual
// benchmark sections run too quickly.

const int         N = 1000;
const int         NN = N * N;

const double      suite_sec = 5.0;
const double      finish_sec = 60.0;

int               R = 1;

Mat<1, N, double> rv;
Mat<N, 1, double> cv;
Mat<N, N, double> m1;
Mat<N, N, double> m2;
Mat<N, N, double> m3;

void              mat_init()
{
    Bench t(cout, NN * 6 + N * 4, 1, "init with rand()");
    for (int i = 0; i < N; ++i) {
        cv.unchecked(i) = rand() * 2.0 / RAND_MAX - 1;
        rv.unchecked(i) = rand() * 2.0 / RAND_MAX - 1;
    }
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            m1.unchecked(r, c) = rand() * 2.0 / RAND_MAX - 1;
            m2.unchecked(r, c) = rand() * 2.0 / RAND_MAX - 1;
            m3.unchecked(r, c) = rand() * 2.0 / RAND_MAX - 1;
        }
    }
    EXPECT_NE(normsq(cv), 0);
    EXPECT_NE(normsq(rv), 0);
    EXPECT_NE(normsq(m1), 0);
    EXPECT_NE(normsq(m2), 0);
    EXPECT_NE(normsq(m3), 0);
}

void mat_assign()
{
    Bench t(cout, NN, R, "Mat = Mat");
    for (int i = 0; i < R; ++i) {
        m1 = m2;
    }
}

void mat_addassign()
{
    Bench t(cout, NN, R, "Mat += Mat");
    for (int i = 0; i < R; ++i) {
        m1 += m2;
    }
}

void mat_subassign()
{
    Bench t(cout, NN, R, "Mat -= Mat");
    for (int i = 0; i < R; ++i) {
        m1 -= m2;
    }
}

void mat_add()
{
    Bench t(cout, NN, R, "Mat = Mat + Mat");
    for (int i = 0; i < R; ++i) {
        m1 = m2 + m3;
    }
}

void mat_sub()
{
    Bench t(cout, NN, R, "Mat = Mat - Mat");
    for (int i = 0; i < R; ++i) {
        m3 = m1 - m2;
    }
}

void mat_mul()
{
    Bench t(cout, NN, R, "Mat *= Num");
    for (int i = 0; i < R; ++i) {
        m1 *= 1.1;
    }
}

void mat_div()
{
    Bench t(cout, NN, R, "Mat /= Num");
    for (int i = 0; i < R; ++i) {
        m1 /= 1.01;
    }
}

void vec_mat_mul()
{
    // Multiply R-element row vector by R-by-C matrix.

    static Mat<1, N, double> s = rv;
    Bench                    t(cout, NN, R, "RVec += RVec * Mat");
    for (int i = 0; i < R; ++i) {
        s += rv * m1;
    }
}

void mat_vec_mul()
{
    // Multiply R-by-C matrix by C-element column vector.

    static Mat<N, 1, double> s = cv;
    Bench                    t(cout, NN, R, "CVec += Mat * CVec");
    for (int i = 0; i < R; ++i) {
        s += m1 * cv;
    }
}

void bench_matmul_body()
{
    mat_init();
    mat_assign();
    mat_addassign();
    mat_subassign();
    mat_add();
    mat_sub();
    mat_mul();
    mat_div();
    vec_mat_mul();
    mat_vec_mul();
}

void bench_matmul()
{

    cout << endl
         << "Benchmarking with N=" << N << "\n"
         << "All durations reported in ns per element.\n"
         << "Each matrix uses " << sizeof(m1) << " bytes of storage.\n"
         << "Target time per pass: " << suite_sec << " seconds.\n"
         << "Target time for run: " << finish_sec << " seconds.\n"
         << endl;

    // The first run is subject to Cold Cache effects.
    // Ignore its runtime.
    R = 1;
    bench_matmul_body();

    for (;;) {
        Timer reptime;
        bench_matmul_body();
        double sec = reptime.elapsed_sec();

        // Tune R to get elapsed time to suite_sec
        if (sec < suite_sec / 10) {
            // Insufficient elapsed time to allow us to
            // compute the R that gets us close to suite_sec.
            R *= 10;
        } else {
            // We are within an order of magnitude of the
            // right elapsed time. Pick an R and go with it.
            cerr << "For R=" << R<< ", T=" << str(sec, 0, 2);
            R = (R * suite_sec / sec) + 0.5;
            cerr << "; want T=" << suite_sec;
            cerr << ", will use R=" << R << endl;
            Bench::enable();
            break;
        }
    }

    Timer benchtime;
    for (int reps = 0; reps < 1000; reps ++) {
        bench_matmul_body();
        double total_sec = benchtime.elapsed_sec();
        if (total_sec > finish_sec) {
            cerr
                << "\nBenchmark ending after " << reps << " repetitions of the suite;"
                << "\nTotal elapsed time: " << str(total_sec, 0, 0) << " seconds"
                << endl;
            break;
        }
    }
}

int main()
{
    try {
        bench_matmul();
        Bench::dump_log(cout, "bench_mat_O" OPTIMIZER_LEVEL);
    } catch (Exception &e) {
        cerr << "benchmark terminated by Exception: " << e << endl;
        print_backtrace();
        abort();
    }
}
