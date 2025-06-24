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

    static BenchResults *factory(string const &op, double nsea) { return new BenchResults(op, nsea); }

    void                 update(double nsea) { list_of_nsea.push_back(nsea); }

    double               percentile_of_nsea(int pct = 50)
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

    void            sort_nsea() { std::sort(list_of_nsea.begin(), list_of_nsea.end()); }

    static ostream &report_full_header(ostream &os)
    {
        os << "\n#+begin_src python :session :results none"
           << "\n  #                " << str("min", 8)                //
           << "  " << str("25%", 8)                                   //
           << "  " << str("50%", 8)                                   //
           << "  " << str("75%", 8)                                   //
           << "  " << str("max", 8)                                   //
           << "   " << str("operation", -32)                          //
           << "\n  #                " << str("--------", 8)           //
           << "  " << str("--------", 8)                              //
           << "  " << str("--------", 8)                              //
           << "  " << str("--------", 8)                              //
           << "  " << str("--------", 8)                              //
           << "   " << str("--------------------------------", -32)   //
                ;
        return os;
    }

    ostream &report_full(ostream &os)
    {
        sort_nsea();

        double lo = percentile_of_nsea(0);
        double q1 = percentile_of_nsea(25);
        double q2 = percentile_of_nsea(50);
        double q3 = percentile_of_nsea(75);
        double hi = percentile_of_nsea(100);

        string q = "\"";
        string qopq = q + op + q;

        os << "\n  " << str("tbl_O" OPTIMIZER_LEVEL, -8) << ".append"
           << "([" << str(lo, 8, 3)    //
           << ", " << str(q1, 8, 3)    //
           << ", " << str(q2, 8, 3)    //
           << ", " << str(q3, 8, 3)    //
           << ", " << str(hi, 8, 3)    //
           << ", " << str(qopq, -32)   //
           << "])";
        return os;
    }

    ~BenchResults() { }

    BenchResults(string const &op, double const nsea)
            : op(op)
            , list_of_nsea()
    {
        list_of_nsea.push_back(nsea);
    }

  private:

    string const   op;
    vector<double> list_of_nsea;
};

bool Bench::disabled = true;
void Bench::enable() { disabled = false; }
void Bench::disable() { disabled = true; }

void Bench::log_result()
{
    if (disabled || logged)
        return;

    double        dt = nsea();   // sample the timer ASAP

    BenchResults *br = op_results[op];
    if (!br) {
        op_order.push_back(op);
        op_results[op] = BenchResults::factory(op, dt);
    } else {
        br->update(dt);
    }
    logged = true;
}

void Bench::dump_log(ostream &os)
{
    BenchResults::report_full_header(os);

    int op_count = op_order.size();
    for (int i = 0; i < op_count; ++i) {
        string const &op = op_order[i];
        BenchResults *br = op_results[op];
        br->report_full(os);
    }
    os << "\n#+end_src"
       << "\n"
       << endl;

    op_order.clear();
    op_results.clear();
}
