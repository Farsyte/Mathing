#include "bench.hh"

#include "debug.hh"
#include "str.hh"

#include <iomanip>
#include <map>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstdlib>

using std::map;
using std::vector;
using std::string;
using std::ostream;
using std::endl;
using std::cerr;

class BenchResults;
static map<string, BenchResults *> op_results;
static vector<string>              op_order;

extern "C" {
    static int compare_nsea(   //
            void const *p1,    //
            void const *p2)
    {
        double v1 = *(double *)p1;
        double v2 = *(double *)p2;
        if (v1 < v2)
            return -1;
        if (v1 > v2)
            return +1;
        return 0;
    }
}

#define SWAP(a, b) \
    do { \
        double t = a; \
        a = b; \
        b = t; \
    } while (0)

class BenchResults {
  public:

    static BenchResults *factory(   //
            string const &op,       //
            double        nsea)
    {
        return new BenchResults(op, nsea);
    }

    void update(double nsea)
    {
        if (min_of_nsea > nsea)
            min_of_nsea = nsea;
        if (max_of_nsea < nsea)
            max_of_nsea = nsea;
        list_of_nsea.push_back(nsea);
    }

    double percentile_of_msea(double pct = 50.0)
    {
        assert(0 <= pct);
        assert(pct <= 100.0);

        int size = (unsigned long)list_of_nsea.size();
        if (size < 1)
            return 0.0;

        // degenerate case
        if (size == 1)
            return list_of_nsea[0];

        double pix = (size - 1) * 0.01 * pct;
        int    pixf = pix;
        int    pixc = pix + 1;
        double fr = pix - pixf;

        assert(0 <= pixf);
        assert(pixf < pixc);
        assert(pixc < size);

        assert(0.0 <= fr);
        assert(fr <= 1.0);

        double rf = list_of_nsea[pixf];
        double rc = list_of_nsea[pixc];
        double result = rc + (rf - rc) * fr;

        return result;
    }

    void sort_msea()
    {
        int size = (int)list_of_nsea.size();

        qsort(&list_of_nsea[0],
              size,
              sizeof(list_of_nsea[0]),
              compare_nsea);
    }

    static ostream &report_full_header(          //
            ostream           &os,               //
            std::string const &tblname)
    {
        os << "#+tblname: " << tblname << "\n"   //
           << "| " << str("1st", 8)              //
           << " | " << str("min", 8)             //
           << " | " << str("25%", 8)             //
           << " | " << str("50%", 8)             //
           << " | " << str("75%", 8)             //
           << " | " << str("max", 8)             //
           << " | " << str("operation", -32) << " |\n"
           << "|-" << str("--------", 8)         //
           << "-|-" << str("--------", 8)        //
           << "-|-" << str("--------", 8)        //
           << "-|-" << str("--------", 8)        //
           << "-|-" << str("--------", 8)        //
           << "-|-" << str("--------", 8)        //
           << "-|-" << str("--------------------------------", -32)
           << "-|\n";
        return os;
    }

    ostream &report_full(   //
            ostream &os)
    {
        sort_msea();

        int size = list_of_nsea.size();
        for (int i = 1; i < size; ++i) {
            assert(list_of_nsea[i - 1] <= list_of_nsea[i]);
        }

        double q1 = percentile_of_msea(25);
        double q2 = percentile_of_msea(50);
        double q3 = percentile_of_msea(75);

        assert(min_of_nsea <= q1);
        assert(q1 <= q2);
        assert(q2 <= q3);
        assert(q3 <= max_of_nsea);

        os << "| " << str(initial_nsea, 8, 3)   //
           << " | " << str(min_of_nsea, 8, 3)   //
           << " | " << str(q1, 8, 3)            //
           << " | " << str(q2, 8, 3)            //
           << " | " << str(q3, 8, 3)            //
           << " | " << str(max_of_nsea, 8, 3)   //
           << " | " << str(op, -32) << " |" << endl;
        return os;
    }

    ~BenchResults() { DEBUG_MESSAGE("BenchResults dtor"); }

  public:   // private:

    string const   op;
    double         initial_nsea;
    double         min_of_nsea;
    double         max_of_nsea;

    vector<double> list_of_nsea;

    BenchResults(               //
            string const &op,   //
            double const  nsea)
            : op(op)
            , initial_nsea(nsea)
            , min_of_nsea(nsea)
            , max_of_nsea(nsea)
            , list_of_nsea()
    {
    }
};

void Bench::log_result(     //
        string const &op,   //
        double        nsea)
{
    BenchResults *br = op_results[op];

    if (!br) {
        br = BenchResults::factory(op, nsea);
        op_order.push_back(br->op);
        op_results[br->op] = br;
    } else {
        br->update(nsea);
    }
}

void Bench::dump_log(            //
        ostream           &os,   //
        std::string const &tblname)
{
    BenchResults::report_full_header(os, tblname);

    int op_count = op_order.size();
    for (int i = 0; i < op_count; ++i) {
        string const &op = op_order[i];
        BenchResults *br = op_results[op];
        br->report_full(os);
    }
    os << endl;

    op_order.clear();
    op_results.clear();
}
