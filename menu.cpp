#include "menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <iomanip> 
#include <algorithm>
#include <regex>
#include <kurs.h>


// <----------------------toUpper---------------------->
// Globale Hilfsfunktion: Konvertiert einen String in Großbuchstaben
std::string toUpper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// <---------------------- ADD ---------------------->
void handleAdd(HashTable& ht) {
    // Eingabepuffer leeren
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Aktie a;
    std::cout << "Aktienname eingeben: ";
    std::getline(std::cin, a.name);
    a.name = toUpper(a.name);

    std::cout << "WKN eingeben: ";
    std::getline(std::cin, a.wkn);
    // Hier können Sie optional auch toUpper() anwenden, falls gewünscht

    std::cout << "Ticker eingeben: ";
    std::getline(std::cin, a.ticker);
    a.ticker = toUpper(a.ticker);

    // Da intern die HashTable WKN als Schlüssel verwendet, übergeben wir a.wkn als key
    if (ht.add(a, a.wkn)) {
        std::cout << "Aktie erfolgreich hinzugefügt!\n";
    } else {
        std::cout << "Fehler beim Hinzufügen! Möglicherweise existiert bereits eine Aktie mit dieser WKN.\n";
    }
}

// <---------------------- DEL ---------------------->
void handleDel(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    int mode = 0;
    std::cout << "Löschen nach (1) WKN, (2) Name oder (3) Ticker? ";
    std::cin >> mode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string input;
    Aktie* a = nullptr;
    if (mode == 1) {
        std::cout << "WKN eingeben: ";
        std::getline(std::cin, input);
        // Bei direkter WKN-Nutzung rufen wir search() auf (welches intern die WKN vergleicht)
        a = ht.search(input);
    } else if (mode == 2) {
        std::cout << "Name eingeben: ";
        std::getline(std::cin, input);
        input = toUpper(input);
        // Nehmen Sie an, dass Sie searchByName() implementiert haben
        a = ht.searchByName(input);
    } else if (mode == 3) {
        std::cout << "Ticker eingeben: ";
        std::getline(std::cin, input);
        input = toUpper(input);
        // Nehmen Sie an, dass Sie searchByTicker() implementiert haben
        a = ht.searchByTicker(input);
    } else {
        std::cout << "Ungültige Auswahl.\n";
        return;
    }
    
    if (a == nullptr) {
        std::cout << "Aktie nicht gefunden.\n";
    } else {
        // Löschen erfolgt über die WKN (primärer Schlüssel)
        if (ht.remove(a->wkn)) {
            std::cout << "Aktie erfolgreich gelöscht!\n";
        } else {
            std::cout << "Fehler beim Löschen!\n";
        }
    }
}

// <---------------------- IMPORT---------------------->
void handleImport(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Geben Sie den Namen der Aktie ein, zu der Kursdaten importiert werden sollen: ";
    std::string name;
    std::getline(std::cin, name);
    name = toUpper(name);

    // Aktie anhand des Namens suchen
    Aktie* a = ht.searchByName(name);
    if (!a) {
        std::cout << "Aktie nicht gefunden.\n";
        return;
    }

    std::cout << "Geben Sie den Dateinamen der CSV-Datei ein: ";
    std::string filename;
    std::getline(std::cin, filename);
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Die Datei " << filename << " konnte nicht geöffnet werden.\n";
        return;
    }

    // Header-Zeile überspringen (angenommen, die CSV-Datei hat einen Header)
    std::string line;
    if (!std::getline(file, line)) {
        std::cout << "Die Datei " << filename << " ist leer.\n";
        return;
    }

    // Alte Kursdaten löschen, damit der Neuimport alle bisherigen ersetzt
    a->kursdaten.clear();

    int count = 0;
    // Lese maximal 30 Kurseinträge ein
    while (std::getline(file, line) && count < 30) {
        std::stringstream ss(line);
        std::string token;
        Kurs k;
        // CSV-Format: Date,Close,Volume,Open,High,Low

        // Datum einlesen (ohne Änderung)
        if (std::getline(ss, token, ',')) {
            k.date = token;
        }
        // Close-Wert einlesen, unerwünschte Zeichen entfernen
        if (std::getline(ss, token, ',')) {
            token = std::regex_replace(token, std::regex("[^0-9\\.-]"), "");
            try { 
                k.close = std::stod(token); 
            } catch (...) { 
                k.close = 0.0; 
            }
        }
        // Volume einlesen – hier werden nur Ziffern beibehalten
        if (std::getline(ss, token, ',')) {
            token = std::regex_replace(token, std::regex("[^0-9]"), "");
            try { 
                k.volume = std::stol(token); 
            } catch (...) { 
                k.volume = 0; 
            }
        }
        // Open-Wert einlesen
        if (std::getline(ss, token, ',')) {
            token = std::regex_replace(token, std::regex("[^0-9\\.-]"), "");
            try { 
                k.open = std::stod(token); 
            } catch (...) { 
                k.open = 0.0; 
            }
        }
        // High-Wert einlesen
        if (std::getline(ss, token, ',')) {
            token = std::regex_replace(token, std::regex("[^0-9\\.-]"), "");
            try { 
                k.high = std::stod(token); 
            } catch (...) { 
                k.high = 0.0; 
            }
        }
        // Low-Wert einlesen
        if (std::getline(ss, token, ',')) {
            token = std::regex_replace(token, std::regex("[^0-9\\.-]"), "");
            try { 
                k.low = std::stod(token); 
            } catch (...) { 
                k.low = 0.0; 
            }
        }
        a->kursdaten.push_back(k);
        count++;
    }
        
    file.close();
    std::cout << "Import abgeschlossen. " << count << " Kurseinträge importiert.\n";
}

