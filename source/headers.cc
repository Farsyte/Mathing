#include "headers.hh"

int main(int argc, char **argv) {
    std::cout << "Header check file runs!" << std::endl;
    for (int argi = 1; argi < argc; ++argi)
	std::cout << "  argv[" << argi << "] = " << argv[argi] << std::endl;
    return 0;
}
