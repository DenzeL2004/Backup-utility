#include <iostream>

#include "backup.hpp"
#include "argument_parser.hpp"


int main(int argc, char* argv[]) {
    utils::backup::Command cmd = utils::backup::ParseArguments(argc, argv);
    utils::backup::MyBackup(cmd);

}