// <---------------------- SEARCH ---------------------->
void handleSearch(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int mode = 0;
    std::cout << "Suche nach (1) WKN, (2) Name oder (3) Ticker? ";
    std::cin >> mode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string input;
    Aktie* a = nullptr;
    if (mode == 1) {
        std::cout << "WKN eingeben: ";
        std::getline(std::cin, input);
        a = ht.search(input); // search() arbeitet mit WKN
    } else if (mode == 2) {
        std::cout << "Name eingeben: ";
        std::getline(std::cin, input);
        input = toUpper(input);
        a = ht.searchByName(input);
    } else if (mode == 3) {
        std::cout << "Ticker eingeben: ";
        std::getline(std::cin, input);
        input = toUpper(input);
        a = ht.searchByTicker(input);
    } else {
        std::cout << "Ungültige Auswahl.\n";
        return;
    }
    
    if (a == nullptr) {
        std::cout << "Aktie nicht gefunden.\n";
    } else {
        std::cout << "Aktie gefunden:\n"
                  << "Name: " << a->name << "\n"
                  << "WKN: " << a->wkn << "\n"
                  << "Ticker: " << a->ticker << "\n";
        if (!a->kursdaten.empty()) {
            // als Beispiel der erste Eintrag)
            const Kurs& k = a->kursdaten[0];
            std::cout << "Aktuellster Kurseintrag:\n"
                      << "Datum: " << k.date << "\n"
                      << "Close: " << k.close << "\n"
                      << "Volume: " << k.volume << "\n"
                      << "Open: " << k.open << "\n"
                      << "High: " << k.high << "\n"
                      << "Low: " << k.low << "\n";
        } else {
            std::cout << "Keine Kursdaten vorhanden.\n";
        }
    }
}

// <---------------------- PLOT ---------------------->

void handlePlot(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    int mode = 0;
    std::cout << "Plot nach (1) WKN, (2) Name oder (3) Ticker? ";
    std::cin >> mode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string input;
    Aktie* a = nullptr;
    if (mode == 1) {
        std::cout << "WKN eingeben: ";
        std::getline(std::cin, input);
        a = ht.search(input);
    } else if (mode == 2) {
        std::cout << "Name eingeben: ";
        std::getline(std::cin, input);
        input = toUpper(input);
        a = ht.searchByName(input);
    } else if (mode == 3) {
        std::cout << "Ticker eingeben: ";
        std::getline(std::cin, input);
        input = toUpper(input);
        a = ht.searchByTicker(input);
    } else {
        std::cout << "Ungültige Auswahl.\n";
        return;
    }
    
    if (a == nullptr) {
        std::cout << "Aktie nicht gefunden.\n";
        return;
    }
    
    if (a->kursdaten.empty()) {
        std::cout << "Keine Kursdaten vorhanden.\n";
        return;
    }
    
    // ASCII-Plot der Schlusskurse (Close) der letzten 30 Tage
    // Bestimme min und max
    double minClose = a->kursdaten.front().close;
    double maxClose = a->kursdaten.front().close;
    for (const auto& k : a->kursdaten) {
        minClose = std::min(minClose, k.close);
        maxClose = std::max(maxClose, k.close);
    }
    
    const int plotWidth = 50;  // Breite des Plots in Zeichen
    std::cout << "\nASCII Plot der Schlusskurse:\n";
    
    for (const auto& k : a->kursdaten) {
        // Skaliere den Schlusskurs in den Bereich [0, plotWidth]
        int numChars = static_cast<int>( ((k.close - minClose) / (maxClose - minClose)) * plotWidth );
        std::cout << std::setw(10) << k.date << " | ";
        for (int i = 0; i < numChars; i++) {
            std::cout << "*";
        }
        std::cout << " (" << k.close << ")\n";
    }
}

