#ifndef KSIAZKA_H
#define KSIAZKA_H

#include <iostream>
#include <gtk/gtk.h>

class Ksiazka
{
private:
    int idKsiazki;
    std::string nazwa;
    std::string autor;
    int cena;
    std::string okladka;
public:
    void set_idKsiazki(const int);
    void set_nazwaKsiazki(const std::string);
    void set_autorKsiazki(const std::string);
    void set_cenaKsiazki(const int);
    void set_okladkaKsiazki(const std::string);

    const int get_idKsiazki() const ;
    const std::string get_nazwaKsiazki();
    const std::string get_autorKsiazki();
    const int get_cenaKsiazki();
    const std::string get_okladkaKsiazki();

    Ksiazka();
    Ksiazka(int,const std::string&,const std::string&,int,const std::string&);
    virtual ~Ksiazka();

    operator==(const Ksiazka &q)
    {
        if(get_idKsiazki() == q.get_idKsiazki())
        {
            return true;
        }
        else return false;
    }
};

#endif // KSIAZKA_H
