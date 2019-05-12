#include "Ksiazka.h"
#include "Shop.h"

Ksiazka::Ksiazka()
{

}

Ksiazka::Ksiazka(int id_,const std::string& nazwa_,const std::string& autor_,int cena_,const std::string& okl) :
                                                                                                idKsiazki(id_),
                                                                                                nazwa(nazwa_),
                                                                                                autor(autor_),
                                                                                                cena(cena_),
                                                                                                okladka(okl)
{

}

Ksiazka::~Ksiazka()
{

}

void Ksiazka::set_idKsiazki(const int id_ksiazki_)
{
    this->idKsiazki = id_ksiazki_;
}

void Ksiazka::set_nazwaKsiazki(const std::string nazwa_)
{
    this->nazwa = nazwa_;
}

void Ksiazka::set_autorKsiazki(const std::string autor_)
{
    this->autor = autor_;
}

void Ksiazka::set_cenaKsiazki(const int cena_)
{
    this->cena = cena_;
}

void Ksiazka::set_okladkaKsiazki(const std::string okladka_)
{
    this->okladka = okladka_;
}

const int Ksiazka::get_idKsiazki() const
{
    return this->idKsiazki;
}

const std::string Ksiazka::get_nazwaKsiazki()
{
    return this->nazwa;
}

const std::string Ksiazka::get_autorKsiazki()
{
    return this->autor;
}

const int Ksiazka::get_cenaKsiazki()
{
    return this->cena;
}

const std::string Ksiazka::get_okladkaKsiazki()
{
    return this->okladka;
}
