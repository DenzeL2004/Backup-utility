#pragma once 

#include <exception>
#include <filesystem>
#include <string>
#include <format>
#include <iostream>

namespace Utils::Backup {

enum class Errors : size_t {
    SUCCESS,
    UNDEFINED_COMMAND,
    NOT_EXIST_WORK_DIR,
    NOT_EXIST_BACKUP_DIR,
    SAME_DIR,
    SUB_DIR,
};

struct Command {

    public:

    using operation_t = size_t;
    enum Operation : operation_t {
        FULL, 
        INCREMENTAL,

        UNDEFINED,
        COUNT
    };

    std::filesystem::path work_dir;
    std::filesystem::path backup_dir;
    Operation type;

    static constexpr size_t kCountOperations = static_cast<operation_t>(Operation::COUNT) - 1;

    using OperationNameList = std::array<std::string, kCountOperations>;
    static const OperationNameList kOperationNames;

    public:

    Command(const std::string& operation, const std::string& work_dir_name, const std::string& backup_dir_name);

    private:

    static constexpr OperationNameList InitalizateOperationNames();
};

void MyBackup(const Command& cmd);

static Errors CheckBackendCommand(const Command& cmd);

} 
