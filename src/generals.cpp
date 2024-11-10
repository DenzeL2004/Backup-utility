#include "generals.hpp"

namespace Utils {

ErrorStatus::ErrorStatus(Errors err, const std::string& description) : err(err), description(description) {}

bool ErrorStatus::isSuccess() {
    return err == Errors::SUCCESS;
}

}