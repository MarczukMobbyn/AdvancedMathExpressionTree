#include "CNumber.h"
#include <iostream>
#include <map>
#include <set>

CNumber::CNumber(double d_val)
{
    d_value = d_val;
}


CNumber::CNumber(const CNumber& r_other)
{
    d_value = r_other.d_value;
}


int CNumber::i_countLeaves() const {
    return 1;
}

CNode* CNumber::pc_clone() const
{
    return new CNumber(*this);
}

void CNumber::v_print(std::ostream& r_os) const
{
    r_os << d_value;
}

double CNumber::d_eval(const std::map<std::string,double>& m_vars) const
{
    return d_value;
}

// Klonowanie z podmianą (Dla liścia)
CNode* CNumber::pc_joinClone(CNode* p_subtreeToInsert, bool& r_isReplaced) const
{
    // Jeśli liść NIE ZOSTAL jeszcze wymieniony, to jest PIERWSZY liść!
    if (!r_isReplaced)
    {
        r_isReplaced = true;
        // ZWRACAMY głęboką kopię T2 (korzeń) zamiast siebie.
        return p_subtreeToInsert->pc_clone();
    }
    else
    {
        // Jeśli liść ZOSTAL już wymieniony, po prostu zwracamy kopię liścia (siebie).
        return new CNumber(*this);
    }
}

void CNumber::v_collectVars(std::set<std::string>& r_vars) const
{
    // liczba nie ma zmiennych -> nic nie robi
}
