#ifndef CMYSMARTPOINTER_H
#define CMYSMARTPOINTER_H
#include "CRefCounter.h"

template <typename T>
class CMySmartPointer
{
public:
    CMySmartPointer (T *pcPointer)
    {
        pc_pointer = pcPointer;
        pc_counter = new CRefCounter<T>();
        pc_counter->iAdd();
        pc_counter->v_add_pointer(this);
    }

    CMySmartPointer (const CMySmartPointer &pcOther)
    {
        pc_pointer = pcOther.pc_pointer;
        pc_counter = pcOther.pc_counter;
        pc_counter->iAdd();
        pc_counter->v_add_pointer(this);
    }
    ~CMySmartPointer()
    {
        pc_counter->v_remove_pointer(this);

        if (pc_counter->iDec() == 0)
        {
            delete pc_pointer;
            delete pc_counter;
        }
    }

    CMySmartPointer& operator=(const CMySmartPointer &pcOther)
    {
        if (this == &pcOther)
        {
            return *this;
        }

        pc_counter->v_remove_pointer(this);

        if (pc_counter->iDec() == 0)
        {
            // Był to ostatni wskaźnik, musimy zwolnić pamięć.
            delete pc_pointer;
            delete pc_counter;
        }

        pc_pointer = pcOther.pc_pointer;
        pc_counter = pcOther.pc_counter;

        pc_counter->v_add_pointer(this);

        pc_counter->iAdd();

        return *this;
    }

    int iGetCount() const {
        return pc_counter->iGet();
    }

    T& operator*() { return (*pc_pointer); }
    T* operator->() { return (pc_pointer); }

    std::vector<CMySmartPointer<T>*> pv_get_all_this() {
        return pc_counter->pv_get_all_pointers();
    }



private:
    CRefCounter<T> *pc_counter;
    T *pc_pointer;
};

#endif