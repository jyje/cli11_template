#include "args.hpp"

/// CLI11 TEMPLATE
int main(int argc, char* argv[]) {
    return Args::getInstance().parseArgs(argc, argv);
}
