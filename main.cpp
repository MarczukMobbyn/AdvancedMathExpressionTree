#include "CInterface.h"
#include <iostream>
#include <utility>

#include "CMySmartPointer.h"

void v_test() {
    CMySmartPointer<int> *pc_ptr;
    std::cout << "--- START TESTU LISTY WSKAZNIKOW ---" << std::endl;

    if (3>2) {
        CMySmartPointer<int> c_orig(new int(123)); // 1.
        CMySmartPointer<int> c_copy(c_orig);      // 2.
        pc_ptr = new CMySmartPointer<int>(c_copy);// 3.

        std::vector<CMySmartPointer<int>*> v_list_before = c_orig.pv_get_all_this();
        std::cout << "W Bloku: Liczba wspoldzielonych wskaznikow: " << v_list_before.size() << " (Oczekiwano: 3)" << std::endl;

    }

    std::vector<CMySmartPointer<int>*> v_list_after = pc_ptr->pv_get_all_this();
    std::cout << "Po Bloku: Liczba wspoldzielonych wskaznikow: " << v_list_after.size() << " (Oczekiwano: 1)" << std::endl;

    delete pc_ptr;
    std::cout << "--- KONIEC TESTU ---" << std::endl;
}

int main()
{
    // std::cout << "--- APLIKACJA DRZEW MATEMATYCZNYCH (TEP) ---" << std::endl;
    // CInterface interface;
    // interface.v_run();

    // std::cout << "--- TEST Z INT: Licznik odwolania ---" << std::endl;
    //
    // // 1. Inicjalizacja: pc_ptr1 jest jedynym wskaźnikiem na nowy int o wartości 42.
    // CMySmartPointer<int> pc_ptr1(new int(42));
    // std::cout << "Wartosc pc_ptr1: " << *pc_ptr1 << " licznik: " << pc_ptr1.iGetCount() << std::endl;
    //
    // // 2. Kopiowanie: pc_ptr2 jest kopia, licznik powinien wzrosnąć.
    // CMySmartPointer<int> pc_ptr2 = pc_ptr1;
    // std::cout << "Wartosc pc_ptr2: " << *pc_ptr2 << " licznik: " << pc_ptr2.iGetCount() <<std::endl;
    //
    // // 3. Przypisanie: pc_ptr3 wczesniej wskazywal na 99, teraz bedzie wskazywal na 42.
    // // 99 zostanie skasowane, a licznik dla 42 wzrośnie.
    // CMySmartPointer<int> pc_ptr3(new int(99));
    // std::cout << "Wartosc pc_ptr3 przed przypisaniem: " << *pc_ptr3 << " licznik: " << pc_ptr3.iGetCount() <<std::endl;
    //
    // pc_ptr3 = pc_ptr1;
    // std::cout << "Wartosc pc_ptr3 po przypisaniu: " << *pc_ptr3 << " licznik: " << pc_ptr3.iGetCount() <<std::endl;
    //
    // // Modyfikacja przez jeden wskaźnik wpływa na wszystkie
    // *pc_ptr1 = 100;
    //
    // std::cout << "Wartosc pc_ptr1 po modyfikacji: " << *pc_ptr1 << std::endl;
    // std::cout << "Wartosc pc_ptr2 po modyfikacji: " << *pc_ptr2 << std::endl;
    // std::cout << "Wartosc pc_ptr3 po modyfikacji: " << *pc_ptr3 << std::endl;
    //
    // // W tym momencie:
    // // Pamięć dla 99 została zwolniona (cicho, przez delete).
    // // Pamięć dla 100 zostanie zwolniona dopiero, gdy pc_ptr1, pc_ptr2 i pc_ptr3 wyjdą z zakresu (cicho, przez delete).
    //
    // std::cout << "--- Koniec zakresu. Pamiec zostanie zwolniona ---" << std::endl;

    // CTree a, b;
    // std::vector<std::string> msgs;
    //
    // a.v_enter("+ x 1", msgs);
    // b.v_enter("* x 2", msgs);
    //
    // CTree::i_copyCount = 0;
    // CTree::i_moveCount = 0;
    //
    // CTree c = a + b;
    // a = std::move(c);
    //
    // std::cout << "Copies: " << CTree::i_copyCount << std::endl;
    // std::cout << "Moves: " << CTree::i_moveCount << std::endl;

    v_test();

    return 0;
}