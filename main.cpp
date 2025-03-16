#include <iostream>
#include <limits>
#include "hashtable.h"
#include "menu.h"

// <---------------------- MENÜ AUSGABE ---------------------->
void printMenu() {
    std::cout << "\n========== Aktienverwaltung ==========\n";
    std::cout << "1) ADD\n";
    std::cout << "2) DEL\n";
    std::cout << "3) IMPORT\n";
    std::cout << "4) SEARCH\n";
    std::cout << "5) PLOT\n";
    std::cout << "6) SAVE\n";
    std::cout << "7) LOAD\n";
    std::cout << "8) QUIT\n";
    std::cout << "=======================================\n";
    std::cout << "Bitte auswählen: ";
}

// <---------------------- MAIN FUNKTION ---------------------->
int main() {
    HashTable ht;

    bool running = true;
    while (running) {
        printMenu();
        int choice;
        std::cin >> choice;

        if (!std::cin) {
            // Eingabefehler abfangen
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ungültige Eingabe!\n";
            continue;
        }

        switch (choice) {
            case 1:
                handleAdd(ht);
                break;
            case 2:
                handleDel(ht);
                break;
            case 3:
                handleImport(ht);
                break;
            case 4:
                handleSearch(ht);
                break;
            case 5:
                handlePlot(ht);
                break;
            case 6:
                handleSave(ht);
                break;
            case 7:
                handleLoad(ht);
                break;
            case 8:
                std::cout << "Programm beendet.\n";
                running = false;
                break;
            default:
                std::cout << "Ungültige Auswahl!\n";
                break;
        }
    }

    return 0;
}
