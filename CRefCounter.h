#ifndef CREFCOUNTER_H
#define CREFCOUNTER_H

#include <vector>

template <typename T>
class CMySmartPointer;

template <typename T>
class CRefCounter
{
private:
    int i_count;
    std::vector<CMySmartPointer<T>*> pv_all_pointers;

public:
    CRefCounter() : i_count(0) {}

    int iAdd() { return (++i_count); }
    int iDec() { return (--i_count); }
    int iGet() const { return i_count; }

    void v_add_pointer(CMySmartPointer<T>* p_sp) {
        pv_all_pointers.push_back(p_sp);
    }

    void v_remove_pointer(CMySmartPointer<T>* p_sp) {
        for (typename std::vector<CMySmartPointer<T>*>::iterator it = pv_all_pointers.begin();
             it != pv_all_pointers.end(); ++it)
        {
            if (*it == p_sp) {
                pv_all_pointers.erase(it);
                return;
            }
        }
    }

    std::vector<CMySmartPointer<T>*>& pv_get_all_pointers() {
        return pv_all_pointers;
    }
};

#endif