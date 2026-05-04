#ifndef COPERATOR_H
#define COPERATOR_H

#define OP_ADD      "+"
#define OP_SUB      "-"
#define OP_MUL      "*"
#define OP_DIV      "/"
#define OP_SIN      "sin"
#define OP_COS      "cos"

#include "CNode.h"
#include <vector>
#include <string>

class COperator : public CNode
{
private:
    std::string s_op;                  // np. "+", "-", "*", "/", "sin", "cos"
    std::vector<CNode*> v_children;    // lista argumentów operatora
    int i_arity;   // liczba argumentów operatora


public:
    COperator(const std::string& r_op);
    COperator(const COperator& r_other);

    virtual int i_countLeaves() const;

    void v_addChild(CNode* p_child);

    // getter — parser pyta o liczbę argumentów
    int i_getArity() const { return i_arity; }

    virtual CNode* pc_joinClone(CNode* p_subtreeToInsert, bool& r_isReplaced) const;

    virtual CNode* pc_clone() const;
    virtual void v_print(std::ostream& r_os) const;
    virtual double d_eval(const std::map<std::string,double>& m_vars) const;
    virtual void v_collectVars(std::set<std::string>& r_vars) const;

    virtual ~COperator();
};

#endif
