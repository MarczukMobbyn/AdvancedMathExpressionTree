#ifndef CNODE_H
#define CNODE_H

#include <iostream>
#include <map>
#include <set>

class CNode {
private:
    CNode* p_parent;
public:
    CNode() { p_parent = NULL; }
    virtual ~CNode() {}

    // METODY DO ZARZĄDZANIA RODZICEM
    void v_setParent(CNode* p_newParent) { p_parent = p_newParent; }
    CNode* pc_getParent() const { return p_parent; }

    // klonowanie poddrzewa
    virtual CNode* pc_clone() const = 0;

    // Klonowanie i wstawianie poddrzewa (używane przez JOIN)
    // p_subtreeToInsert to korzeń T2. r_isReplaced to flaga, która zapewnia,
    // że tylko pierwszy liść zostanie zamieniony.
    virtual CNode* pc_joinClone(CNode* p_subtreeToInsert, bool& r_isReplaced) const = 0;

    // wypisanie w formie prefixowej
    virtual void v_print(std::ostream& r_os) const = 0;

    virtual int i_countLeaves() const = 0;

    // obliczanie wartości
    virtual double d_eval(const std::map<std::string,double>& m_vars) const = 0;

    // zbieranie zmiennych
    virtual void v_collectVars(std::set<std::string>& r_vars) const = 0;

};

#endif
