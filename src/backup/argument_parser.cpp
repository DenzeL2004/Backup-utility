#include "argument_parser.hpp"

namespace Utils::Backup {

Command ParseArguments(size_t argc, char* argv[]) {
    
    if (argc - 1 != kCountArguments) {
        throw std::invalid_argument(std::format("Invalid number({}) of input parameters.\n "
                                                "Check the input line, there must be exactly {} arguments: operation name, name work directory, name backup directory.", 
                                                argc - 1, kCountArguments));
    }

    std::string command(argv[1]);
    return Command(command, argv[2], argv[3]);
}

}