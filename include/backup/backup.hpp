#pragma once 

#include "generals.hpp"
namespace utils::backup {

struct Command {

    public:

    using operation_t = size_t;
    enum Operation : operation_t {
        FULL, 
        INCREMENTAL,

        UNDEFINED,
        COUNT
    };

    FilePath work_dir;
    FilePath backup_dir;
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

static ErrorStatus ValidCommand(const Command& cmd);

static FilePath CreateBackupDir(const FilePath& backup_dir);

static void FullBackup(const FilePath& work_dir, const FilePath& backup_dir, Logger& logger);

static void FullCopy(const FilePath& work_dir, const FilePath& backup_dir, Logger& logger);

static void IncBackup(const FilePath& work_dir, const FilePath& backup_dir, Logger& logger);

static bool IncCopy(const FilePath& work_dir, const FilePath& backup_dir, Logger& logger, const FilesBackup& saved_files);

} 
