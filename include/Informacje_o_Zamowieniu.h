#ifndef INFORMACJE_O_ZAMOWIENIU_H
#define INFORMACJE_O_ZAMOWIENIU_H
#include <iostream>
#include <vector>
#include "mysql.h"

struct Str_informacje_o_zamowieniu
{
    std::string imie_i_nazwisko;
    std::vector<std::string> zamowione_ksiazki;
    std::vector<std::string> ilosc_ksiazki;
    std::vector<std::string> ceny_ksiazek;
    std::string gotowe_zdanie;
    void prepare();
};

void Str_informacje_o_zamowieniu::prepare()
{
    gotowe_zdanie = "Zamówienie zawiera:";
    gotowe_zdanie+='\n';
    for(int i=0;i<zamowione_ksiazki.size();i++)
    {
        gotowe_zdanie+=zamowione_ksiazki[i]+' ';
        gotowe_zdanie+='x'+ilosc_ksiazki[i]+' ';
        gotowe_zdanie+=ceny_ksiazek[i]+"zł"+'\n';
    }
}

#endif // INFORMACJE_O_ZAMOWIENIU_H
