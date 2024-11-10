#pragma once

#include <exception>
#include <filesystem>
#include <string>
#include <format>
#include <iostream>

namespace Utils {

const std::string kBackupLogFileName = ".BackupLog";

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
