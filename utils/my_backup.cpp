#include "argument_parser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    BackupCommand cmd = ParseArguments(argc, argv);

    std::cout << cmd.type << " " << cmd.work_dir << " " << cmd.backup_dir << std::endl; 
}