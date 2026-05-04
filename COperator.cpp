#include "COperator.h"
#include <iostream>
#include <cmath>
#include <map>
#include <set>

#include "CNumber.h"
#include "CTreeException.h"
#include "CVar.h"

COperator::COperator(const std::string& s_opName)
{
    s_op = s_opName;

    if (s_op == OP_SIN || s_op == OP_COS)
        i_arity = 1;
    else if (s_op == OP_ADD|| s_op == OP_SUB || s_op == OP_MUL|| s_op == OP_DIV)
        i_arity = 2;
    else
        i_arity = 0;  // nieznany operator
}


COperator::COperator(const COperator& r_other)
{
    s_op = r_other.s_op;
    i_arity = r_other.i_arity;

    // GŁĘBOKA kopia dzieci
    for (size_t i = 0; i < r_other.v_children.size(); ++i)
    {
        CNode* p_clone = r_other.v_children[i]->pc_clone();
        p_clone->v_setParent(this); //Ustawienie nowego rodzica dla klonowanego dziecka
        v_children.push_back(p_clone);
    }
}


int COperator::i_countLeaves() const {

    if (v_children.empty()) {
        return 1;
    }

    int count = 0;

    for (CNode* p_child : v_children) {

        count += p_child->i_countLeaves();
    }

    return count;
}

void COperator::v_addChild(CNode* p_child)
{
    if (p_child != NULL)
    {
        p_child->v_setParent(this); // Ustawienie rodzica dla nowego dziecka
    }
    v_children.push_back(p_child);
}

CNode* COperator::pc_joinClone(CNode* p_subtreeToInsert, bool& r_isReplaced) const
{
    COperator* p_newOp = new COperator(s_op);

    for (size_t i = 0; i < v_children.size(); ++i)
    {
        CNode* p_childClone;

        if (!r_isReplaced)
        {
            // Próbujemy klonować z warunkiem podmiany.
            p_childClone = v_children[i]->pc_joinClone(p_subtreeToInsert, r_isReplaced);
        }
        else
        {
            // Liść już wymieniony, wykonujemy standardową kopię.
            p_childClone = v_children[i]->pc_clone();
        }

        p_newOp->v_children.push_back(p_childClone);
        p_childClone->v_setParent(p_newOp); // NAPRAWA WSKAŹNIKA RODZICA
    }

    return p_newOp;
}

CNode* COperator::pc_clone() const
{
    return new COperator(*this);
}

void COperator::v_print(std::ostream& r_os) const
{
    r_os << s_op;

    for (size_t i = 0; i < v_children.size(); ++i)
    {
        r_os << " ";
        v_children[i]->v_print(r_os);
    }
}

double COperator::d_eval(const std::map<std::string,double>& m_vars) const
{
    // funkcje jednoargumentowe
    if (s_op == OP_SIN)
        return std::sin(v_children[0]->d_eval(m_vars));

    if (s_op == OP_COS)
        return std::cos(v_children[0]->d_eval(m_vars));

    // operacje dwuargumentowe
    double d_left  = v_children[0]->d_eval(m_vars);
    double d_right = v_children[1]->d_eval(m_vars);

    if (s_op == OP_ADD) return d_left + d_right;
    if (s_op == OP_SUB) return d_left - d_right;
    if (s_op == OP_MUL) return d_left * d_right;
    if (s_op == OP_DIV)
    {
        if (d_right == 0.0)
        {
            throw CDivisionByZeroException();
        }
        return d_left / d_right;
    }

    return 0.0; // gdy operator nieznany
}

void COperator::v_collectVars(std::set<std::string>& r_vars) const
{
    for (size_t i = 0; i < v_children.size(); ++i)
        v_children[i]->v_collectVars(r_vars);
}

COperator::~COperator()
{
    // usuwamy WSZYSTKIE dzieci
    for (size_t i = 0; i < v_children.size(); ++i)
        delete v_children[i];
}
