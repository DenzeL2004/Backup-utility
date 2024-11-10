#pragma once 

#include "generals.hpp"
namespace Utils::Backup {

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

static ErrorStatus CheckBackendCommand(const Command& cmd);

} 
