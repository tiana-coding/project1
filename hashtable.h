#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include "aktie.h"
#include "slotstatus.h"
#include "hashfunction.h"

class HashTable {
public:
    HashTable(size_t size = 1031);
    bool add(const Aktie& aktie, const std::string& key);
  
    // Getter für TableSize
    size_t getTableSize() const { return tableSize; }

    // Getter für Status eines bestimmten Slots
    SlotStatus getStatus(size_t index) const {
        return (index < status.size()) ? status[index] : SlotStatus::EMPTY;
    }

    // Getter für die gesamte Tabelle (Referenz zurückgeben)
    const std::vector<Aktie*>& getTable() const { return table; }


    // Search by primary key (WKN)
    Aktie* search(const std::string& key);
    // Remove using primary key (WKN)
    bool remove(const std::string& key);
    
    // Additional search functions for Name and Ticker:
    Aktie* searchByName(const std::string& name);
    Aktie* searchByTicker(const std::string& ticker);
    
    void print() const;
    void clear();

    ~HashTable();
    
    private:

    size_t tableSize;                
    std::vector<Aktie*> table;         
    std::vector<SlotStatus> status;    
};

#endif // HASHTABLE_H
