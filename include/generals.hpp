#pragma once

#include <filesystem>

#include <exception>

#include <fstream>
#include <iostream>

#include <string>
#include <format>

namespace utils {

const std::string kBackupLogFileName = ".BackupLog";

using FilePath = std::filesystem::path;

enum class Errors : size_t {
    SUCCESS,
    UNDEFINED_COMMAND,
    NOT_EXIST_WORK_DIR,
    NOT_EXIST_BACKUP_DIR,
    SAME_DIR,
    SUB_DIR,
};

struct ErrorStatus {

    Errors err;
    std::string description;

    ErrorStatus(Errors err, const std::string& description);

    bool isSuccess();
};

}
