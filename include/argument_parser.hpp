#pragma once

#include <exception>
#include <filesystem>
#include <string>
#include <format>

size_t const kCountArguments = 3;

struct BackupCommand {
    
    public:

    using command_t = size_t;
    enum Command : command_t {
        FULL, 
        INCREMENTAL,

        UNDEFINED,
        COUNT
    };

    std::filesystem::path work_dir;
    std::filesystem::path backup_dir;
    Command type;

    static constexpr size_t kCountCommands = static_cast<BackupCommand::command_t>(Command::COUNT) - 1;

    using CommandNameList = std::array<std::string, BackupCommand::kCountCommands>;
    static const CommandNameList kCommandNames;

    public:

    BackupCommand(const std::string& command, const std::string& work_dir_name, const std::string& backup_dir_name);

    private:

    static constexpr CommandNameList InitalizateCommandNames() {
        CommandNameList names;
        names[static_cast<BackupCommand::command_t>(BackupCommand::Command::FULL)] = "full";
        names[static_cast<BackupCommand::command_t>(BackupCommand::Command::INCREMENTAL)] = "incremental";

        return names;
    }
};

BackupCommand ParseArguments(size_t argc, char* argv[]);