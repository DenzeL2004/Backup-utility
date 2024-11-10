#pragma once

#include "restore.hpp"

namespace utils::restore {

size_t const kCountArguments = 2;

Command ParseArguments(size_t argc, char* argv[]);

}