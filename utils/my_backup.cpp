#include <iostream>

#include "backup.hpp"
#include "argument_parser.hpp"


int main(int argc, char* argv[]) {
    Utils::Backup::Command cmd = Utils::Backup::ParseArguments(argc, argv);
    Utils::Backup::MyBackup(cmd);

}