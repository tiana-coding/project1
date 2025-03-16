#ifndef AKTIE_H
#define AKTIE_H
#include <string>
#include <vector>
#include "kurs.h"

struct Aktie{

    std::string name;
    std::string wkn;
    std::string ticker;
    std::vector<Kurs> kursdaten;




};

#endif//AKTIE_H