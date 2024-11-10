#include "generals.hpp"

namespace utils {

const std::string Logger::kBackupLogFileName = ".BackupHistory";
const std::string Logger::kLastFullLog = ".LastLog";

Logger::Logger(const FilePath& dir,  const std::string& log_file, const std::string& last_full_file) : 
        history_log_file(dir / log_file),
        last_full_file(dir / last_full_file),
        hole_history(dir / log_file, std::ios_base::app),
        last_full(dir / last_full_file, std::ios_base::out | std::ios_base::in) {}


void Logger::LogHistory(const std::string& msg) {
    hole_history << msg;
}    

void Logger::LogToLast(const std::string& msg) {
    last_full << msg;
}

void Logger::Log(const std::string& msg) {
    LogToLast(msg);
    LogHistory(msg);
}

void Logger::ClearLastFullBackup(){
    last_full.close();

    std::fstream clear_file(last_full_file, std::ios::out);
    clear_file.close();

    last_full.open(last_full_file, std::ios_base::out | std::ios_base::in);
}

bool Logger::CheckWasFullBackup() {
    return std::filesystem::file_size(last_full_file) != 0;
}

FilesBackup Logger::GetLastFullBackup() {
    std::string text;

    FilesBackup saved_files;

    while(std::getline(last_full, text)){
        std::stringstream line(text);
        std::string path, data, size;

        line >> path;
        line >> data;

        if (line >> size) {
            saved_files[path] = {std::stoull(size), data};
        }
        else {
            saved_files[path] = {0, data};
        }
    }

    return saved_files;
}

//-------------------------------------------------------------------------------------------------------------

bool operator==(const FileInfo& lhs, const FileInfo& rhs) {
    return lhs.size == rhs.size && lhs.last_changed == rhs.last_changed;
}

//-------------------------------------------------------------------------------------------------------------

ErrorStatus::ErrorStatus(Errors err, const std::string& description) : err(err), description(description) {}

bool ErrorStatus::isSuccess() {
    return err == Errors::SUCCESS;
}

//-------------------------------------------------------------------------------------------------------------

bool CheckFileReadable(const FilePath& file) { 
    std::filesystem::perms file_perms = std::filesystem::status(file).permissions();
    return (file_perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
}

bool CheckFileWritable(const FilePath& file) { 
    std::filesystem::perms file_perms = std::filesystem::status(file).permissions();
    return (file_perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
}

std::time_t LastFileWrite(const FilePath& file) {
    auto file_time = std::filesystem::last_write_time(file);
    auto system_time = std::chrono::clock_cast<std::chrono::system_clock>(file_time);
    auto time = std::chrono::system_clock::to_time_t(system_time);

    return time;
}

std::string GetDate(const time_t& time) {

    tm date = *localtime(&time);

    std::string stime("YYYY-MM-DD_HH-MM-SS");
    sprintf(stime.data(), "%.4d-%.2d-%.2d_%.2d-%.2d-%.2d",  date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, 
                                                            date.tm_hour, date.tm_min, date.tm_sec);

    return stime;
}

std::string GetDateFromFile(const FilePath& file) { 
    return GetDate(LastFileWrite(file));
}

ErrorStatus ValidDir(const FilePath& file) {
    if (!std::filesystem::exists(file)) {
        return ErrorStatus(Errors::NOT_EXIST_WORK_DIR, std::format("File {} isn't exist.\n", file.string()));
    }

    if (!std::filesystem::is_directory(file)) {
        return ErrorStatus(Errors::NOT_DIRECTORY, std::format("File {} isn't a drectory.\n", file.string()));
    }

    return ErrorStatus(Errors::SUCCESS, "");
}

}