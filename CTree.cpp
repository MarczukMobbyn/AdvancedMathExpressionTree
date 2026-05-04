#include "CTree.h"
#include <cstdlib>
#include "CNode.h"
#include "COperator.h"
#include "CNumber.h"
#include "CVar.h"
#include <vector>
#include <cctype>
#include <sstream>
#include <iostream>
#include "CError.h"
#include "CResult.h"

int CTree::i_copyCount = 0;
int CTree::i_moveCount = 0;

CTree::CTree()
{
    p_root = NULL;
}

CTree::CTree(const CTree& r_other)
{
    i_copyCount++;

    if (r_other.p_root == NULL)
        p_root = NULL;
    else
        p_root = r_other.p_root->pc_clone();
}

CTree::CTree(CTree&& r_other)
{
    i_moveCount++;

    p_root = r_other.p_root;

    r_other.p_root = NULL;
}

CTree::~CTree()
{
    delete p_root;
}


CTree& CTree::operator=(const CTree& r_other)
{
    i_copyCount++;
    if (this != &r_other)
    {
        CNode* p_newRoot = NULL;

        if (r_other.p_root != NULL)
            p_newRoot = r_other.p_root->pc_clone();

        delete p_root;
        p_root = p_newRoot;
    }

    return *this;
}

CTree& CTree::operator=(CTree&& r_other)
{
    i_moveCount++;

    if (this == &r_other)
    {
        return *this;
    }

    delete p_root;

    p_root = r_other.p_root;

    r_other.p_root = NULL;

    return *this;
}


void CTree::v_setRoot(CNode* p_newRoot)
{
    delete p_root;
    p_root = p_newRoot;

    // Korzeń nie ma rodzica
    if (p_root != NULL)
    {
        p_root->v_setParent(NULL);
    }
}


void CTree::v_join(CTree& r_other)
{
    // Krok 1: Wygeneruj wynik operacji JOIN za pomocą operatora +
    CTree t_joined = *this + r_other;

    // Krok 2: Przypisz wynik do 'this', modyfikując je.
    *this = t_joined;

}

// tworzy NOWE drzewo, pozostawia THIS i r_other niezmienione.
CTree CTree::operator+(const CTree& r_other) const
{
    // Jeżeli jedno z drzew jest puste, zwracamy kopię drugiego.
    if (p_root == NULL)
        return r_other;


    if (r_other.p_root == NULL)
        return *this;

    // 1. Klonowanie T1 (this) z jednoczesnym wstawieniem T2
    bool b_isReplaced = false; // Flaga, aby podmienić tylko pierwszy liść

    // Używamy pc_joinClone do stworzenia NOWEGO korzenia:
    // klonuje *this, wstawiając w miejsce pierwszego liścia klon r_other.
    CNode* p_joinedRoot = p_root->pc_joinClone(r_other.p_root, b_isReplaced);

    // 2. Tworzymy i zwracamy nowy obiekt, którego p_root to p_joinedRoot.
    CTree t_joined;
    t_joined.v_setRoot(p_joinedRoot);

    return t_joined; // Zwracamy głęboką kopię (T1 + T2)
}

void CTree::v_print() const
{
    if (p_root != NULL)
    {
        p_root->v_print(std::cout);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "EMPTY TREE" << std::endl;
    }
}


void CTree::v_printPrefix(std::ostream& os) const
{
    if (p_root != NULL)
        p_root->v_print(os);
    else
        os << "EMPTY_TREE";
}

int CTree::i_countLeaves() const {
    if (p_root == NULL) {
        return 0;
    }
    return p_root->i_countLeaves();
}


std::vector<std::string> CTree::v_getVars() const
{
    std::set<std::string> s_vars;

    if (p_root != NULL)
        p_root->v_collectVars(s_vars);

    return std::vector<std::string>(s_vars.begin(), s_vars.end());
}

double CTree::d_comp(const std::vector<double>& v_values) const
{
    if (p_root == NULL)
        return 0.0;

    // pobieramy zmienne
    std::vector<std::string> v_names = v_getVars();

    // tworzymy mapę
    std::map<std::string,double> m_vals;

    for (size_t i = 0; i < v_names.size(); ++i)
    {
        if (i < v_values.size())
            m_vals[v_names[i]] = v_values[i];
        else
            m_vals[v_names[i]] = 0; // brak wartości -> 0
    }

    return p_root->d_eval(m_vals);
}


