#ifndef CNUMBER_H
#define CNUMBER_H

#include <iosfwd>

#include "CNode.h"


class CNumber : public CNode
{
private:
    double d_value;

public:
    CNumber(double d_val);
    CNumber(const CNumber& r_other);

    virtual CNode* pc_clone() const;
    virtual void v_print(std::ostream& r_os) const;
    virtual double d_eval(const std::map<std::string,double>& m_vars) const;
    virtual void v_collectVars(std::set<std::string>& r_vars) const;
    virtual CNode* pc_joinClone(CNode* p_subtreeToInsert, bool& r_isReplaced) const;

    virtual int i_countLeaves() const;

    virtual ~CNumber() {}
};

#endif
