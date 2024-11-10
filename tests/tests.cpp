#include "argument_parser.hpp"
#include <gtest/gtest.h>
#include <numeric>
#include <type_traits>
#include <sstream>
#include <string>
#include <fstream>

std::string kWorkDirName = "work";
std::string kBackupDirName = "backup";
std::string kOtherFile = "rubish.txt";

const size_t argc = 4;
char* argv_full[] = {kOtherFile.data(), "full", kWorkDirName.data(), kBackupDirName.data()};
char* argv_inc[] = {kOtherFile.data(), "incremental", kWorkDirName.data(), kBackupDirName.data()};


TEST(NoExceptions, PARSER) {
    ASSERT_NO_THROW(utils::backup::ParseArguments(argc, argv_full));
    ASSERT_NO_THROW(utils::backup::ParseArguments(argc, argv_inc));
}

TEST(UncorrectArgc, BACKUP) {
    ASSERT_ANY_THROW(utils::backup::ParseArguments(1, argv_full));
}

TEST(UndefineCommand, BACKUP) {
    char* argv[] = {kOtherFile.data(), "undefinwe", kWorkDirName.data(), kBackupDirName.data()};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(NotExistWorkDir, BACKUP) {
    char* argv[] = {kOtherFile.data(), "full", "work1", kBackupDirName.data()};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(NotExistBackupDir, BACKUP) {
    char* argv[] = {kOtherFile.data(), "full", kWorkDirName.data(), "backup1"};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(WorkIsNotDir, BACKUP) {
    char* argv[] = {kOtherFile.data(), "full", kOtherFile.data(), "backup1"};
    ASSERT_ANY_THROW(utils::backup::ParseArguments(1, argv_full));
}

TEST(BackupIsNotDir, BACKUP) {
    char* argv[] = {kOtherFile.data(), "full", kWorkDirName.data(), kOtherFile.data()};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(BackuSameWork, BACKUP) {
    char* argv[] = {kOtherFile.data(), "full", kWorkDirName.data(), kWorkDirName.data()};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}


TEST(BackupIsSubdirWork, BACKUP) {
    char* argv[] = {kOtherFile.data(), "full", kWorkDirName.data(), "work/subdir"};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(GenetateLogFile, BACKUP) {
    utils::FilePath log_file = utils::FilePath(kBackupDirName) / utils::FilePath(utils::Logger::kBackupLogFileName);
    if (std::filesystem::exists(log_file)){
        std::filesystem::remove(log_file);
    }

    utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv_full));

    ASSERT_TRUE(std::filesystem::exists(log_file));
}


int main(int argc, char* argv[]) {
    
    std::ofstream(kOtherFile, std::fstream::out);
    std::filesystem::create_directory(kWorkDirName);                           //      work/
    std::ofstream(kWorkDirName + "/file1.txt", std::fstream::out);             //          file1.txt
    std::ofstream(kWorkDirName + "/file2.txt", std::fstream::out);             //          file2.txt
    std::filesystem::create_directory(kWorkDirName + "/subdir");               //          subdir/
    std::ofstream(kWorkDirName + "/subdir/file3.txt", std::fstream::out);      //              file3.txt
    std::filesystem::create_directory(kBackupDirName);                         //      backup/

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