// <---------------------- SAVE ---------------------->

void handleSave(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string filename;
    std::cout << "Bitte Dateinamen zum Speichern eingeben: ";
    std::getline(std::cin, filename);
    
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cout << "Fehler: Datei " << filename << " konnte nicht geöffnet werden.\n";
        return;
    }
    
    // Zähle gespeicherte Aktien
    int count = 0;
    for (size_t i = 0; i < ht.getTableSize(); i++) {
        if (ht.getStatus(i) == SlotStatus::OCCUPIED && ht.getTable()[i] != nullptr) {  
            count++;
        }
    }
    ofs << count << "\n";  // Erste Zeile = Anzahl der Aktien

    // Speichere jede Aktie mit ihren Kursdaten
    for (size_t i = 0; i < ht.getTableSize(); i++) {
        if (ht.getStatus(i) == SlotStatus::OCCUPIED && ht.getTable()[i] != nullptr) {  
            Aktie* a = ht.getTable()[i];
            ofs << a->name << "|" << a->wkn << "|" << a->ticker << "|" << a->kursdaten.size() << "\n";
            
            // Speichere Kursdaten im CSV-Format
            for (const Kurs& k : a->kursdaten) {
                ofs << k.date << "," << k.close << "," << k.volume << "," 
                    << k.open << "," << k.high << "," << k.low << "\n";
            }
        }
    }

    ofs.close();
    std::cout << "Daten wurden in " << filename << " gespeichert.\n";
}

void handleLoad(HashTable& ht) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string filename;
    std::cout << "Bitte Dateinamen zum Laden eingeben: ";
    std::getline(std::cin, filename);

    std::ifstream ifs(filename);
    if (!ifs) {
        std::cout << "Fehler: Datei " << filename << " konnte nicht geöffnet werden.\n";
        return;
    }

    // HashTable leeren
    ht.clear();  

    // Anzahl der Aktien einlesen
    int count = 0;
    ifs >> count;
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout<< count << " Aktien werden geladen...\n";

    for (int i = 0; i < count; i++) {
        std::string line;
        if (!std::getline(ifs, line)) break;

        std::istringstream iss(line);
        std::string name, wkn, ticker, numKursStr;
        if (!std::getline(iss, name, '|')) continue;
        if (!std::getline(iss, wkn, '|')) continue;
        if (!std::getline(iss, ticker, '|')) continue;
        if (!std::getline(iss, numKursStr)) continue;

        int numKurs = std::stoi(numKursStr);
        Aktie a;
        a.name = toUpper(name);
        a.wkn = wkn;
        a.ticker = toUpper(ticker);

        std::cout << "Geladen -> " << a.name << " (WKN: " << a.wkn << ", Ticker: " << a.ticker << ")\n";

        // Kursdaten einlesen
        for (int j = 0; j < numKurs; j++) {
            std::string kursLine;
            if (!std::getline(ifs, kursLine)) break;

            std::istringstream kss(kursLine);
            std::string date, closeStr, volumeStr, openStr, highStr, lowStr;
            if (!std::getline(kss, date, ',')) continue;
            if (!std::getline(kss, closeStr, ',')) continue;
            if (!std::getline(kss, volumeStr, ',')) continue;
            if (!std::getline(kss, openStr, ',')) continue;
            if (!std::getline(kss, highStr, ',')) continue;
            if (!std::getline(kss, lowStr, ',')) continue;

            Kurs k;
            k.date = date;
            k.close = std::stod(closeStr);
            k.volume = std::stol(volumeStr);
            k.open = std::stod(openStr);
            k.high = std::stod(highStr);
            k.low = std::stod(lowStr);
            a.kursdaten.push_back(k);/*Die importierten Kurse werden in kursdaten.push_back(k); eingefügt. Die neuesten Daten stehen am Anfang in der CSV-Datei, aber im Vektor stehen sie am Ende.*/


        }

        // Aktie in die HashTable einfügen
        if (!ht.add(a, a.wkn)) {
            std::cout << "Warnung: Aktie mit WKN " << a.wkn << " konnte nicht hinzugefügt werden (möglicherweise Duplikat).\n";
        }
    }

    ifs.close();
    std::cout << "Daten wurden erfolgreich aus " << filename << " geladen."<<std::endl;}
