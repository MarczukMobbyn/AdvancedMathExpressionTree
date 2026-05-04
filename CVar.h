#ifndef CVAR_H
#define CVAR_H

#include "CNode.h"
#include <string>

class CVar : public CNode
{
private:
    std::string s_name;

public:
    CVar(const std::string& r_name);
    CVar(const CVar& r_other);

    virtual CNode* pc_clone() const;
    virtual void v_print(std::ostream& r_os) const;
    virtual double d_eval(const std::map<std::string,double>& m_vars) const;
    virtual void v_collectVars(std::set<std::string>& r_vars) const;
    virtual CNode* pc_joinClone(CNode* p_subtreeToInsert, bool& r_isReplaced) const;

    virtual int i_countLeaves() const;

    virtual ~CVar() {}
};

#endif
