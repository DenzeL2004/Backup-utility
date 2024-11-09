#include "argument_parser.hpp"

const BackupCommand::CommandNameList BackupCommand::kCommandNames = BackupCommand::InitalizateCommandNames();

BackupCommand::BackupCommand(const std::string& command, const std::string& work_dir_name, const std::string& backup_dir_name) :
                work_dir(work_dir_name),  backup_dir(backup_dir_name), type(Command::UNDEFINED) {
    for (size_t i = 0; i < kCountCommands; ++i) {
        if (command == kCommandNames[i]) {
            type = static_cast<Command>(i);
            break;
        }
    }
}

BackupCommand ParseArguments(size_t argc, char* argv[]) {
    
    if (argc - 1 != kCountArguments) {
        throw std::invalid_argument(std::format("Invalid number({}) of input parameters.\n "
                                                "Check the input line, there must be exactly {} arguments: operation name, name work directory, name backup directory.", 
                                                argc - 1, kCountArguments));
    }

    std::string command(argv[1]);
    std::string work_dir(argv[2]);
    std::string backup_dir(argv[3]);

    BackupCommand cmd(command, work_dir, backup_dir);

    if (cmd.type == BackupCommand::Command::UNDEFINED) { 

        throw std::invalid_argument(std::format("Undefined command. You entered:{}.\n"
                                                "The command can be: \'{}\', \'{}\'.", 
                                                command, 
                                                BackupCommand::kCommandNames[BackupCommand::Command::FULL], 
                                                BackupCommand::kCommandNames[BackupCommand::Command::INCREMENTAL]));
    }

    if (!std::filesystem::exists(cmd.work_dir)) {
        throw std::invalid_argument(std::format("Work directory {} isn't exist.", work_dir));
    }

    if (!std::filesystem::exists(cmd.backup_dir)) {
        throw std::invalid_argument(std::format("Backup directory {} isn't exist.", backup_dir));
    }

    return cmd; 
}