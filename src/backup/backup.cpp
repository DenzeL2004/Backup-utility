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
    ErrorStatus err = CheckBackendCommand(cmd);
    if (!err.isSuccess()){
        throw std::invalid_argument(err.description);
    }
}


static ErrorStatus CheckBackendCommand(const Command& cmd) {
    
    if (cmd.type == Command::Operation::UNDEFINED || cmd.type >= Command::kCountOperations) { 
        return ErrorStatus(Errors::UNDEFINED_COMMAND, std::format(  "Undefined peration was enterd.\n"
                                                                    "The operation can be: \'{}\', \'{}\'.\n", 
                                                                    Command::kOperationNames[Command::Operation::FULL], 
                                                                    Command::kOperationNames[Command::Operation::INCREMENTAL]));
    }

    if (!std::filesystem::exists(cmd.work_dir)) {
        return ErrorStatus(Errors::NOT_EXIST_WORK_DIR, std::format("Work directory {} isn't exist.\n", cmd.work_dir.string()));
    }

    if (!std::filesystem::exists(cmd.backup_dir)) {
        return ErrorStatus(Errors::NOT_EXIST_BACKUP_DIR, std::format("Backup directory {} isn't exist.\n", cmd.backup_dir.string()));
    }

    if (std::filesystem::equivalent(cmd.backup_dir, cmd.work_dir)) {
        return ErrorStatus(Errors::SAME_DIR, std::format("It looks like you are trying to save the backup({})"
                                                        " to the work({}) directory.\n"
                                                        "Please change name backupdirectory.", 
                                                        cmd.backup_dir.string(), cmd.work_dir.string()));
    }

    for (auto const& work_subdir : std::filesystem::recursive_directory_iterator(cmd.work_dir)) {
        
        if (std::filesystem::equivalent(cmd.backup_dir, work_subdir)){
            return ErrorStatus(Errors::SUB_DIR, std::format("It looks like you are trying to save the backup({})"
                                                            " to the work's({}) subdirectory.\n"
                                                            "Please change name backup directory.", 
                                                            cmd.backup_dir.string(), cmd.work_dir.string()));
        }
    }

    return ErrorStatus(Errors::SUCCESS, "");
}

} 

