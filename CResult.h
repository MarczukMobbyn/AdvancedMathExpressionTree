#ifndef CRESULT_H
#define CRESULT_H
#include <vector>

template <typename T, typename E>
class CResult
{
public:
    CResult(const T& c_value);
    CResult(E* pc_error);
    CResult(std::vector<E*>& v_errors);
    CResult(const CResult<T, E>& c_other);
    ~CResult();

    static CResult<T, E> cOk(const T& c_value);
    static CResult<T, E> cFail(E* pc_error);
    static CResult<T, E> cFail(std::vector<E*>& v_errors);

    CResult<T, E>& operator=(const CResult<T, E>& c_other);

    bool bIsSuccess() const;
    T* cGetValue() const;
    const std::vector<E*>& vGetErrors() const;

private:
    T *pc_value;
    std::vector<E*> v_errors;
};

template <typename E>
class CResult<void, E>
{
public:
    CResult();
    CResult (E *pc_error);
    CResult(std::vector<E*>& v_errors);
    CResult(const CResult<void, E>& c_other);
    ~CResult();

    static CResult<void, E> cOk();
    static CResult<void, E> cFail (E* pc_error);
    static CResult<void, E> cFail (std::vector<E*>& v_errors);

    CResult<void, E>& operator=(const CResult<void, E>& c_other);
    bool bIsSuccess() const;
    const std::vector<E*>& vGetErrors() const;

private:
    std::vector<E*> v_errors; // Brak pc_value
};

template <typename T, typename E>
CResult<T, E>::~CResult()
{
    if (pc_value != NULL)
    {
        delete pc_value;
    }

    // 2. Zwolnij pamięć dla obiektów błędów.
    for (int i = 0; i < v_errors.size(); ++i)
    {
        delete v_errors[i];
    }
}



template<typename T, typename E>
CResult<T, E>::CResult(const CResult<T, E> &c_other) {

    pc_value = NULL;

    // 1. Kopiowanie Wartości T
    if (c_other.pc_value != NULL)
    {
        pc_value = new T(*(c_other.pc_value));
    }

    // 2. Kopiowanie Błędów E*
    for (size_t i = 0; i < c_other.v_errors.size(); ++i)
    {
        E* pc_new_error = new E(*(c_other.v_errors[i]));
        v_errors.push_back(pc_new_error);
    }
}

template<typename T, typename E>
CResult<T, E> &CResult<T, E>::operator=(const CResult<T, E> &c_other) {
    // 1. Ochrona przed autodeklaracją (c1 = c1)
    if (this == &c_other) {
        return *this;
    }

    // 2. Zwolnienie obecnej pamięci
    if (pc_value != NULL) {
        delete pc_value;
        pc_value = NULL;
    }

    // Zwolnienie obiektów w wektorze
    for (size_t i = 0; i < v_errors.size(); ++i) {
        delete v_errors[i];
    }
    v_errors.clear(); // Opróżnienie wektora

    // 3. Wykonanie Głębokiej Kopii

    // Kopiowanie wartości T
    if (c_other.pc_value != NULL) {
        pc_value = new T(*(c_other.pc_value));
    }

    // Kopiowanie wektora błędów
    for (size_t i = 0; i < c_other.v_errors.size(); ++i) {
        E* pc_new_error = new E(*(c_other.v_errors[i]));
        v_errors.push_back(pc_new_error);
    }

    return *this;
}

template<typename T, typename E>
CResult<T, E>::CResult(const T& c_value) {
    pc_value = NULL;

    // Alokujemy nową pamięć i kopiujemy wartość.
    pc_value = new T(c_value);

    // v_errors jest domyślnie pusty.
}

template<typename T, typename E>
CResult<T, E>::CResult(E* pc_error) {
    pc_value = NULL;

    // Dodajemy wskaźnik do błędu (zakładamy, że został on zaalokowany dynamicznie
    v_errors.push_back(pc_error);
}

