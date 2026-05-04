#ifndef CTREE_H
#define CTREE_H

#include "CNode.h"
#include "COperator.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "CError.h"

template <typename T, typename E>
class CResult;

class CTree
{
private:
    CNode* p_root;

public:
    CTree();
    CTree(const CTree& r_other);
    CTree(CTree&& r_other);
    CTree &operator=(const CTree& r_other);
    CTree& operator=(CTree&& r_other);
    ~CTree();


    void v_print() const;
    //printuje drzewo na podane wyjście
    void v_printPrefix(std::ostream& os) const;
    std::vector<std::string> v_getVars() const;
    double d_comp(const std::vector<double>& v_values) const;

    CTree operator+(const CTree& r_other) const;

    // Właściwa operacja join, która modyfikuje drzewo THIS i ZERUJE drzewo źródłowe
    void v_join(CTree& r_other);

    // ustawianie nowego drzewa (używane przez parser)
    void v_setRoot(CNode* p_newRoot);
    CNode* pc_parse(const std::vector<std::string>& v_tokens, int& i_index, std::vector<std::string>& r_messages);

    //sprawdzamy czy drzewo jest puste
    bool b_isEmpty() const { return p_root == NULL; }

    // funkcje do sprawdzania czym jest token
    static bool b_isVariable(const std::string& s_token, std::string& s_validToken, std::vector<std::string>& r_messages);
    bool b_isNumber(const std::string& s_token);
    bool b_isOperator(const std::string& s_token);

    //modyfikacja
    int i_countLeaves() const;

    // Wczytywanie wyrażenia z obsługą komunikatów
    void v_enter(const std::string& s_expression, std::vector<std::string>& r_messages);

    //wczytywanie wyrażenia zwracające CResult
    CResult<CTree, CError> c_parseExpression(const std::string& s_expression);

    static int i_copyCount;
    static int i_moveCount;

};

#endif
