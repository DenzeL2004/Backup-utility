#include "argument_parser.hpp"
#include <gtest/gtest.h>
#include <numeric>
#include <type_traits>
#include <sstream>
#include <string>
#include <fstream>

std::string work_dir_name = "work";
std::string backup_dir_name = "backup";

const size_t argc = 4;
char* argv_full[] = {"rubish", "full", work_dir_name.data(), backup_dir_name.data()};
char* argv_inc[] = {"rubish", "incremental", work_dir_name.data(), backup_dir_name.data()};


TEST(NoExceptions, PARSER) {
    ASSERT_NO_THROW(utils::backup::ParseArguments(argc, argv_full));
    ASSERT_NO_THROW(utils::backup::ParseArguments(argc, argv_inc));
}

TEST(UncorrectArgc, BACKUP) {
    ASSERT_ANY_THROW(utils::backup::ParseArguments(1, argv_full));
}

TEST(UndefineCommand, BACKUP) {
    char* argv[] = {"rubish", "undefinwe", work_dir_name.data(), backup_dir_name.data()};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(NotExistWorkDir, BACKUP) {
    char* argv[] = {"rubish", "full", "work1", backup_dir_name.data()};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(NotExistbackupDir, BACKUP) {
    char* argv[] = {"rubish", "full", work_dir_name.data(), "backup1"};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(BackuSameWork, BACKUP) {
    char* argv[] = {"rubish", "full", work_dir_name.data(), work_dir_name.data()};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}


TEST(BackupIsSubdirWork, BACKUP) {
    char* argv[] = {"rubish", "full", work_dir_name.data(), "work/subdir"};
    ASSERT_ANY_THROW(utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv)));
}

TEST(GenetateLogFile, BACKUP) {
    utils::FilePath log_file = utils::FilePath(backup_dir_name) / utils::FilePath(utils::kBackupLogFileName);
    if (std::filesystem::exists(log_file)){
        std::filesystem::remove(log_file);
    }

    utils::backup::MyBackup(utils::backup::ParseArguments(argc, argv_full));

    ASSERT_TRUE(std::filesystem::exists(log_file));
}


int main(int argc, char* argv[]) {
    
    std::filesystem::create_directory(work_dir_name);                           //      work/
    std::ofstream(work_dir_name + "/file1.txt", std::fstream::out);             //          file1.txt
    std::ofstream(work_dir_name + "/file2.txt", std::fstream::out);             //          file2.txt
    std::filesystem::create_directory(work_dir_name + "/subdir");               //          subdir/
    std::ofstream(work_dir_name + "/subdir/file3.txt", std::fstream::out);      //              file3.txt
    std::filesystem::create_directory(backup_dir_name);                         //      backup/

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
