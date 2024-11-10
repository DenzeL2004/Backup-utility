#include "backup.hpp"

namespace utils::backup {

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
    if (!err.isSuccess()) {
        throw std::invalid_argument(err.description);
    }

    Logger logger(cmd.backup_dir, Logger::kBackupLogFileName, Logger::kLastFullLog);
    FilePath backup_dir = CreateBackupDir(cmd.backup_dir);

    switch (cmd.type) {
        case Command::Operation::FULL:
            FullBackup(cmd.work_dir, backup_dir, logger);
            break;
        default:
            break;
    }

    if (!err.isSuccess()) {
        throw std::runtime_error(err.description);
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

    for (auto const& content : std::filesystem::recursive_directory_iterator(cmd.work_dir)) {
        
        if (std::filesystem::equivalent(cmd.backup_dir, content)){
            return ErrorStatus(Errors::SUB_DIR, std::format("It looks like you are trying to save the backup({})"
                                                            " to the work's({}) subdirectory.\n"
                                                            "Please change name backup directory.", 
                                                            cmd.backup_dir.string(), cmd.work_dir.string()));
        }
    }

    return ErrorStatus(Errors::SUCCESS, "");
}

static FilePath CreateBackupDir(const FilePath& dir) {
    
    FilePath cur_backup_dir = dir / GetDate(time(NULL));


    if (!std::filesystem::exists(cur_backup_dir)) {
        std::filesystem::create_directory(cur_backup_dir);
    }

    return cur_backup_dir;
}

static void FullBackup(const FilePath& work_dir, const FilePath& backup_dir, Logger& logger) {
    logger.Log(std::format("{} {} {} \n", Command::kOperationNames[Command::Operation::FULL], 
                                            work_dir.string(), backup_dir.string())); 

    FullCopy(work_dir, backup_dir, logger);
    logger.Log("\n\n");
}


static void FullCopy(const FilePath& work_dir, const FilePath& backup_dir, Logger& logger) {
    
    for (auto const& content : std::filesystem::directory_iterator(work_dir)) {
        FilePath file(content);
        if (!CheckFileReadable(file)) {
            std::cout << std::format("Can't make backup file: {}. Set owner read permision by 1.\n", file.string());
            continue;
        }

        
        if (std::filesystem::is_directory(file)) {
            logger.Log(std::format("{} {}\n", file.string(), GetDateFromFile(file)));

            FilePath subdir = backup_dir / file.filename(); 
            std::filesystem::create_directory(subdir);

            FullCopy(file, subdir, logger);
        }
        else {
            std::filesystem::copy(file, backup_dir);
            logger.Log(std::format("{} {} {}\n",    file.string(), 
                                                    GetDateFromFile(file), 
                                                    std::filesystem::file_size(file)));
        }
    }
    
}


} 

