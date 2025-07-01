#include "bench.hh"

#include "debug.hh"
#include "str.hh"
#include "exception.hh"

#include <algorithm>
#include <iomanip>
#include <map>
#include <vector>

using std::map;
using std::vector;
using std::string;
using std::ostream;
using std::endl;
using std::cerr;

class BenchResults;
static map<string, BenchResults *> op_results;
static vector<string>              op_order;

template<typename T>
T clip(const T &lo, const T &hi, const T &v)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

class BenchResults {
  public:

    static BenchResults *factory(   //
            string const &op,       //
            double        nsea)
    {
        return new BenchResults(op, nsea);
    }

    void   update(double nsea) { list_of_nsea.push_back(nsea); }

    double percentile_of_nsea(int pct = 50)
    {
        int size = (unsigned long)list_of_nsea.size();
        if (size < 1)
            return 0.0;

        // degenerate case
        if (size == 1)
            return list_of_nsea[0];

        int    maxi = size - 1;

        double frac = pct / 100.0;

        double di = frac * maxi;
        int    i1 = clip(0, maxi, (int)di);
        int    i2 = clip(0, maxi, i1 + 1);

        double fr = clip(0.0, 1.0, di - i1);

        double v1 = list_of_nsea[i1];
        double v2 = list_of_nsea[i2];
        double result = v1 + (v2 - v1) * fr;

        return result;
    }

    void sort_nsea()
    {
        std::sort(list_of_nsea.begin(), list_of_nsea.end());
    }

    static ostream &report_full_header(        //
            ostream           &os,             //
            std::string const &tblname)
    {
        os << "#+tblname: " << tblname         //
           << "\n"                             //
           << "| " << str("min", 8)            //
           << " | " << str("25%", 8)           //
           << " | " << str("50%", 8)           //
           << " | " << str("75%", 8)           //
           << " | " << str("max", 8)           //
           << " | " << str("operation", -32)   //
           << " |\n"
           << "|-" << str("--------", 8)       //
           << "-|-" << str("--------", 8)      //
           << "-|-" << str("--------", 8)      //
           << "-|-" << str("--------", 8)      //
           << "-|-" << str("--------", 8)      //
           << "-|-" << str("--------------------------------", -32)
           << "-|\n";
        return os;
    }

    ostream &report_full(   //
            ostream &os)
    {
        sort_nsea();

        double lo = percentile_of_nsea(0);
        double q1 = percentile_of_nsea(25);
        double q2 = percentile_of_nsea(50);
        double q3 = percentile_of_nsea(75);
        double hi = percentile_of_nsea(100);

        // RangeValue::check(0, q1, lo);
        // RangeValue::check(lo, q2, q1);
        // RangeValue::check(q1, q3, q2);
        // RangeValue::check(q2, hi, q3);

        os << "| " << str(lo, 8, 3)    //
           << " | " << str(q1, 8, 3)   //
           << " | " << str(q2, 8, 3)   //
           << " | " << str(q3, 8, 3)   //
           << " | " << str(hi, 8, 3)   //
           << " | "                    //
           // << "[" << str(list_of_nsea.size(), 4) << "] "   //
           << str(op, -32) << " |" << endl;
        return os;
    }

    ~BenchResults() { DEBUG_MESSAGE("BenchResults dtor"); }

  public:   // private:

    string const   op;
    vector<double> list_of_nsea;

    BenchResults(               //
            string const &op,   //
            double const  nsea)
            : op(op)
            , list_of_nsea()
    {
        list_of_nsea.push_back(nsea);
    }
};

bool Bench::log_disable = true;
void Bench::enable() { log_disable = false; }

void Bench::log_result(     //
        string const &op,   //
        double        nsea)
{
    if (log_disable)
        return;

    BenchResults *br = op_results[op];
    if (!br) {
        op_order.push_back(op);
        op_results[op] = BenchResults::factory(op, nsea);
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
