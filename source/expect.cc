#include "expect.hh"

/** \file
 * \brief Expect support source code
 */

#include <iostream>

using std::ostream;
using std::string;
using std::cerr;
using std::endl;

namespace Expect {
    unsigned    count = 0;
    unsigned    fail = 0;
    const char *init_file = "";
    int         init_line = 0;
    const char *init_func = "";

    ostream    *use_str = &cerr;

    void        init(const char *file, const int line, const char *func)
    {
        init_file = file;
        init_line = line;
        init_func = func;
        count = 0;
        fail = 0;
        use_str = &cerr;
    }

    void     use_stream(std::ostream &str) { use_str = &str; }

    ostream &report_at(const char *file, const int line, const char *func)
    {
        return (*use_str) << file << ":" << line << ": in " << func << ", ";
    }

    int report_pass(const char *file, const int line, const char *func)
    {
        (void)file;
        (void)line;
        (void)func;

        count++;
        return 0;
    }

    ostream &report_fail(const char *file, const int line, const char *func)
    {
        fail++;
        return report_at(file, line, func);
    }

    int fini(const char *file, const int line, const char *func)
    {
        if (fail == 0) {
            report_at(file, line, func)               //
                    << "passed "                      //
                    << count << " test conditions."   //
                    << endl;
            return 0;
        }
        report_at(file, line, func)               //
                << "Failed " << fail << " of "    //
                << count << " test conditions."   //
                << endl;
        return 1;
    }

    ostream &report_anex(ostream &os, std::string const &src, std::string const &exp)
    {
        return os << "Unexpected exception from"     //
                  << "\n\t«" << src << "»"           //
                  << "\nunexpected exception was:"   //
                  << "\n\t«" << exp << "»"           //
                  << std::endl;
    }

    ostream &report_noex(ostream &os, std::string const &src, std::string const &exp)
    {
        return os << "Did not see an expected exception from"   //
                  << "\n\t«" << src << "»"                      //
                  << "\nexpected exception:"                    //
                  << "\n\t«" << exp << "»"                      //
                  << std::endl;
    }

    ostream &report_badx(ostream &os, std::string const &src, std::string const &exp, std::string const &obs)
    {
        return os << "Saw the wrong exception from"   //
                  << "\n\t«" << src << "»"            //
                  << "\nexpected exception:"          //
                  << "\n\t«" << exp << "»"            //
                  << "\nobserved exception:"          //
                  << "\n\t«" << obs << "»"            //
                  << std::endl;
    }
}
