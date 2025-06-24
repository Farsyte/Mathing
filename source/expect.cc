#define HEADERS_STD_ONLY
#include "headers.hh"
#include "expect.hh"

/** \file
 * \brief Expect support source code
 */

using std::ostream;
using std::string;
using std::cerr;
using std::endl;

namespace Expect {
    unsigned    count = 0;
    unsigned    fail = 0;
    const char *init_file = "";
    int         init_line = 0;

    ostream    *use_str = &cerr;

    void        init(const char *file, const int line)
    {
        init_file = file;
        init_line = line;
        count = 0;
        fail = 0;
        use_str = &cerr;
    }

    void     use_stream(std::ostream &str) { use_str = &str; }

    ostream &report_at(const char *file, const int line)
    {
        return (*use_str) << file << ":" << line << ": ";
    }

    int report_pass(const char *file, const int line)
    {
        (void)file;
        (void)line;

        count++;
        return 0;
    }

    ostream &report_fail(const char *file, const int line)
    {
        fail++;
        return report_at(file, line);
    }

    int fini(const char *file, const int line)
    {
        if (fail == 0) {
            report_at(file, line)                     //
                    << "passed "                      //
                    << count << " test conditions."   //
                    << endl;
            return 0;
        }
        report_at(file, line)                     //
                << "Failed " << fail << " of "    //
                << count << " test conditions."   //
                << endl;
        return 1;
    }
}
