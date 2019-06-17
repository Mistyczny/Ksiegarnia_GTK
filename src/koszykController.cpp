#include "koszykController.h"

std::unique_ptr<koszykController> koszykController::koszykController_init()
{
    std::unique_ptr<koszykController> p(new koszykController());
return p;
}

koszykController::koszykController()
{
}

koszykController::~koszykController()
{
}

void koszykController::dodajKsiazkiDoZbioru(Ksiazka* temp,int ilosc)
{
    this->moneyToPay+= temp->get_cenaKsiazki()*ilosc;

    for(const auto &pair : zbiorKsiazek)
    {
        if(pair.first->get_idKsiazki() == temp->get_idKsiazki())
        {
            zbiorKsiazek[pair.first]+=ilosc;
            return;
        }
    }
    zbiorKsiazek[temp]=ilosc;
}

void koszykController::usunKsiazkeZeZbioru(int IdKsiazki)
{
    for(auto it = zbiorKsiazek.begin();it!=zbiorKsiazek.end();it++)
    {
        if(it->first->get_idKsiazki() == IdKsiazki)
        {
            zbiorKsiazek[it->first]--;
            this->moneyToPay-= it->first->get_cenaKsiazki();

            if(zbiorKsiazek[it->first]==0)
            {
                zbiorKsiazek.erase(it);
            }
        }
    }
}

bool koszykController::sprawdzCzyKsiazkaJestWZbiorze(Ksiazka* temp)
{
    for(const auto &pair : zbiorKsiazek)
    {
        if(pair.first->get_idKsiazki() == temp->get_idKsiazki())
        {
            return true;
        }
    }
return false;
}

std::map<Ksiazka*,int>& koszykController::getZbiorKsiazek()
{
    return this->zbiorKsiazek;
}


int koszykController::getAmountOfMoneyToPay()
{
    return this->moneyToPay;
}
