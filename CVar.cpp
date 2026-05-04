#include "CVar.h"
#include <iostream>
#include <map>
#include <set>

CVar::CVar(const std::string& r_name)
{
    s_name = r_name;
}

CVar::CVar(const CVar& r_other)
{
    s_name = r_other.s_name;
}


int CVar::i_countLeaves() const {
    return 1;
}

CNode* CVar::pc_clone() const
{
    return new CVar(*this);
}

void CVar::v_print(std::ostream& r_os) const
{
    r_os << s_name;
}

// NOWA FUNKCJA: Klonowanie z podmianą (Dla liścia)
CNode* CVar::pc_joinClone(CNode* p_subtreeToInsert, bool& r_isReplaced) const
{
    // Jeśli liść NIE ZOSTAL jeszcze wymieniony, to jest PIERWSZY liść!
    if (!r_isReplaced)
    {
        r_isReplaced = true;
        // ZWRACAMY głęboką kopię T2 (korzeń) zamiast siebie.
        // To jest moment podmiany.
        return p_subtreeToInsert->pc_clone();
    }
    else
    {
        // Jeśli liść ZOSTAL już wymieniony, po prostu zwracamy kopię liścia (siebie).
        return new CVar(*this);
    }
}

double CVar::d_eval(const std::map<std::string,double>& m_vars) const
{
    // szukamy zmiennej w mapie
    std::map<std::string,double>::const_iterator it = m_vars.find(s_name);

    if (it == m_vars.end())
        return 0; // brak wartości -> zwróć 0 (bo nie wolno robić wyjątków)

    return it->second;
}

void CVar::v_collectVars(std::set<std::string>& r_vars) const
{
    r_vars.insert(s_name);
}
