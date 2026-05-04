#include "CInterface.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "CTreeException.h"

// Pomocnicza funkcja do czyszczenia bufora wejścia
void v_clearInput() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

CInterface::CInterface() {
    // Konstruktor
}

void CInterface::v_printMessages(const std::vector<std::string>& r_messages) const {
    for (size_t i = 0; i < r_messages.size(); ++i) {
        std::cout << "   [INFO/" << ERR_BLAD << "]: " << r_messages[i] << std::endl;
    }
}

void CInterface::v_showMenu() const {
    std::cout << "\n---------------- MENU ----------------" << std::endl;
    std::cout << MENU_ENTER << ". " << CMD_ENTER << " - Wprowadz formule" << std::endl;
    std::cout << MENU_COMP << ". " << CMD_COMP << "  - Oblicz wartosc" << std::endl;
    std::cout << MENU_JOIN << ". " << CMD_JOIN << "  - Wstaw poddrzewo" << std::endl;
    std::cout << MENU_PRINT << ". " << CMD_PRINT << " - Wypisz obecne drzewo" << std::endl;
    std::cout << MENU_VARS << ". " << CMD_VARS << " - Wypisz wszystkie zmienne w drzewie" << std::endl;
    std::cout << MENU_LEAVES << ". " << CMD_LEAVES << " - Wypisz liczbe lisci w drzewie" << std::endl;
    std::cout << MENU_SAVE << ". " << CMD_SAVE << " - Zapisz stan drzewa do pliku (Po save wpisz nazwe pliku)" << std::endl;
    std::cout << MENU_EXIT << ". " << CMD_EXIT << "  - Wyjscie" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}

void CInterface::v_handleEnter(std::stringstream& ss) {
    std::string s_formula;
    // Pobierz całą resztę linii ze strumienia jako formułę
    std::getline(ss, s_formula);

    // Usuń ewentualne wiodące spacje, które pozostały po komendzie "ENTER"
    s_formula.erase(0, s_formula.find_first_not_of(" \t\n\r\f\v"));

    if (s_formula.empty()) {
        std::cout << "Blad: Brak wyrazenia do wprowadzenia po komendzie ENTER." << std::endl;
        return;
    }

    // 1. WYWOŁANIE NOWEJ FUNKCJI PARSOWANIA
    CResult<CTree, CError> c_result = c_tree.c_parseExpression(s_formula);

    // 2. PRZECHWYCENIE KOMUNIKATÓW (Ostrzeżenia/Błędy)
    std::vector<std::string> v_messages;
    const std::vector<CError*>& v_errors = c_result.vGetErrors();

    // Konwersja CError* na std::string
    for (size_t i = 0; i < v_errors.size(); ++i) { //
        // Użyj indeksowania:
        v_messages.push_back(v_errors[i]->s_getMessage());
    }

    // 3. OBSŁUGA WYNIKU
    if (c_result.bIsSuccess()) {
        // SUKCES

        c_tree = *c_result.cGetValue(); // Ustawiamy drzewo na sparsowane drzewo

        std::cout << "Drzewo zostalo pomyslnie wprowadzone." << std::endl;
        v_printMessages(v_messages); // Wypisujemy ewentualne ostrzeżenia
    } else {
        // PORAŻKA
        std::cout << "Blad parsowania. Nie wprowadzono zadnego drzewa." << std::endl;
        v_printMessages(v_messages);
    }

    // Wypisanie aktualnego stanu drzewa (nawet jeśli puste)
    v_handlePrint();
}


