#include "backup.hpp"

namespace Utils::Backup {

const Command::OperationNameList Command::kOperationNames = Command::InitalizateOperationNames();

Command::Command(const std::string& command, const std::string& work_dir_name, const std::string& backup_dir_name) :
                work_dir(work_dir_name),  backup_dir(backup_dir_name), type(Operation::UNDEFINED) {
    for (size_t i = 0; i < kCountOperations; ++i) {
        if (command == kOperationNames[i]) {
            type = static_cast<Operation>(i);
            break;
        }
    }
} 

constexpr Command::OperationNameList Command::InitalizateOperationNames() {
    Command::OperationNameList names;
    names[static_cast<operation_t>(Command::Operation::FULL)] = "full";
    names[static_cast<operation_t>(Command::Operation::INCREMENTAL)] = "incremental";

    return names;
}


void MyBackup(const Command& cmd) {
    Errors err = CheckBackendCommand(cmd);
    if (err != Errors::SUCCESS){
        throw std::invalid_argument("Something wrong with arguments.");
    }
}


static Errors CheckBackendCommand(const Command& cmd) {
    
    if (cmd.type == Command::Operation::UNDEFINED || cmd.type >= Command::kCountOperations) { 
        std::cerr << std::format(   "Undefined peration was enterd.\n"
                                    "The operation can be: \'{}\', \'{}\'.\n", 
                                    Command::kOperationNames[Command::Operation::FULL], 
                                    Command::kOperationNames[Command::Operation::INCREMENTAL]);

        return Errors::UNDEFINED_COMMAND;
    }

    if (!std::filesystem::exists(cmd.work_dir)) {
        std::cerr << std::format("Work directory {} isn't exist.\n", cmd.work_dir.string());
        return Errors::NOT_EXIST_WORK_DIR;
    }

    if (!std::filesystem::exists(cmd.backup_dir)) {
        std::cerr <<  std::format("Backup directory {} isn't exist.\n", cmd.backup_dir.string());
        return Errors::NOT_EXIST_BACKUP_DIR;
    }

    if (std::filesystem::equivalent(cmd.backup_dir, cmd.work_dir)) {
        std::cerr << std::format(   "it looks like you are trying to save the backup({})"
                                    " to the work({}) directory.\n"
                                    "Please change name backupdirectory.", cmd.backup_dir.string(), cmd.work_dir.string());
    
        return Errors::SAME_DIR;
    }

    for (auto const& work_subdir : std::filesystem::recursive_directory_iterator(cmd.work_dir)) {
        
        if (std::filesystem::equivalent(cmd.backup_dir, work_subdir)){
            std::cerr << std::format(   "it looks like you are trying to save the backup({})"
                                        " to the work's({}) subdirectory.\n"
                                        "Please change name backup directory.", cmd.backup_dir.string(), cmd.work_dir.string());
            return Errors::SUB_DIR;
        }
    }

    return Errors::SUCCESS;
}

} 

