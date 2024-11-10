#pragma once 

#include "generals.hpp"

namespace utils::restore {

struct Command {

    public:

    FilePath backup_dir;
    FilePath work_dir;
    

    public:

    Command(const std::string& backup_dir_name, const std::string& work_dir_name);
};

static ErrorStatus ValidCommand(const Command& cmd);

void MyRestore(const Command& cmd);

void Copy(const FilePath& work_dir, const FilePath& backup_dir);

} 