void CInterface::v_handleComp(std::stringstream& ss) {
    if (c_tree.b_isEmpty()) {
        std::cout << "Blad: Nie wprowadzono zadnej formuly." << std::endl;
        return;
    }

    // 1. Pobieranie listy zmiennych
    std::vector<std::string> v_vars = c_tree.v_getVars();

    if (v_vars.empty()) {
        // Jeśli nie ma zmiennych, po prostu oblicz i wypisz wynik
        try {
            // Zakładam, że CDivisionByZeroException jest zdefiniowany
            std::cout << ">> WYNIK: " << std::fixed << std::setprecision(6) << c_tree.d_comp(std::vector<double>()) << std::endl;
        }
        catch (...) { // Używamy catch(...) jako ogólnej obsługi, włączając CDivisionByZeroException
            std::cout << "Blad obliczeniowy! Wyjatek CDivisionByZeroException lub inny." << std::endl;
        }
        return;
    }

    // 2. Wczytywanie wartości zmiennych z strumienia ss
    std::cout << "\nDrzewo wymaga " << v_vars.size() << " wartosci dla zmiennych: ";
    for (const std::string& var : v_vars) {
        std::cout << var << " ";
    }
    std::cout << std::endl;

    std::vector<double> v_values;
    double d_val;

    // Pobieramy wszystkie wartości double dostępne w strumieniu ss
    while (ss >> d_val) {
        v_values.push_back(d_val);
    }

    // Sprawdzenie błędów
    if (v_values.size() != v_vars.size()) {
        std::cout << "Blad: Podano " << v_values.size()
                  << " wartosci, ale wymagane jest " << v_vars.size() << " wartosci." << std::endl;

        // Jeśli ss.fail() jest true (ale nie ss.eof()), oznacza to, że w linii były nieczytelne dane (np. tekst po liczbach)
        if (ss.fail() && !ss.eof()) {
            std::cout << "   Uwaga: W linii wejscia wystapil blad formatu (oczekiwano liczb)." << std::endl;
        }
        return;
    }

    // 3. Obliczanie
    try {
        // Obliczanie
        std::cout << ">> WYNIK: " << std::fixed << std::setprecision(6) << c_tree.d_comp(v_values) << std::endl;
    }
    catch (...) { // Używamy catch(...) jako ogólnej obsługi, włączając CDivisionByZeroException
        std::cout << "\nBLAD OBLICZENIOWY: Wyjatek CDivisionByZeroException lub inny." << std::endl;
    }
}


void CInterface::v_handleJoin(std::stringstream& ss) {
    if (c_tree.b_isEmpty()) {
        std::cout << "Blad: Drzewo docelowe (T1) jest puste. Wprowadz najpierw wyrazenie (ENTER)." << std::endl;
        return;
    }

    // 1. Pobranie reszty linii jako wyrażenie T2
    std::string s_formula_t2;
    // Pobierz całą resztę linii ze strumienia jako formułę
    std::getline(ss, s_formula_t2);
    // Usuń ewentualne wiodące spacje
    s_formula_t2.erase(0, s_formula_t2.find_first_not_of(" \t\n\r\f\v"));

    if (s_formula_t2.empty()) {
        std::cout << "Blad: Brak wyrazenia (T2) do dolaczenia." << std::endl;
        return;
    }

    // 2. Parsowanie wyrażenia T2 do tymczasowego drzewa
    CTree t2;
    std::vector<std::string> v_messages;

    // Wywołanie parsowania, które zwróci CResult<CTree*, CError>
    CResult<CTree, CError> c_result_t2 = t2.c_parseExpression(s_formula_t2); // Wywołujemy na tymczasowej t2, ale to ZWROTKA jest ważna

    const std::vector<CError*>& v_errors = c_result_t2.vGetErrors();

    // Konwersja CError* na std::string
    for (CError* pc_err : v_errors) {
        v_messages.push_back(pc_err->s_getMessage());
    }
    v_printMessages(v_messages); // Wypisujemy ewentualne błędy/ostrzeżenia

    if (!c_result_t2.bIsSuccess()) {
        std::cout << "Blad: Nie mozna sparsowac wyrazenia T2. Operacja JOIN anulowana." << std::endl;
        return; // Anulujemy operację, jeśli T2 jest niepoprawne
    }

    // Jeśli sukces, to nowo sparsowane drzewo jest w c_result_t2.cGetValue().
    // Musimy skopiować je do naszej tymczasowej zmiennej 't2'.
    CTree* pc_parsedTree_t2 = c_result_t2.cGetValue();
    t2 = *pc_parsedTree_t2;


    // 3. Wyświetlenie stanu i wykonanie operacji JOIN
    std::cout << "Wstawienie poddrzewa (JOIN) modyfikuje obecne drzewo, wstawiajac formule 't2' w pierwszy napotkany lisc 't1'." << std::endl;

    std::cout << "\nT1 (Przed join): "; c_tree.v_print(); std::cout << std::endl;
    std::cout << "T2 (Do wstawienia): "; t2.v_print(); std::cout << std::endl;

    c_tree.v_join(t2); // Wywołanie logiki JOIN (która używa operatorów + i =)
    
    std::cout << "\n-- Wynik --" << std::endl;
    std::cout << "T1 (Po join): "; c_tree.v_print(); std::cout << std::endl;
    std::cout << "T2 (Bez zerowania): "; t2.v_print(); std::cout << std::endl;
}

void CInterface::v_handlePrint() const {
    std::cout << "Aktualne drzewo (Prefix): ";
    c_tree.v_print();
    std::cout << std::endl;
}

