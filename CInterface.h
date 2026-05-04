#ifndef CINTERFACE_H
#define CINTERFACE_H

#include <string>
#include <vector>
#include "CTree.h"
#include "CResultWriter.h"

// --- NUMERY OPCJI MENU (dla switch) ---
#define MENU_ENTER  1
#define MENU_COMP   2
#define MENU_JOIN   3
#define MENU_PRINT  4
#define MENU_VARS   5
#define MENU_LEAVES 6
#define MENU_SAVE   7
#define MENU_EXIT   0

// --- KOMENDY TEKSTOWE I STAŁE ---
#define CMD_ENTER   "ENTER"
#define CMD_COMP    "COMP"
#define CMD_JOIN    "JOIN"
#define CMD_PRINT   "PRINT"
#define CMD_EXIT    "EXIT"
#define CMD_VARS    "VARS"
#define CMD_LEAVES  "LEAVES"
#define CMD_SAVE    "SAVE"
#define ERR_BLAD    "Blad" // Stała dla sprawdzania komunikatów błędów

class CInterface
{
private:
    CTree c_tree;

    // Funkcje pomocnicze dla menu
    void v_showMenu() const;
    void v_handleEnter(std::stringstream& ss);
    void v_handleComp(std::stringstream& ss);
    void v_handleJoin(std::stringstream& ss);
    void v_handleVars() const;
    void v_handleLeaves() const;
    void v_handleSave(std::stringstream& ss) const;
    void v_handlePrint() const;

    // Wypisuje komunikaty zebrane z CTree
    void v_printMessages(const std::vector<std::string>& r_messages) const;

public:
    CInterface();
    void v_run();
};

#endif // CINTERFACE_H