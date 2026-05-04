#ifndef CERROR_H
#define CERROR_H
#include <string>

class CError {
private:
    std::string s_errorMessage;
public:
    CError(const std::string& rs_msg);

    std::string s_getMessage() const;
};

#endif