bool CTree::b_isNumber(const std::string& s_token)
{
    // Liczby składają się wyłącznie z cyfr i mogą być jedynie dodatnie.

    if (s_token.empty())
        return false;

    // Muszą to być wyłącznie cyfry
    for (size_t i = 0; i < s_token.length(); ++i)
    {
        if (!isdigit(s_token[i]))
            return false;
    }

    return true;
}

bool CTree::b_isOperator(const std::string& s_token)
{
    // Obsługuje operacje: +, -, *, /, sin, cos
    return s_token == OP_ADD || s_token == OP_SUB || s_token == OP_MUL || s_token == OP_DIV || s_token == OP_SIN || s_token == OP_COS;
}

bool CTree::b_isVariable(const std::string& s_token, std::string& s_validToken, std::vector<std::string>& v_messages)
{
    s_validToken = "";
    bool b_hasLetter = false;
    bool b_hasInvalidChar = false;

    for (size_t i = 0; i < s_token.length(); ++i)
    {
        char c = s_token[i];

        if (isalnum(c))
        {
            s_validToken += c;
            if (isalpha(c))
            {
                b_hasLetter = true;
            }
        }
        else
        {
            b_hasInvalidChar = true;
        }
    }

    if (s_validToken.empty() || !b_hasLetter)
    {
        return false;
    }

    // KOMUNIKAT BŁĘDU PRZEKAZYWANY WYŻEJ
    if (b_hasInvalidChar)
    {
        std::string s_msg = "Ostrzezenie: Wyrazenie '" + s_token + "' zawieralo niedozwolone znaki. Zostaly one zignorowane. Zmienna zostanie przetworzona jako: '" + s_validToken + "'";
        v_messages.push_back(s_msg);
    }

    return true;
}


CNode* CTree::pc_parse(const std::vector<std::string>& v_tokens, int& i_index, std::vector<std::string>& r_messages)
{
    if (i_index >= (int)v_tokens.size())
        return NULL;

    std::string s_tok = v_tokens[i_index];
    i_index++;

    // 1. operator
    if (b_isOperator(s_tok))
    {
        COperator* p_op = new COperator(s_tok);
        int argCount = p_op->i_getArity();

        for (int i = 0; i < argCount; i++)
        {
            // ZMIANA: Przekazujemy r_messages do rekurencyjnego wywołania
            CNode* p_child = pc_parse(v_tokens, i_index, r_messages);

            // Naprawianie: jeśli brakuje argumentu, uzupełnij domyślną wartością 0
            if (!p_child)
            {
                p_child = new CNumber(0);
                // KOMUNIKAT BŁĘDU PRZEKAZYWANY WYŻEJ
                const std::string s_msg = "Ostrzezenie: Brakuje argumentu dla operatora '" + s_tok + "'. Uzupełniono wartoscia '0'.";
                r_messages.push_back(s_msg);
            }
            p_op->v_addChild(p_child);
        }

        return p_op;
    }

    // 2. liczba
    if (b_isNumber(s_tok))
    {
        double d_val = 0.0;
        std::stringstream ss(s_tok);
        if (!(ss >> d_val)) {
            // KOMUNIKAT BŁĘDU PRZEKAZYWANY WYŻEJ
            const std::string s_msg = "Blad: Nie mozna sparsowac liczby: '" + s_tok + "'. Uzyto wartosci 0.";
            r_messages.push_back(s_msg);
        }
        return new CNumber(d_val);
    }


    // 3. zmienna
    std::string s_validName;
    // Przekazujemy r_messages do b_isVariable
    if (b_isVariable(s_tok, s_validName, r_messages))
    {
        return new CVar(s_validName);
    }


    // Jeśli token nie jest ani operatorem, ani poprawną liczbą, ani poprawną zmienną
    // KOMUNIKAT BŁĘDU PRZEKAZYWANY WYŻEJ
    const std::string s_msg = "Blad: Nieznany lub niepoprawny token: '" + s_tok + "'. Zostanie zignorowany/uzupelniony.";
    r_messages.push_back(s_msg);
    return NULL;
}


