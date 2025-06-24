#include "headers.hh"

int main(int argc, const char *argv[])
{

#if NDEBUG
    /*
     * This specific file intentionally does not reference these
     * variables when built with NDEBUG on. Tell the compiler this is
     * intentional, so it does not generate WARNING messages.
     *
     * Note that when built with NDEBUG on, this program should
     * produce no output on stdout or stderr.
     */
    (void)argc;
    (void)argv;
    (void)x;
#endif

    DEBUG_MESSAGE("Demonstration of DEBUG macros:");

    int x = 10;

    DEBUG_MESSAGE("x = " << x);
    DEBUG_VALUE(argc);
    DEBUG_VALUE(argv[0]);

    return 0;
}
