#include "generals.hpp"

namespace utils {

const std::string Logger::kBackupLogFileName = ".BackupHistory";
const std::string Logger::kLastFullLog = ".LastFull";

Logger::Logger(const FilePath& dir,  const std::string& log_file, const std::string& last_full) : 
        hole_history(dir / log_file, std::ofstream::in | std::ofstream::out | std::ios_base::app),
        last_full(dir / last_full,   std::ofstream::in | std::ofstream::out) {}


void Logger::LogHistory(const std::string& msg) {
    hole_history << msg;
}    

void Logger::LogToLast(const std::string& msg) {
    last_full << msg;
}

void Logger::Log(const std::string& msg) {
    last_full << msg;
    hole_history << msg;
}

ErrorStatus::ErrorStatus(Errors err, const std::string& description) : err(err), description(description) {}

bool ErrorStatus::isSuccess() {
    return err == Errors::SUCCESS;
}

bool CheckFileReadable(const FilePath& file) { 
    std::filesystem::perms file_perms = std::filesystem::status(file).permissions();
    return (file_perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
}

}