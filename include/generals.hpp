#pragma once

#include <filesystem>

#include <exception>

#include <fstream>
#include <iostream>

#include <string>
#include <format>

namespace utils {

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

class Logger {
    
    public: 
    
    static const std::string kBackupLogFileName;
    static const std::string kLastFullLog; 

    Logger(const FilePath& dir, const std::string& log_file, const std::string& last_full);    

    void LogHistory(const std::string& msg);    
    void LogToLast(const std::string& msg);
    void Log(const std::string& msg);

    private: 

    std::ofstream hole_history;
    std::ofstream last_full;
};

bool CheckFileReadable(const FilePath& file);

}
