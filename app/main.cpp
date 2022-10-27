#include "../source/args.hpp"

int main(int argc, char* argv[]) {
    return Args::getInstance().parseArgs(argc, argv);
}
