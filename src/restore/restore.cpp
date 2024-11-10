#include "restore.hpp"


namespace utils::restore {


Command::Command(const std::string& backup_dir_name, const std::string& work_dir_name) : 
                backup_dir(backup_dir_name), work_dir(work_dir_name){}


static ErrorStatus ValidCommand(const Command& cmd) {
    
    ErrorStatus err = ValidDir(cmd.work_dir);
    if (!err.isSuccess()) return err;

    err = ValidDir(cmd.backup_dir);
    if (!err.isSuccess()) return err;

    if (std::filesystem::equivalent(cmd.backup_dir, cmd.work_dir)) {
        return ErrorStatus(Errors::SAME_DIR, std::format("It looks like you are trying to save the backup({})"
                                                        " to the work({}) directory.\n"
                                                        "Please change name backupdirectory.", 
                                                        cmd.backup_dir.string(), cmd.work_dir.string()));
    }

    if (!CheckFileReadable(cmd.work_dir)){
        return ErrorStatus(Errors::NOT_READABLE, std::format("File {} is not specified flag read ownre by 1," 
                                                                    " further backup is not possible.\n", cmd.work_dir.string()));
    }

    if (!CheckFileWritable(cmd.backup_dir)){
        return ErrorStatus(Errors::NOT_WRITABLE, std::format("File {} is not specified flag write ownre by 1," 
                                                            " further backup is not possible.\n", cmd.backup_dir.string()));
    }

    std::string work_dir_abs_path = std::filesystem::absolute(cmd.work_dir).string();
    std::string backup_dir_abs_path = std::filesystem::absolute(cmd.backup_dir).string();

    if (work_dir_abs_path.find(backup_dir_abs_path) == 0) {
        return ErrorStatus(Errors::SUB_DIR, std::format("It looks like you are trying to restore the backup({})"
                                                        " to the backup's({}) subdirectory.\n"
                                                        "Please change name work directory.", 
                                                        cmd.backup_dir.string(), cmd.work_dir.string()));
    }

    return ErrorStatus(Errors::SUCCESS, "");
}

void MyRestore(const Command& cmd) { 
    ErrorStatus err = ValidCommand(cmd);
    if (!err.isSuccess()) {
        throw std::invalid_argument(err.description);
    }

    Copy(cmd.work_dir, cmd.backup_dir);
}

void Copy(const FilePath& work_dir, const FilePath& backup_dir) { 

    for (auto const& content : std::filesystem::directory_iterator(backup_dir)) {
        FilePath file(content);
        if (!CheckFileReadable(file)) {
            std::cout << std::format("Can't restore backup file: {}. Set owner read permision by 1.\n", file.string());
            continue;
        }
        
        if (std::filesystem::is_directory(file)) {
        
            FilePath subdir = work_dir / file.filename(); 
            if (!std::filesystem::exists(subdir)){
                std::filesystem::copy(file, subdir, std::filesystem::copy_options::none);
                Copy(subdir, file);
            }
            else {
                Copy(subdir, file);
            }
        }
        else {
            FilePath work_file = work_dir / file.filename(); 
            if (!std::filesystem::exists(work_file)){
                std::filesystem::copy_file(file, work_file);
            }
            else {
                if (CheckFileWritable(work_file)){
                    std::filesystem::copy_file(file, work_file, std::filesystem::copy_options::overwrite_existing);
                }
                else {
                    std::cout << std::format("Can't restore backup file: {}. Set owner write permision by 1.\n", file.string());
                }
            }
        }
    }
}

} 