void CTree::v_enter(const std::string& s_expression, std::vector<std::string>& r_messages)
{
    // 1. Tokenizacja
    std::vector<std::string> v_tokens;
    std::stringstream ss(s_expression);
    std::string s_tok;
    while (ss >> s_tok)
    {
        v_tokens.push_back(s_tok);
    }

    // 2. Parsowanie (Przekazujemy wektor komunikatów)
    int i_index = 0;
    CNode* p_newRoot = pc_parse(v_tokens, i_index, r_messages);

    // 3. Obsługa nadmiarowych tokenów (jeśli parser skończył, a zostały tokeny)
    if (i_index < (int)v_tokens.size())
    {
        std::string s_skippedTokens;
        for (int i = i_index; i < (int)v_tokens.size(); ++i)
        {
            s_skippedTokens += v_tokens[i] + " ";
        }
        const std::string s_msg = "Ostrzezenie: Po poprawnym sparsowaniu wyrazenia pozostaly nadmiarowe symbole: " + s_skippedTokens;
        r_messages.push_back(s_msg);
    }

    // 4. Wymiana korzenia
    v_setRoot(p_newRoot); // v_setRoot delete'uje stary p_root
}


CResult<CTree, CError> CTree::c_parseExpression(const std::string& s_expression)
{
    // Wektor do zbierania błędów i ostrzeżeń jako obiektów CError.
    std::vector<CError*> v_errors;

    // Używamy tymczasowego wektora stringów, który przekażemy do istniejącego parsera CNode*
    std::vector<std::string> v_messages;

    // 1. Tokenizacja
    std::vector<std::string> v_tokens;
    std::stringstream ss(s_expression);
    std::string s_tok;
    while (ss >> s_tok)
    {
        v_tokens.push_back(s_tok);
    }

    // Walidacja pustego wyrażenia
    if (v_tokens.empty())
    {
        v_errors.push_back(new CError("Wyrazenie nie moze byc puste."));
        // Porażka, zwracamy błędy (Drzewo jest NULL)
        return CResult<CTree, CError>::cFail(v_errors);
    }

    // 2. Parsowanie
    int i_index = 0;
    // funkcja pc_parse zbiera wszystkie komunikaty (ostrzeżenia i błędy) do v_messages.
    CNode* p_newRoot = pc_parse(v_tokens, i_index, v_messages);

    // 3. Obsługa nadmiarowych tokenów
    if (i_index < (int)v_tokens.size())
    {
        std::string s_skippedTokens;
        for (int i = i_index; i < (int)v_tokens.size(); ++i)
        {
            s_skippedTokens += v_tokens[i] + " ";
        }
        const std::string s_msg = "Ostrzezenie: Po poprawnym sparsowaniu wyrazenia pozostaly nadmiarowe symbole: " + s_skippedTokens;
        v_messages.push_back(s_msg);
    }

    // 4. Przekształcenie Komunikatów (string -> CError)
    for (size_t i = 0; i < v_messages.size(); ++i)
    {
        v_errors.push_back(new CError(v_messages[i]));
    }

    // 5. Decyzja o Wyniku
    if (p_newRoot != NULL)
    {
        // 1. Tworzymy tymczasowy OBIEKT CTree (na stosie)
        CTree temp_tree;

        // 2. Przekazujemy mu korzeń (v_setRoot automatycznie dealokuje stary korzeń, jeśli istnieje)
        temp_tree.v_setRoot(p_newRoot);

        // 3. Zwracamy CResult z OBIEKTEM temp_tree.
        return CResult<CTree, CError>::cOk(temp_tree);
    }
    else
    {
        // PORAŻKA (p_newRoot == NULL)
        if (v_errors.empty()) {
            v_errors.push_back(new CError("Blad krytyczny: Parsowanie zakonczone niepowodzeniem, nie zidentyfikowano szczegolowego bledu."));
        }

        // Zwracamy Porażkę z listą błędów.
        return CResult<CTree, CError>::cFail(v_errors);
    }
}





