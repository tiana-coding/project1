#include "hashtable.h"
#include <iostream>

// Konstruktor: Initialisiert die Hashtabelle mit der angegebenen Größe.
// Alle Slots werden initial auf EMPTY (leer) gesetzt.
HashTable::HashTable(size_t size)
    : tableSize(size), table(size, nullptr), status(size, SlotStatus::EMPTY) { }

// Destructor
HashTable::~HashTable() {
    clear();  // Free memory on destruction
}

// Clearing the entire HashTable
void HashTable::clear() {
    for (size_t i = 0; i < tableSize; i++) {
        if (status[i] == SlotStatus::OCCUPIED) {
            delete table[i];  // Free memory
            table[i] = nullptr;
        }
        status[i] = SlotStatus::EMPTY;  // Reset status
    }
}

// <---------------------- ADD ---------------------->

// Fügt eine Aktie in die Hashtabelle ein.
// 'key' wird als Schlüssel verwendet (z. B. WKN) und ist eindeutig.
bool HashTable::add(const Aktie& aktie, const std::string& key) {
    // Berechne den Startindex mithilfe der Hashfunktion und des Schlüssels.
    unsigned int startIndex = hashFunction(key, tableSize);
    int firstDeleted = -1; // -1 signalisiert, dass noch kein gelöschter Slot gefunden wurde

    // Durchlaufe alle Slots mittels quadratischer Sondierung.
    for (size_t i = 0; i < tableSize; i++) {
        size_t currentIndex = (startIndex + i * i) % tableSize;

        if (status[currentIndex] == SlotStatus::OCCUPIED) {
            // Überprüfe, ob bereits eine Aktie mit derselben WKN existiert (Duplikat)
            if (table[currentIndex]->wkn == aktie.wkn) {
                return false; // Duplikat gefunden – Eintrag nicht hinzufügen
            }
        } else if (status[currentIndex] == SlotStatus::DELETED && firstDeleted == -1) {
            // Merke den Index des ersten gelöschten Slots (für Lazy Deletion)
            firstDeleted = currentIndex;
        } else if (status[currentIndex] == SlotStatus::EMPTY) {  // Leerer Slot gefunden
            // Falls ein gelöschter Slot gefunden wurde, verwende diesen Slot
            if (firstDeleted != -1) {
                currentIndex = firstDeleted;
            }
            // Erzeuge eine Kopie der Aktie im gefundenen Slot und markiere den Slot als OCCUPIED
            table[currentIndex] = new Aktie(aktie);
            status[currentIndex] = SlotStatus::OCCUPIED;
            return true;
        }
    }
    // Kein freier oder wiederverwendbarer Slot gefunden
    return false;
}

// <---------------------- SEARCH ---------------------->
// Sucht eine Aktie in der Hashtabelle anhand des Schlüssels (hier: WKN).
// Gibt einen Zeiger auf die gefundene Aktie zurück oder nullptr, wenn nicht gefunden.

Aktie* HashTable::search(const std::string& key) {
    unsigned int startIndex = hashFunction(key, tableSize);
    // Durchlaufe alle Slots mittels quadratischer Sondierung
    for (size_t i = 0; i < tableSize; i++) {
        size_t currentIndex = (startIndex + i * i) % tableSize;
        // Ein leerer Slot (EMPTY) signalisiert das Ende der Suchkette.
        if (status[currentIndex] == SlotStatus::EMPTY) {
            return nullptr;
        }
        // Wenn der Slot besetzt (OCCUPIED) ist und die WKN übereinstimmt, wurde die Aktie gefunden.
        if (status[currentIndex] == SlotStatus::OCCUPIED && table[currentIndex]->wkn == key) {
            return table[currentIndex];
        }
        // DELETED-Slots werden ignoriert, aber die Suche wird fortgesetzt.
    }
    return nullptr; // Kein Eintrag gefunden
}

// Sucht eine Aktie anhand des Namens
Aktie* HashTable::searchByName(const std::string& name) {
    for (size_t i = 0; i < tableSize; i++) {
        if (status[i] == SlotStatus::OCCUPIED && table[i] && table[i]->name == name) {
            return table[i];
        }
    }
    return nullptr;
}

// Sucht eine Aktie anhand des Tickers
Aktie* HashTable::searchByTicker(const std::string& ticker) {
    for (size_t i = 0; i < tableSize; i++) {
        if (status[i] == SlotStatus::OCCUPIED && table[i] && table[i]->ticker == ticker) {
            return table[i];
        }
    }
    return nullptr;
}

// <---------------------- DEL ---------------------->
// Entfernt eine Aktie aus der Hashtabelle anhand des Schlüssels (WKN) mittels Lazy Deletion.
bool HashTable::remove(const std::string& key) {
    unsigned int startIndex = hashFunction(key, tableSize);
    // Durchlaufe alle Slots mittels quadratischer Sondierung
    for (size_t i = 0; i < tableSize; i++) {
        size_t currentSlot = (startIndex + i * i) % tableSize;
        // Wenn ein leerer Slot (EMPTY) erreicht wird, existiert das Element nicht.
        if (status[currentSlot] == SlotStatus::EMPTY) {
            return false;
        }
        // Wenn der Slot besetzt ist und die WKN übereinstimmt, markiere ihn als DELETED.
        if (status[currentSlot] == SlotStatus::OCCUPIED && table[currentSlot]->wkn == key) {
            status[currentSlot] = SlotStatus::DELETED;
            return true;
        }
    }
    return false;
}

// <---------------------- PRINT ---------------------->
// Gibt zur Debug-Diagnose alle belegten Slots der Hashtabelle aus.
void HashTable::print() const {
    for (size_t i = 0; i < tableSize; i++) {
        if (status[i] == SlotStatus::OCCUPIED) {
            std::cout << "Index " << i << ": "
                      << "Name: " << table[i]->name << ", "
                      << "WKN: " << table[i]->wkn << ", "
                      << "Ticker: " << table[i]->ticker << std::endl;
        }
    }
}
