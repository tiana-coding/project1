#ifndef KURS_H
#define KURS_H
#include <string>

struct Kurs{

    std::string date; //z.b.:2025-03-14
    double close;
    long volume;
    double open;
    double high;
    double low;
};



#endif//KURS_H