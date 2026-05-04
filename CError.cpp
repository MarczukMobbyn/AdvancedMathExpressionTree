#include "CError.h"

CError::CError(const std::string &rs_msg) {
    s_errorMessage = rs_msg;
}

std::string CError::s_getMessage() const {
    return s_errorMessage;
}


