#include "restore/argument_parser.hpp"

namespace utils::restore {

Command ParseArguments(size_t argc, char* argv[]) {
    
    if (argc - 1 != kCountArguments) {
        throw std::invalid_argument(std::format("Invalid number({}) of input parameters.\n "
                                                "Check the input line, there must be exactly {} arguments: operation name, name work directory, name backup directory.", 
                                                argc - 1, kCountArguments));
    }

    return Command(argv[1], argv[2]);
}

}