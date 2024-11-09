#pragma once

#include "backup.hpp"

namespace Utils::Backup {

size_t const kCountArguments = 3;

Command ParseArguments(size_t argc, char* argv[]);

}