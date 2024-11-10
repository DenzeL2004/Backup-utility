#include <iostream>

#include "restore.hpp"
#include "restore/argument_parser.hpp"


int main(int argc, char* argv[]) {
    utils::restore::Command cmd = utils::restore::ParseArguments(argc, argv);
    utils::restore::MyRestore(cmd);
}