// CResultWriter.h

#ifndef CRESULTWRITER_H
#define CRESULTWRITER_H

#include "CResult.h"
#include "CError.h"
#include "CTree.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class CResultWriter
{
private:
    static inline void v_writeErrors(std::ostream& os, const std::vector<CError*>& v_errors);


public:
    template <typename T>
    bool inline bWriteToFile(const CResult<T, CError>& cResult, const std::string& sFileName);
};


void inline CResultWriter::v_writeErrors(std::ostream& os, const std::vector<CError*>& v_errors)
{
    os << "--- BLEDY ---" << std::endl;
    for (size_t i = 0; i < v_errors.size(); ++i)
    {
        os << v_errors[i]->s_getMessage() << std::endl;
    }
}

// 1. IMPLEMENTACJA SZABLONU OGÓLNEGO
template <typename T>
bool inline CResultWriter::bWriteToFile(const CResult<T, CError>& cResult, const std::string& sFileName)
{
    std::ofstream file(sFileName.c_str());
    if (!file.is_open()) return false;

    if (!cResult.bIsSuccess())
    {
        CResultWriter::v_writeErrors(file, cResult.vGetErrors());
    }
    file.close();
    return true;
}

// 2. IMPLEMENTACJA PEŁNEJ SPECJALIZACJI
template <>
bool inline CResultWriter::bWriteToFile<CTree>(const CResult<CTree, CError>& cResult, const std::string& sFileName)
{
    std::ofstream file(sFileName.c_str());
    if (!file.is_open()) return false;

    if (!cResult.bIsSuccess())
    {
        v_writeErrors(file, cResult.vGetErrors());
    }
    else
    {
        file << "--- SUKCES: DRZEWO W POSTACI PREFIKSOWEJ ---" << std::endl;
        CTree* pc_tree = cResult.cGetValue();
        pc_tree->v_printPrefix(file);
    }

    file.close();
    return true;
}

#endif