#include "backtrace.hh"
#include <execinfo.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>   // For free

using std::cerr;
using std::endl;

void print_backtrace()
{
    const int FRAMES = 1024;
    void     *callstack[FRAMES];
    int       frames = backtrace(callstack, FRAMES);
    char    **strs = backtrace_symbols(callstack, frames);

    cerr << "I see " << frames << " frames:" << endl;
    for (int i = 0; i < frames; ++i) {
        cerr << strs[i] << endl;
    }
    free(strs);   // Free the memory allocated by backtrace_symbols
}
