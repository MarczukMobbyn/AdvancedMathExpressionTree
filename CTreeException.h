#ifndef CTREEEXCEPTION_H
#define CTREEEXCEPTION_H

#include <string>

// Klasa bazowa dla wszystkich wyjątków w projekcie
class CTreeException
{
private:
    std::string s_message;
public:
    CTreeException(const std::string& r_msg) : s_message(r_msg) {}
    
    // Metoda zwracająca komunikat błędu
    const char* what() const { return s_message.c_str(); }
};

// Specyficzny wyjątek dla dzielenia przez zero
class CDivisionByZeroException : public CTreeException
{
public:
    CDivisionByZeroException() : CTreeException("Blad: Proba dzielenia przez zero.") {}
};

#endif