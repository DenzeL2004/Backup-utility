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
    BackupCommand cmd(command, argv[2], argv[3]);

    if (cmd.type == BackupCommand::Command::UNDEFINED) { 

        throw std::invalid_argument(std::format("Undefined command: {}.\n"
                                                "The command can be: \'{}\', \'{}\'.", 
                                                command, 
                                                BackupCommand::kCommandNames[BackupCommand::Command::FULL], 
                                                BackupCommand::kCommandNames[BackupCommand::Command::INCREMENTAL]));
    }

    if (!std::filesystem::exists(cmd.work_dir)) {
        throw std::invalid_argument(std::format("Work directory {} isn't exist.", cmd.work_dir.string()));
    }

    if (!std::filesystem::exists(cmd.backup_dir)) {
        throw std::invalid_argument(std::format("Backup directory {} isn't exist.", cmd.backup_dir.string()));
    }

    if (std::filesystem::equivalent(cmd.backup_dir, cmd.work_dir)) {
        throw std::invalid_argument(std::format("it looks like you are trying to save the backup({})"
                                                    " to the work({}) directory.\n"
                                                    "Please change name backupdirectory.", cmd.backup_dir.string(), cmd.work_dir.string()));
    }

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(cmd.work_dir)) {
        if (std::filesystem::equivalent(cmd.backup_dir, dir_entry)){
            throw std::invalid_argument(std::format("it looks like you are trying to save the backup({})"
                                                    " to the work({}) subdirectory.\n"
                                                    "Please change name backupdirectory.", cmd.backup_dir.string(), cmd.work_dir.string()));
        }
    }

    return cmd; 
}