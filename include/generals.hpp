#pragma once

#include <filesystem>

#include <exception>
#include <chrono>

#include <fstream>
#include <iostream>

#include<map>

#include <string>
#include <format>
#include <sstream>

namespace utils {

using FilePath = std::filesystem::path;

enum class Errors : size_t {
    SUCCESS,
    UNDEFINED_COMMAND,
    NOT_EXIST_WORK_DIR,
    NOT_EXIST_BACKUP_DIR,
    NOT_DIRECTORY,
    SAME_DIR,
    SUB_DIR,
    NOT_READABLE,
    NOT_WRITABLE,
};

struct ErrorStatus {

    Errors err;
    std::string description;

    ErrorStatus(Errors err, const std::string& description);

    bool isSuccess();
};
struct FileInfo{
    size_t size;
    std::string last_changed;

    friend bool operator==(const FileInfo& lhs, const FileInfo& rhs);
};

using FilesBackup = std::map<std::string, FileInfo>;
class Logger {
    
    public: 
    
    static const std::string kBackupLogFileName;
    static const std::string kLastFullLog; 

    Logger(const FilePath& dir, const std::string& log_file, const std::string& last_full);    

    void LogHistory(const std::string& msg);    
    void LogToLast(const std::string& msg);
    void Log(const std::string& msg);

    void ClearLastFullBackup();

    bool CheckWasFullBackup();

    FilesBackup GetLastFullBackup();

    private: 

    FilePath history_log_file;
    FilePath last_full_file;

    std::fstream hole_history;
    std::fstream last_full;

};

bool CheckFileReadable(const FilePath& file);

bool CheckFileWritable(const FilePath& file);

bool CheckEquelDate(const time_t& lhs, const time_t& rhs);

bool CheckEquelSize(const size_t lhs, const size_t rhs);

bool CheckFileChange(const FilePath& file, const time_t& prev_time, const size_t prev_size);

bool CheckDirChange(const FilePath& file, const time_t& prev_time);

std::time_t LastFileWrite(const FilePath& file);

std::string GetDate(const time_t& time);

std::string GetDateFromFile(const FilePath& file);

}