void CInterface::v_handleVars() const {
    if (c_tree.b_isEmpty()) {
        std::cout << "Blad: Nie wprowadzono zadnej formuly." << std::endl;
        return;
    }

    std::vector<std::string> v_vars = c_tree.v_getVars();

    if (v_vars.empty()) {
        std::cout << "Brak zmiennych w drzewie." << std::endl;
        return;
    }

    std::cout << "Znalezione zmienne: ";
    for (size_t i = 0; i < v_vars.size(); ++i) {
        std::cout << v_vars[i];
        if (i < v_vars.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
}

void CInterface::v_handleLeaves() const {
    std:: cout << "Ilosc isci w drzewie wynosi: " << c_tree.i_countLeaves() << std::endl;
}

void CInterface::v_handleSave(std::stringstream& ss) const {
    std::string s_fileName;

    // Pobierz nazwę pliku
    if (!(ss >> s_fileName)) {
        std::cout << "Blad: Nie podano nazwy pliku." << std::endl;
        return;
    }

    if (c_tree.b_isEmpty()) {
        std::cout << "Blad: Nie mozna zapisac stanu - drzewo jest puste. Wprowadz najpierw formule." << std::endl;
        return;
    }

    // 1. Utworzenie obiektu CResult<CTree, CError> w stanie SUKCESU
    // CResult::cOk(c_tree) tworzy CResult, który przechowuje głęboką kopię (CTree)
    CResult<CTree, CError> c_result = CResult<CTree, CError>::cOk(c_tree);

    // 2. Wywołanie CResultWriter::bWriteToFile
    CResultWriter writer;

    // Musimy użyć bWriteToFile<CTree> dla tej specjalizacji
    if (writer.bWriteToFile<CTree>(c_result, s_fileName)) {
        std::cout << "Stan drzewa zostal pomyslnie zapisany do pliku: " << s_fileName << std::endl;
    } else {
        std::cout << "Blad: Nie udalo sie zapisac stanu do pliku: " << s_fileName << std::endl;
    }

    // KONIEC: Destruktor c_result jest wywoływany automatycznie i zwalnia kopię c_tree.
}


void CInterface::v_run() {
    std::string s_line;
    std::string s_command;
    bool b_running = true;

    v_showMenu(); // Wyświetlamy menu raz na początku

    while (b_running) {
        std::cout << "\nKomenda (wpisz " << CMD_EXIT << ", aby wyjsc): ";

        // 1. Wczytanie całej linii wejścia
        if (!std::getline(std::cin, s_line)) {
            // W przypadku błędu wejścia (np. EOF / Ctrl+D), ustawiamy wyjście
            b_running = false;
            std::cout << "\nWylaczanie programu. Do zobaczenia!" << std::endl;
        }

        if (b_running) {
            // 2. Tokenizacja - wczytujemy pierwszą komendę
            std::stringstream ss(s_line);

            // Jeśli linia jest pusta lub składa się ze spacji, ss >> s_command zwróci false
            if (!(ss >> s_command)) {
                // Użytkownik wcisnął Enter, ignorujemy i kontynuujemy pętlę
            } else {
                // 3. Konwersja komendy na wielkie litery
                std::transform(s_command.begin(), s_command.end(), s_command.begin(), ::toupper);

                // 4. Obsługa komend
                if (s_command == "0" || s_command == CMD_EXIT) { // Użycie stałych
                    std::cout << "Wylaczanie programu. Do zobaczenia!" << std::endl;
                    b_running = false; // Zmieniamy warunek pętli
                }
                else {
                    // W tej implementacji, komendy tekstowe są mapowane na logikę
                    if (s_command == CMD_ENTER) {
                        v_handleEnter(ss);
                    }
                    else if (s_command == CMD_COMP) {
                        v_handleComp(ss);
                    }
                    else if (s_command == CMD_JOIN) {
                        v_handleJoin(ss);
                    }
                    else if (s_command == CMD_PRINT) {
                        v_handlePrint();
                    }
                    else if (s_command == CMD_VARS) {
                         // Należy upewnić się, że v_handleVars() jest zadeklarowane i zaimplementowane
                         v_handleVars();
                    }
                    else if (s_command == CMD_LEAVES) {
                        v_handleLeaves();
                    }
                    else if (s_command == CMD_SAVE) {
                        v_handleSave(ss);
                    }
                    else {
                        // Użycie stałej lub zdefiniowanie tego komunikatu jako stałej
                        std::cout << "Blad: Nieznana komenda '" << s_command << "'. Sprawdz menu." << std::endl;
                    }
                }
            }
        }
    }
}