template<typename T, typename E>
CResult<T, E>::CResult(std::vector<E*>& v_otherErrors)
{
    // 1. Inicjalizacja
    pc_value = NULL;

    // 2. Głęboka Kopia Wskaźników Błędów
    for (size_t i = 0; i < v_otherErrors.size(); ++i)
    {
        E* pc_new_error = new E(*(v_otherErrors[i]));
        v_errors.push_back(pc_new_error);
    }
}

template<typename T, typename E>
CResult<T, E> CResult<T, E>::cOk(const T& c_value) {
    // Statyczna metoda tworząca stan sukcesu
    return CResult<T, E>(c_value);
}

template<typename T, typename E>
CResult<T, E> CResult<T, E>::cFail(E* pc_error) {
    // Statyczna metoda tworząca stan porażki (pojedynczy błąd)
    return CResult<T, E>(pc_error);
}

template<typename T, typename E>
CResult<T, E> CResult<T, E>::cFail(std::vector<E*>& v_errors) {
    // Statyczna metoda tworząca stan porażki (wektor błędów)
    return CResult<T, E>(v_errors);
}

template<typename T, typename E>
bool CResult<T, E>::bIsSuccess() const {
    // Zwraca true, gdy jest wartość i nie ma błędów
    return pc_value != NULL && v_errors.empty();
}

template<typename T, typename E>
T* CResult<T, E>::cGetValue() const{
    // Użytkownik musi teraz sprawdzić: if (CResult.ptGetValue() != NULL)

    return pc_value;
}

template<typename T, typename E>
const std::vector<E*>& CResult<T, E>::vGetErrors() const {
    // Zwraca referencję do wewnętrznego wektora błędów
    return v_errors;
}

template <typename E>
CResult<void, E>::CResult()
{
    // Sukces - pusty wektor błędów
}

template <typename E>
CResult<void, E>::CResult(E *pcError)
{
    // Porażka - dodanie pojedynczego błędu
    v_errors.push_back(pcError);
}

template <typename E>
CResult<void, E>::CResult(std::vector<E*>& vErrors)
{
    // Porażka - głęboka kopia wektora błędów
    for (size_t i = 0; i < vErrors.size(); ++i)
    {
        E* pc_new_error = new E(*(vErrors[i]));
        v_errors.push_back(pc_new_error);
    }
}

template <typename E>
CResult<void, E>::CResult(const CResult<void, E>& cOther)
{
    // Konstruktor kopiujący: głęboka kopia v_errors
    for (size_t i = 0; i < cOther.v_errors.size(); ++i)
    {
        E* pc_new_error = new E(*(cOther.v_errors[i]));
        v_errors.push_back(pc_new_error);
    }
}

template <typename E>
CResult<void, E>::~CResult()
{
    for (size_t i = 0; i < v_errors.size(); ++i)
    {
        delete v_errors[i];
    }
}

template<typename E>
CResult<void, E>& CResult<void, E>::operator=(const CResult<void, E>& cOther)
{
    // Operator przypisania: głęboka kopia
    if (this == &cOther) return *this;

    // 1. Zwolnienie obecnej pamięci
    for (size_t i = 0; i < v_errors.size(); ++i) delete v_errors[i];
    v_errors.clear();

    // 2. Głęboka Kopia
    for (size_t i = 0; i < cOther.v_errors.size(); ++i) {
        E* pc_new_error = new E(*(cOther.v_errors[i]));
        v_errors.push_back(pc_new_error);
    }

    return *this;
}

template <typename E>
CResult<void, E> CResult<void, E>::cOk()
{
    // Statyczna metoda sukcesu
    return CResult<void, E>();
}

template <typename E>
CResult<void, E> CResult<void, E>::cFail(E* pcError)
{
    return CResult<void, E>(pcError);
}

template <typename E>
CResult<void, E> CResult<void, E>::cFail(std::vector<E*>& vErrors)
{
    return CResult<void, E>(vErrors);
}

template <typename E>
bool CResult<void, E>::bIsSuccess() const
{
    // Sukces: wektor błędów jest pusty
    return v_errors.empty();
}

template <typename E>
const std::vector<E*>& CResult<void, E>::vGetErrors() const
{
    // Dostęp do wektora błędów
    return v_errors;
}


#endif
