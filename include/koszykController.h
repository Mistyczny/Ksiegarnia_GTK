#ifndef KOSZYKCONTROLLER_H
#define KOSZYKCONTROLLER_H
#include <map>
#include "Ksiazka.h"
#include <iostream>
#include <memory>

class koszykController
{
private:
    koszykController();
    std::map<Ksiazka*,int> zbiorKsiazek;
    int moneyToPay;
public:
        static std::unique_ptr<koszykController> koszykController_init();
        virtual ~koszykController();

        void dodajKsiazkiDoZbioru(Ksiazka*,int i=1);
        void usunKsiazkeZeZbioru(int IdKsiazki);
        bool sprawdzCzyKsiazkaJestWZbiorze(Ksiazka*);
        std::map<Ksiazka*,int>& getZbiorKsiazek();
        int getAmountOfMoneyToPay();
};

#endif // KOSZYKCONTROLLER_H
